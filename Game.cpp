#include <iostream>
#include <memory>

#include "Game.h"
#include "Enemy.h"
#include "Position.h"
#include "Bullet.h"
#include "Player.h"
#include "Wall.h"

#include "EnemyShooter.h"
#include "EnemyTurret.h"
#include "EnemySpawner.h"
#include "EnemySniper.h"
#include "EnemyCharger.h"
#include "EnemySeeker.h"
#include "EnemyStats.h"
#include "PlayerStats.h"

Game::Game(int FPS) {
    setStatsScaleWithFPS(FPS);

    player = Player();

    if (!font.loadFromFile("./fonts/FiraCode.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }
    
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 975);
    text.setCharacterSize(15);

    walls.push_back(std::make_unique<Wall>(Position(500, 500)));
    enemies.push_back(std::make_unique<EnemyShooter>(Position(100, 100)));
    enemies.push_back(std::make_unique<EnemyShooter>(Position(100, 100)));
    enemies.push_back(std::make_unique<EnemyShooter>(Position(100, 100)));
    enemies.push_back(std::make_unique<EnemyShooter>(Position(100, 100)));
    enemies.push_back(std::make_unique<EnemyShooter>(Position(100, 100)));
    //enemies.push_back(std::make_unique<EnemyShooter>(Position(200, 100)));
    //enemies.push_back(std::make_unique<EnemyTurret>(Position(200, 200)));
    //enemies.push_back(std::make_unique<EnemySpawner>(Position(300, 300)));
    //enemies.push_back(std::make_unique<EnemySniper>(Position(400, 400)));
    enemies.push_back(std::make_unique<EnemyCharger>(Position(500, 600)));
}

Game::~Game(){}

void Game::destroyInWallOrOutOfBounds(std::vector<std::unique_ptr<Bullet>> &bullets) {
    Position bulletPos, wallPos;
    float bulletSize, angleWallBullet;
    bool destroyed;

    for (auto bullet = bullets.begin(); bullet != bullets.end();) {
        destroyed = false;
        bulletPos = (*bullet)->getPosition();
        bulletSize = (*bullet)->getSize();


        if(bulletPos.x < - bulletSize * 2 || bulletPos.y < - bulletSize * 2 ||
        bulletPos.x > 1000 + bulletSize * 2 || bulletPos.y > 1000 + bulletSize * 2){
            bullet -> reset();
            bullet = bullets.erase(bullet);
            continue;
        }

        // If the bullet touches a wall
        for (const auto& wall : walls) {
            wallPos = wall->getPosition();
            angleWallBullet = atan2(wallPos.y - bulletPos.y, bulletPos.x - wallPos.x);
            Position bulletWallPos = bulletPos + Position(-cos(angleWallBullet), sin(angleWallBullet)) * bulletSize;
            if (wall->isInWall(bulletWallPos)) {
                bullet->reset();
                bullet = bullets.erase(bullet);
                destroyed = true;
                break;
            }
        }

        if(!destroyed) bullet++;
    }
}

void Game::destroyWalls(std::vector<std::unique_ptr<Wall>> &walls){
    Position wallPos, bombshellPos;
    float angleWallBombshell;
    for(const auto&bombshell : bombshells){
        bombshellPos = bombshell->getPosition();
        for(auto wall = walls.begin(); wall != walls.end();){
            wallPos = (*wall)->getPosition();
            angleWallBombshell = atan2(wallPos.y - bombshellPos.y, bombshellPos.x - wallPos.x);
            Position bulletWallPos = bombshellPos + Position(-cos(angleWallBombshell), sin(angleWallBombshell)) * bombshell->getSize();
            if ((*wall)->isInWall(bulletWallPos)) {
                wall->reset();
                wall = walls.erase(wall);
            }
            else{
                wall++;
            }
        }
    }
}

void Game::bulletCollisions(){
    Position diffPos;
    float hitBoxBoth;
    bool allyBulletDestroyed;
    for(auto bulletAlly = bulletsAlly.begin(); bulletAlly != bulletsAlly.end();){
        if((*bulletAlly)->getSize() <= 0){
            bulletAlly++;
            continue;
        }

        //If the ally bullet touch a enemy bullet that is destructible -> destroy both bullets
        allyBulletDestroyed = false;
        for(auto bulletEnemy = bulletsEnemy.begin(); bulletEnemy != bulletsEnemy.end();){
            if (!(*bulletEnemy)->isDestructible() || (*bulletEnemy)->getSize() <= 0){
                bulletEnemy++;
                continue;
            }

            diffPos = (*bulletAlly)->getPosition() - (*bulletEnemy)->getPosition();
            hitBoxBoth = (*bulletAlly)->getSize() + (*bulletEnemy)->getSize();
            if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth*hitBoxBoth) {
                if((*bulletAlly)->isDestructible()){
                    bulletAlly -> reset();
                    bulletAlly = bulletsAlly.erase(bulletAlly);
                    allyBulletDestroyed = true;
                }
                bulletEnemy -> reset();
                bulletEnemy = bulletsEnemy.erase(bulletEnemy);
                break;
            }
        
            bulletEnemy++;
        }

        if(!allyBulletDestroyed) bulletAlly++;
    }

    Position bombshellPos;
    float bombshellSize;
    for(const auto&bombshell : bombshells){
        bombshellPos = bombshell->getPosition();
        bombshellSize = bombshell->getSize();
        
        if(bombshellSize <= 0)   
            continue;

        for(auto bulletEnemy = bulletsEnemy.begin(); bulletEnemy != bulletsEnemy.end();){
            if(!(*bulletEnemy)->isDestructible()){
                bulletEnemy++;
                continue;
            }
            diffPos = bombshellPos - (*bulletEnemy)->getPosition();
            hitBoxBoth = bombshellSize + (*bulletEnemy)->getSize();
            if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth*hitBoxBoth) {
                bulletEnemy -> reset();
                bulletEnemy = bulletsEnemy.erase(bulletEnemy);
                continue;
            }
            bulletEnemy++;
        }

        for(auto bulletAlly = bulletsAlly.begin(); bulletAlly != bulletsAlly.end();){
            if(!(*bulletAlly)->isDestructible()){
                bulletAlly++;
                continue;
            }
            diffPos = bombshellPos - (*bulletAlly)->getPosition();
            hitBoxBoth = bombshellSize + (*bulletAlly)->getSize();
            if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth*hitBoxBoth) {
                bulletAlly -> reset();
                bulletAlly = bulletsAlly.erase(bulletAlly);
                continue;
            }
            bulletAlly++;
        }
    }
}

