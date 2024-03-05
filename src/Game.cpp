#include <iostream>
#include <memory>

#include "Game.h"
#include "Enemy/Enemy.h"
#include "Position.h"
#include "Projectile/Bullet.h"
#include "Player/Player.h"
#include "Wall.h"

#include "Enemy/EnemyShooter.h"
#include "Enemy/EnemyTurret.h"
#include "Enemy/EnemySpawner.h"
#include "Enemy/EnemySniper.h"
#include "Enemy/EnemyCharger.h"
#include "Enemy/EnemySeeker.h"
#include "Enemy/EnemyStats.h"
#include "Player/PlayerStats.h"

Game::Game(int FPS) {
    setStatsScaleWithFPS(FPS);

    player = Player();

    bulletsEnemy = std::make_shared<std::vector<std::unique_ptr<Bullet>>>();
    bulletsAlly = std::make_shared<std::vector<std::unique_ptr<Bullet>>>();
    bombshells = std::make_shared<std::vector<std::unique_ptr<Bombshell>>>();
    walls = std::make_shared<std::vector<std::unique_ptr<Wall>>>();
    enemies = std::make_shared<std::vector<std::unique_ptr<Enemy>>>();

    if (!font.loadFromFile("./fonts/FiraCode.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }
    
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 975);
    text.setCharacterSize(15);

    //A CHANGER --- DEBUG ROOM ---
    walls->push_back(std::make_unique<Wall>(Position(500, 500)));
    enemies->push_back(std::make_unique<EnemyShooter>(Position(100, 100)));
    enemies->push_back(std::make_unique<EnemyShooter>(Position(100, 100)));
    enemies->push_back(std::make_unique<EnemyTurret>(Position(200, 200)));
    enemies->push_back(std::make_unique<EnemySpawner>(Position(300, 300)));
    enemies->push_back(std::make_unique<EnemySniper>(Position(400, 400)));
    enemies->push_back(std::make_unique<EnemyCharger>(Position(500, 600)));
}

Game::~Game(){}

bool isBulletOOB(const std::unique_ptr<Bullet>& bullet){
    Position bulletPos = bullet->getPosition();
    float bulletSize = bullet->getSize();

    return (bulletPos.x < - bulletSize * 2 || bulletPos.y < - bulletSize * 2 ||
        bulletPos.x > 1000 + bulletSize * 2 || bulletPos.y > 1000 + bulletSize * 2);
}

bool isBulletInWalls(const std::unique_ptr<Bullet>& bullet, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls){
    Position wallPos, bulletWallPos;
    float angleWallBullet;
    Position bulletPos = bullet->getPosition();
    float bulletSize = bullet->getSize();

    for (unsigned int indexWall = 0; indexWall<walls->size(); indexWall++) {
        wallPos = walls->at(indexWall)->getPosition();
        angleWallBullet = atan2(wallPos.y - bulletPos.y, bulletPos.x - wallPos.x);
        bulletWallPos = bulletPos + Position(-cos(angleWallBullet), sin(angleWallBullet)) * bulletSize;

        if (walls->at(indexWall)->isInWall(bulletWallPos)) {
            return true;
        }
    }

    return false;
}

void destroyBulletsOOBinWall(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls){
    Position bulletPos, wallPos;
    float bulletSize, angleWallBullet;
    bool destroyed;

    for (auto bullet = bullets->begin(); bullet != bullets->end();) {
        if(isBulletOOB(*bullet)) {
            bullet = bullets->erase(bullet);
        } else if(isBulletInWalls(*bullet, walls)){
            bullet = bullets->erase(bullet);
        } else {   
            bullet = std::next(bullet);
        }
    }
}

void Game::checkBulletsOOB(){
    for(int i=0; i < 2; i++){
        destroyBulletsOOBinWall(i==0 ? bulletsAlly : bulletsEnemy, walls);
    }
}

bool isWallInBombshell(const std::unique_ptr<Wall>& wall, const std::unique_ptr<Bombshell>& bombshell){
    Position bombshellPos = bombshell->getPosition();
    Position wallPos = wall->getPosition();
    float angleWallBombshell = atan2(wallPos.y - bombshellPos.y, bombshellPos.x - wallPos.x);
    Position bulletWallPos = bombshellPos + Position(-cos(angleWallBombshell), sin(angleWallBombshell)) * bombshell->getSize();
            
    return (wall->isInWall(bulletWallPos));
}

void Game::destroyWalls(){
    for(unsigned int indexBomb = 0; indexBomb < bombshells->size(); indexBomb++){
        for (auto wall = walls->begin(); wall != walls->end();) {
            if (isWallInBombshell(*wall, bombshells->at(indexBomb))) {
                wall = walls->erase(wall);
            } else {
                wall = std::next(wall);
            }
        }
    }
}

bool isBulletInBombshell(const std::unique_ptr<Bullet>& bullet, const std::unique_ptr<Bombshell>& bombshell) {
    Position bombshellPos = bombshell->getPosition();
    float bombshellSize = bombshell->getSize();
    Position diffPos = bombshellPos - bullet->getPosition();
    float hitBoxBoth = bombshellSize + bullet->getSize();
        
    return (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth*hitBoxBoth);
}

void destroyBulletsInBombshells(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>> bombshells){
    for(unsigned int indexBomb = 0; indexBomb < bombshells->size(); indexBomb++){
        for(auto bullet = bullets->begin(); bullet != bullets->end();){
            if(isBulletInBombshell(*bullet, bombshells->at(indexBomb))){
                bullet = bullets->erase(bullet);
            } else {
                bullet = std::next(bullet);
            }
        }
    }
}

void Game::checkBulletsCollisionWithBombshells(){
    for(int i=0; i<2; i++){
        destroyBulletsInBombshells(i==0 ? bulletsAlly : bulletsEnemy, bombshells);
    }
}

bool isBulletAllyInBulletEnemy(const std::unique_ptr<Bullet>& bulletAlly, const std::unique_ptr<Bullet>& bulletEnemy){
        Position diffPos = bulletAlly->getPosition() - bulletEnemy->getPosition();
        float hitBoxBoth = bulletAlly->getSize() + bulletEnemy->getSize();
        return (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth * hitBoxBoth);
}

void Game::checkBulletAllyCollisionBulletEnemy(){
    Position diffPos;
    float hitBoxBoth;
    bool allyBulletDestroyed;

    for (auto bulletAlly = bulletsAlly->begin(); bulletAlly != bulletsAlly->end();) {
        allyBulletDestroyed = false;

        for (auto bulletEnemy = bulletsEnemy->begin(); bulletEnemy != bulletsEnemy->end();) {
            if (!bulletEnemy->get()->isDestructible()) {
                bulletEnemy = std::next(bulletEnemy);
                continue;
            }

            if (isBulletAllyInBulletEnemy(*bulletAlly, *bulletEnemy)) {
                if ((*bulletAlly)->isDestructible()) {
                    bulletAlly = bulletsAlly->erase(bulletAlly);
                    allyBulletDestroyed = true;
                }

                bulletEnemy = bulletsEnemy->erase(bulletEnemy);
                break;
            }

            bulletEnemy = std::next(bulletEnemy);
        }

        if (!allyBulletDestroyed) {
            bulletAlly = std::next(bulletAlly);
        }
    }
}

void Game::bulletCollisions() {
    checkBulletsOOB();
    checkBulletAllyCollisionBulletEnemy();
    checkBulletsCollisionWithBombshells();
}

void Game::update(sf::RenderWindow& window, float deltaTime){
        //-----PLAYER UPDATE-----
        player.update(window,bulletsAlly,bombshells,walls,deltaTime);
        player.receiveDamageIfShot(bulletsEnemy, bombshells);
        player.receiveDamageIfHit(enemies);

        //-----ENEMIES UPDATE-----
        for(unsigned int indexEnemy=0; indexEnemy<enemies->size(); indexEnemy++){
            enemies->at(indexEnemy)->update(bulletsEnemy, player, walls, enemies, deltaTime);
            enemies->at(indexEnemy)->receiveDamageIfShot(bulletsAlly, bombshells);
        }

        //-----BULLETS UPDATE-----
        //ENEMY'S BULLETS
        for(unsigned int indexBullet=0; indexBullet<bulletsEnemy->size(); indexBullet++){
            bulletsEnemy->at(indexBullet)->update();
        }

        //ALLY'S BULLET
        for(unsigned int indexBullet=0; indexBullet<bulletsAlly->size(); indexBullet++){
            bulletsAlly->at(indexBullet)->update();
        }

        //BOMBSHELLS
        auto bombshell = bombshells->begin();
        while(bombshell != bombshells->end()) {
            bombshell->get()->update(deltaTime);
            if(bombshell->get()->isDeletable()){
                bombshell = bombshells->erase(bombshell);
            } else {
                bombshell++;
            }
        }

        bulletCollisions();
        destroyWalls();

        //-----ENEMIES DEAD-----
        auto enemy = enemies->begin();
        while (enemy != enemies->end()) {
            if(enemy->get()->isDead()){
                messageTerminal.push_back("delete " + (*enemy)->getType());
                enemy = enemies->erase(enemy);
            } else {
                enemy = std::next(enemy);
            }
        }
}

void Game::draw(sf::RenderWindow& window, float deltaTime){
        drawFakeTerminal(window, deltaTime);
        drawCursorTerminal(window, deltaTime);

        for(int i = 0; i < enemies->size(); i++){
            enemies->at(i)->drawEffects(window);
        }

        for(int i = 0; i < bombshells->size(); i++){
            bombshells->at(i)->drawExplosion(window);
        }

        for(int i = 0; i < bulletsAlly->size(); i++){
            bulletsAlly->at(i)->draw(window);
        }

        for(int i = 0; i < bulletsEnemy->size(); i++){
            bulletsEnemy->at(i)->draw(window);
        }

        //ENEMIES
        for(int i = 0; i < enemies->size(); i++){
            enemies->at(i)->draw(window);
        }

        player.draw(window);

        for(int i = 0; i < walls->size(); i++){
            walls->at(i)->draw(window);
        }

        for(int i = 0; i < bombshells->size(); i++){
            bombshells->at(i)->draw(window);
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

    if (displayedMessage.size() < messageTerminal[0].size() && displayedMessageTimer > 0.1) {
        displayedMessage += messageTerminal[0][displayedMessage.size()];
        displayedMessageTimer = 0;
    } else if (displayedMessageTimer > 1) {
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
