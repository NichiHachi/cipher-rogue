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

Game::Game() {
    player = Player();
    player.spawn();

    bulletsEnemy = std::make_shared<std::vector<std::unique_ptr<Bullet>>>();
    bulletsAlly = std::make_shared<std::vector<std::unique_ptr<Bullet>>>();
    bombshells = std::make_shared<std::vector<std::unique_ptr<Bombshell>>>();
    walls = std::make_shared<std::vector<std::unique_ptr<Wall>>>();
    enemies = std::make_shared<std::vector<std::unique_ptr<Enemy>>>();

    if (!font.loadFromFile("../fonts/FiraCode.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }
    
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 975);
    text.setCharacterSize(15);

    //A CHANGER --- DEBUG ROOM ---
    walls->push_back(std::make_unique<Wall>(Position(9, 9)));
    walls->push_back(std::make_unique<Wall>(Position(10, 9)));
    walls->push_back(std::make_unique<Wall>(Position(11, 9)));
    walls->push_back(std::make_unique<Wall>(Position(12, 9)));
    walls->push_back(std::make_unique<Wall>(Position(13, 9)));
    walls->push_back(std::make_unique<Wall>(Position(14, 9)));
    walls->push_back(std::make_unique<Wall>(Position(15, 9)));
    walls->push_back(std::make_unique<Wall>(Position(16, 9)));
    walls->push_back(std::make_unique<Wall>(Position(10, 9)));

    walls->push_back(std::make_unique<Wall>(Position(9, 10)));

    walls->push_back(std::make_unique<Wall>(Position(9, 11)));
    walls->push_back(std::make_unique<Wall>(Position(11, 11)));
    walls->push_back(std::make_unique<Wall>(Position(12, 11)));
    walls->push_back(std::make_unique<Wall>(Position(13, 11)));
    walls->push_back(std::make_unique<Wall>(Position(14, 11)));
    walls->push_back(std::make_unique<Wall>(Position(15, 11)));
    walls->push_back(std::make_unique<Wall>(Position(16, 11)));
    walls->push_back(std::make_unique<Wall>(Position(17, 11)));
    walls->push_back(std::make_unique<Wall>(Position(18, 11)));
    walls->push_back(std::make_unique<Wall>(Position(19, 11)));
    walls->push_back(std::make_unique<Wall>(Position(20, 11)));

    walls->push_back(std::make_unique<Wall>(Position(9, 12)));

    walls->push_back(std::make_unique<Wall>(Position(9, 13)));
    walls->push_back(std::make_unique<Wall>(Position(10, 13)));
    walls->push_back(std::make_unique<Wall>(Position(11, 13)));
    walls->push_back(std::make_unique<Wall>(Position(12, 13)));
    walls->push_back(std::make_unique<Wall>(Position(13, 13)));
    walls->push_back(std::make_unique<Wall>(Position(14, 13)));
    walls->push_back(std::make_unique<Wall>(Position(15, 13)));
    walls->push_back(std::make_unique<Wall>(Position(16, 13)));
    walls->push_back(std::make_unique<Wall>(Position(10, 13)));


    walls->push_back(std::make_unique<Wall>(Position(9, 14)));
    walls->push_back(std::make_unique<Wall>(Position(9, 15)));
    walls->push_back(std::make_unique<Wall>(Position(9, 16)));
    walls->push_back(std::make_unique<Wall>(Position(9, 17)));
    walls->push_back(std::make_unique<Wall>(Position(9, 18)));
    walls->push_back(std::make_unique<Wall>(Position(9, 19)));
    walls->push_back(std::make_unique<Wall>(Position(9, 20)));


    walls->push_back(std::make_unique<Wall>(Position(6, 10)));
    
    enemies->push_back(std::make_unique<EnemyShooter>(Position(100, 100)));
    enemies->push_back(std::make_unique<EnemyTurret>(Position(200, 200)));
    enemies->push_back(std::make_unique<EnemySeeker>(Position(200, 200), 0));
    enemies->push_back(std::make_unique<EnemySpawner>(Position(300, 300)));
    enemies->push_back(std::make_unique<EnemySniper>(Position(400, 400)));
    enemies->push_back(std::make_unique<EnemyCharger>(Position(400, 300)));
}

Game::~Game() = default;

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

    for (const auto & wall : *walls) {
        wallPos = wall->getPosition();
        angleWallBullet = std::atan2(wallPos.y - bulletPos.y, bulletPos.x - wallPos.x);
        bulletWallPos = bulletPos + Position(-std::cos(angleWallBullet), std::sin(angleWallBullet)) * bulletSize;

        if (wall->isInWall(bulletWallPos)) {
            return true;
        }
    }

    return false;
}

void destroyBulletsOOBinWall(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls){
    for (auto bullet = bullets->begin(); bullet != bullets->end();) {
        if (isBulletOOB(*bullet) || isBulletInWalls(*bullet, walls)) {
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
    float angleWallBombshell = std::atan2(wallPos.y - bombshellPos.y, bombshellPos.x - wallPos.x);
    Position bulletWallPos = bombshellPos + Position(-std::cos(angleWallBombshell), std::sin(angleWallBombshell)) * bombshell->getSize();
            
    return (wall->isInWall(bulletWallPos));
}

void Game::destroyWalls(){
    for(const auto & bombshell : *bombshells){
        for (auto wall = walls->begin(); wall != walls->end();) {
            if (isWallInBombshell(*wall, bombshell)) {
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

void destroyBulletsInBombshells(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells){
    for(const auto & bombshell : *bombshells){
        for(auto bullet = bullets->begin(); bullet != bullets->end();){
            if(isBulletInBombshell(*bullet, bombshell)){
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

                bulletsEnemy->erase(bulletEnemy);
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
        for(const auto & enemy : *enemies){
            if(enemy == nullptr)
                continue;
            enemy->update(bulletsEnemy, player, walls, enemies, deltaTime);
            enemy->receiveDamageIfShot(bulletsAlly, bombshells);
        }

        //-----BULLETS UPDATE-----
        //ENEMY'S BULLETS
        for(const auto & indexBullet : *bulletsEnemy){
            indexBullet->update(deltaTime);
        }

        //ALLY'S BULLET
        for(const auto & indexBullet : *bulletsAlly){
            indexBullet->update(deltaTime);
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

        for(const auto & i : *enemies){
            i->drawEffects(window);
        }

        for(const auto & i : *bombshells){
            i->drawExplosion(window);
        }

        for(const auto & i : *bulletsAlly){
            i->draw(window);
        }

        for(const auto & i : *bulletsEnemy){
            i->draw(window);
        }

        //ENEMIES
        for(const auto & i : *enemies){
            i->draw(window);
        }

        player.draw(window);

        for(const auto & i : *walls){
            i->draw(window);
        }

        for(const auto & i : *bombshells){
            i->draw(window);
        }

        player.drawHealth(window);
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
        sf::RectangleShape cursor(sf::Vector2f(2, static_cast<float>(text.getCharacterSize())));
        cursor.setPosition(text.getPosition().x + text.getLocalBounds().width + 5, text.getPosition().y + 2);
        cursor.setFillColor(sf::Color::White);
        window.draw(cursor);
    }

    if (cursorTimer >= 0.5) {
        cursorVisible = !cursorVisible;
        cursorTimer = 0;
    }
}