void Game::update(sf::RenderWindow& window, float deltaTime){
        //-----PLAYER UPDATE-----
        player.update(window,bulletsAlly,bombshells,walls,deltaTime);


        //-----ENEMIES UPDATE-----
        for(auto& enemy : enemies){
            enemy->update(bulletsEnemy, player, walls, enemies, deltaTime);
        }

        //-----BULLETS UPDATE-----
        //ENEMY'S BULLETS
        for(auto& bullet : bulletsEnemy){
            bullet->update();
        }

        destroyInWallOrOutOfBounds(bulletsEnemy);

        //ALLY'S BULLET
        for(auto& bullet : bulletsAlly){
            bullet->update();
        }

        destroyInWallOrOutOfBounds(bulletsAlly);

        //BOMBSHELLS
        for(auto bombshell = bombshells.begin(); bombshell != bombshells.end();) {
            (*bombshell)->update(deltaTime);
            if((*bombshell)->isDeletable()){
                bombshell -> reset();
                bombshell = bombshells.erase(bombshell);
            }
            else bombshell++;
        }

        destroyWalls(walls);

        bulletCollisions();
        
        //-----PLAYER AND ENEMY BULLETS-----
        player.receiveDamageIfShot(bulletsEnemy, bombshells);

        //-----ENEMIES AND ALLY BULLETS-----
        for(auto& enemy : enemies) {
            enemy->receiveDamageIfShot(bulletsAlly, bombshells);
        }

        //-----PLAYER AND ENEMIES-----
        player.receiveDamageIfHit(enemies);

        //-----ENEMIES DEAD-----
        for(auto enemy = enemies.begin(); enemy != enemies.end();) {
            if((*enemy)->isDead()){
                messageTerminal.push_back("delete [] " + (*enemy)->getType());
                enemy->reset();
                enemy = enemies.erase(enemy);
            }
            else enemy++;
        }
}

