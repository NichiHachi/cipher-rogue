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
#include <random>

Game::Game() : player(Player()), bulletsEnemy(std::make_shared<std::vector<std::unique_ptr<Bullet>>>()), bulletsAlly(std::make_shared<std::vector<std::unique_ptr<Bullet>>>()), bombshells(std::make_shared<std::vector<std::unique_ptr<Bombshell>>>()), walls(std::make_shared<std::vector<std::unique_ptr<Wall>>>()), enemies(std::make_shared<std::vector<std::unique_ptr<Enemy>>>()), messageTerminal(std::vector<std::string>()), levelStarted(false), level(1){    
    for(unsigned int i=0; i<10; i++){
        mapSelectionHistory[i] = 0;
    }
    
    if (!font.loadFromFile("./fonts/FiraCode.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }
    
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 975);
    text.setCharacterSize(15);

    enemies->push_back(std::make_unique<EnemyShooter>(Position(100,100)));
    enemies->push_back(std::make_unique<EnemyTurret>(Position(200,200)));
    enemies->push_back(std::make_unique<EnemySpawner>(Position(300,300)));
    enemies->push_back(std::make_unique<EnemySniper>(Position(400,400)));
    enemies->push_back(std::make_unique<EnemyCharger>(Position(400,500)));
    enemies->push_back(std::make_unique<EnemySeeker>(Position(600,600), 0));
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


bool isClose(Position pos1, Position pos2){
    Position diffPos = pos1 - pos2;
    return diffPos.x*diffPos.x + diffPos.y*diffPos.y <= 10000;
}

void Game::update(sf::RenderWindow& window, float deltaTime){
    if(!levelStarted){
        startTime += deltaTime;
        if(startTime > 3){
            startTime = 0;
            levelStarted = true;
        }
    } else {
        if(enemies->size() == 1){
            for(unsigned int i=0; i<bulletsAlly->size(); i++){
                if(isClose(bulletsAlly->at(i)->getPosition(), enemies->at(0)->getPosition()) && bulletsAlly->at(i)->getDamage() >= enemies->at(0)->getHp()){
                    deltaTime/=5;
                        break;
                }
            }
        }

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
            if(*enemy == nullptr){
              enemy = enemies->erase(enemy);
            } else if(enemy->get()->isDead()) {
                messageTerminal.push_back("delete " + (*enemy)->getType());
                enemy = enemies->erase(enemy);
            } else {
                enemy = std::next(enemy);
            }
        }

        //-----LEVEL COMPLETED-----
        if(enemies->empty()){
            startTime += deltaTime;
            if(startTime > 3){
                messageTerminal.push_back("Level " + std::to_string(level) + " completed");
                level++;
                initLevel();
            }
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

        if(startTime > 0){
            displayTimer(window);
        }
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

void Game::initLevel(){
    createMap();

    bulletsAlly->clear();
    bulletsEnemy->clear();
    bombshells->clear();
    startTime = 0;
    levelStarted = false;
}

void Game::createMap(){
    walls->clear();
    int selectedMap = selectMap();
    switch(selectedMap){
        case 0:
            putWallFromTo(Position(3,3), Position(5,3));
            putWallFromTo(Position(3,4), Position(3,5));

            putWallFromTo(Position(11,6), Position(13,6));
            putWallFromTo(Position(13,7), Position(13,8));

            putWallFromTo(Position(6,11), Position(6,13));
            putWallFromTo(Position(7,13), Position(8,13));

            putWallFromTo(Position(14,16), Position(16,16));
            putWallFromTo(Position(16,15), Position(16,14));
            break;
        case 1:
            putWallFromTo(Position(2,5), Position(8,5));
            putWallFromTo(Position(11,5), Position(17,5));

            putWallFromTo(Position(2,14), Position(8,14));
            putWallFromTo(Position(11,14), Position(17,14));
            break;
        case 2:
            putWallFromTo(Position(4,5), Position(4,14));
            putWallFromTo(Position(15,5), Position(15,14));
            putWallFromTo(Position(5,14), Position(14,14));

            putWallFromTo(Position(7,11), Position(7,7));
            putWallFromTo(Position(12,11), Position(12,7));
            putWallFromTo(Position(8,7), Position(11,7));
            break;
        case 3:
            putWallFromTo(Position(3,5), Position(3,13));
            putWallFromTo(Position(16,5), Position(16,13));
            putWallFromTo(Position(6,6), Position(13,6));
            putWallFromTo(Position(6,12), Position(13,12));
            break;
        case 4:
            putWallFromTo(Position(6,2),Position(6,6));
            putWallFromTo(Position(5,6),Position(2,6));

            putWallFromTo(Position(13,2),Position(13,6));
            putWallFromTo(Position(14,6),Position(17,6));

            putWallFromTo(Position(6,13),Position(6,17));
            putWallFromTo(Position(5,13),Position(2,13));

            putWallFromTo(Position(13,13),Position(13,17));
            putWallFromTo(Position(14,13),Position(17,13));
            break;
        case 5:
            putWallFromTo(Position(2,2), Position(3,2));
            putWallFromTo(Position(2,3), Position(3,3));
            putWallFromTo(Position(4,4), Position(5,4));
            putWallFromTo(Position(4,5), Position(5,5));

            putWallFromTo(Position(16,2), Position(17,2));
            putWallFromTo(Position(16,3), Position(17,3));
            putWallFromTo(Position(14,4), Position(15,4));
            putWallFromTo(Position(14,5), Position(15,5));

            putWallFromTo(Position(2,16), Position(3,16));
            putWallFromTo(Position(2,17), Position(3,17));
            putWallFromTo(Position(4,14), Position(5,14));
            putWallFromTo(Position(4,15), Position(5,15));

            putWallFromTo(Position(16,16), Position(17,16));
            putWallFromTo(Position(16,17), Position(17,17));
            putWallFromTo(Position(14,14), Position(15,14));
            putWallFromTo(Position(14,15), Position(15,15));
            break;
        case 6:
            putWallFromTo(Position(0,0), Position(0,19));
            putWallFromTo(Position(0,0), Position(19,0));
            putWallFromTo(Position(19,0), Position(19,19));
            putWallFromTo(Position(0,19), Position(19,19));

            putWallFromTo(Position(5,5), Position(5,14));
            putWallFromTo(Position(14,5), Position(14,14));
            putWallFromTo(Position(5,5), Position(8,5));
            putWallFromTo(Position(11,5), Position(14,5));
            putWallFromTo(Position(5,14), Position(8,14));
            putWallFromTo(Position(11,14), Position(14,14));

            putWallFromTo(Position(9,9), Position(10,9));
            putWallFromTo(Position(9,10), Position(10,10));
            break;
        case 7:
            walls->push_back(std::make_unique<Wall>(Position(14,16)));
            walls->push_back(std::make_unique<Wall>(Position(15,15)));
            putWallFromTo(Position(16,14), Position(16,13));
            putWallFromTo(Position(17,12), Position(17,7));
            putWallFromTo(Position(16,6), Position(16,5));
            walls->push_back(std::make_unique<Wall>(Position(15,4)));
            putWallFromTo(Position(14,3), Position(13,3));
            putWallFromTo(Position(12,2), Position(7,2));
            putWallFromTo(Position(6,3), Position(4,5));
            putWallFromTo(Position(3,6), Position(3,10));
            putWallFromTo(Position(4,11), Position(6,13));
            putWallFromTo(Position(7,14), Position(10,14));
            putWallFromTo(Position(11,13), Position(12,12));
            putWallFromTo(Position(13,11), Position(13,10));
            putWallFromTo(Position(12,9), Position(11,8));
            putWallFromTo(Position(10,7), Position(9,7));
            putWallFromTo(Position(8,8), Position(7,9));
            break;
        case 8:
            fillWallFromTo(Position(3,3),Position(5,9));
            fillWallFromTo(Position(14,3),Position(16,9));

            fillWallFromTo(Position(6,5),Position(7,7));
            fillWallFromTo(Position(12,5),Position(13,7));

            fillWallFromTo(Position(3,13),Position(5,15));
            fillWallFromTo(Position(14,13),Position(16,15));
            break;
        case 9:
            //C
            walls->push_back(std::make_unique<Wall>(Position(7,3)));
            putWallFromTo(Position(6,2), Position(4,2));
            putWallFromTo(Position(3,3), Position(3,5));
            putWallFromTo(Position(4,6), Position(6,6));
            walls->push_back(std::make_unique<Wall>(Position(7,5)));
            //I
            putWallFromTo(Position(9,2), Position(11,2));
            putWallFromTo(Position(9,6), Position(11,6));
            putWallFromTo(Position(10,3), Position(10,5));
            //P
            putWallFromTo(Position(13,2), Position(13,6));
            putWallFromTo(Position(14,2), Position(15,2));
            putWallFromTo(Position(14,4), Position(15,4));
            walls->push_back(std::make_unique<Wall>(Position(16,3)));
            //H
            putWallFromTo(Position(3,10), Position(3,14));
            putWallFromTo(Position(6,10), Position(6,14));
            putWallFromTo(Position(4,12), Position(5,12));
            //E
            putWallFromTo(Position(8,10), Position(8,14));
            putWallFromTo(Position(9,10), Position(11,10));
            putWallFromTo(Position(9,12), Position(10,12));
            putWallFromTo(Position(9,14), Position(11,14));
            //R
            putWallFromTo(Position(13,10), Position(13,14));
            putWallFromTo(Position(14,10), Position(15,10));
            putWallFromTo(Position(14,12), Position(15,12));
            walls->push_back(std::make_unique<Wall>(Position(16,11)));
            putWallFromTo(Position(15,13), Position(16,14));
            break;
        default:   
            break;
    }
}

int Game::selectMap() {
    std::vector<double> weights;

    for (int i = 0; i < 10; i++) {
        double weight = level - mapSelectionHistory[i];
        weights.push_back(weight);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> dist(weights.begin(), weights.end());

    int selectedIndex = dist(gen);
    mapSelectionHistory[selectedIndex]+=1;
    return selectedIndex;
}


void Game::displayTimer(sf::RenderWindow& window) {
    double secondsLeft = 3.0f - startTime;
    int thickness = 5;

    for (int t = 0; t < thickness; ++t) {
        sf::VertexArray timerArc(sf::LinesStrip, 50);
        for (int i = 0; i < timerArc.getVertexCount(); ++i) {
            float angle = i * 2 * M_PI / (timerArc.getVertexCount() - 1) - M_PI / 2;
            float x = cos(angle) * (50 - t);
            float y = sin(angle) * (50 - t);
            timerArc[i].position = sf::Vector2f(window.getSize().x / 2 + x, window.getSize().y / 2 + y);
            timerArc[i].color = sf::Color::Red;
            if (i >= timerArc.getVertexCount() * secondsLeft / 3.0) {
                timerArc[i].color = sf::Color::Transparent;
            }
        }

        sf::Text timerText;
        timerText.setFont(font);
        timerText.setString(std::to_string(static_cast<int>(secondsLeft)+1));
        timerText.setCharacterSize(50);
        timerText.setFillColor(sf::Color::Red);
        timerText.setPosition(window.getSize().x / 2 - 15, window.getSize().y / 2 - 32);

        window.draw(timerArc);
        window.draw(timerText);
    }
}

void Game::putWallFromTo(Position from, Position to){
    Position direction = to - from;
    if (direction.x != 0) {
        direction.x /= std::abs(direction.x);
    }
    if (direction.y != 0) {
        direction.y /= std::abs(direction.y);
    }
    Position current = from;
    for (Position current = from; current != to; current += direction){
        walls->push_back(std::make_unique<Wall>(current));
    }

    walls->push_back(std::make_unique<Wall>(to));
}

void Game::fillWallFromTo(Position from, Position to){
    int directionX = to.x - from.x;
    if(directionX != 0){
        directionX /= std::abs(directionX);
    }

    Position current = from;
    for (Position current = from; current.x != to.x+directionX; current.x += directionX){
        putWallFromTo(current, Position(current.x, to.y));
    }
}