void Game::draw(sf::RenderWindow& window, float deltaTime){
        drawFakeTerminal(window, deltaTime);
        drawCursorTerminal(window, deltaTime);

        for(auto& enemy : enemies){
            enemy->drawEffects(window);
        }

        for(auto& bombshell : bombshells){
            bombshell->drawExplosion(window);
        }

        for(auto& bullet : bulletsAlly){
            bullet->draw(window);
        }

        for(auto& bullet : bulletsEnemy){
            bullet->draw(window);
        }

        //ENEMIES
        for(auto& enemy : enemies){
            enemy->draw(window);
        }

        player.draw(window);

        for(auto& wall : walls){
            wall->draw(window);
        }

        for(auto& bombshell : bombshells){
            bombshell->draw(window);
        }

        player.drawHealth(window);
}


void Game::setStatsScaleWithFPS(int FPS) {
    float ratioFPS = (float)60/FPS;

    Player::stats.speedFactor = ratioFPS;
    Player::stats.speedBulletFactor = ratioFPS;

    EnemyShooter::stats.speedFactor = ratioFPS;
    EnemyShooter::stats.turnSpeedFactor = ratioFPS;
    EnemyShooter::stats.speedBulletFactor = ratioFPS;

    EnemyCharger::stats.speedFactor = ratioFPS;
    EnemyCharger::stats.turnSpeedFactor = ratioFPS;
    EnemyCharger::stats.speedBulletFactor = ratioFPS;

    EnemySniper::stats.speedFactor = ratioFPS;
    EnemySniper::stats.turnSpeedFactor = ratioFPS;
    EnemySniper::stats.speedBulletFactor = ratioFPS;

    EnemySeeker::stats.speedFactor = ratioFPS;
    EnemySeeker::stats.turnSpeedFactor = ratioFPS;
    EnemySeeker::stats.speedBulletFactor = ratioFPS;

    EnemySpawner::stats.speedFactor = ratioFPS;
    EnemySpawner::stats.turnSpeedFactor = ratioFPS;
    EnemySpawner::stats.speedBulletFactor = ratioFPS;

    EnemyTurret::stats.speedFactor = ratioFPS;
    EnemyTurret::stats.turnSpeedFactor = ratioFPS;
    EnemyTurret::stats.speedBulletFactor = ratioFPS;
}

void Game::drawFakeTerminal(sf::RenderWindow& window, float deltaTime){ 
    if(messageTerminal.empty()) return;

    displayedMessageTimer += deltaTime;

    if(displayedMessage.size() < messageTerminal[0].size() && displayedMessageTimer > 0.1) {
        displayedMessage += messageTerminal[0][displayedMessage.size()];
        displayedMessageTimer = 0;
    }
    else if(displayedMessageTimer > 1) {
        displayedMessageTimer = 0;
        messageTerminal.erase(messageTerminal.begin());
        displayedMessage = "";
    }
    
    text.setString(displayedMessage);
    window.draw(text);
}

void Game::drawCursorTerminal(sf::RenderWindow& window, float deltaTime) {
    cursorTimer += deltaTime;
    if (cursorVisible) {
        sf::RectangleShape cursor(sf::Vector2f(2, text.getCharacterSize()));
        cursor.setPosition(text.getPosition().x + text.getLocalBounds().width + 5, text.getPosition().y + 2);
        cursor.setFillColor(sf::Color::White);
        window.draw(cursor);
    }

    if (cursorTimer >= 0.5) {
        cursorVisible = !cursorVisible;
        cursorTimer = 0;
    }
}
