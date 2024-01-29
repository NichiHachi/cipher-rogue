#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Position.h"
#include "Bullet.h"
#include "Bombshell.h"
#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(Position position, float speed, float angle, float shootTimer, float speedBullet, int hp, int size, bool movable) : position(position), speed(speed), angle(angle), shootTimer(shootTimer), speedBullet(speedBullet), hp(hp), size(size), movable(movable){}

void Enemy::receiveDamageIfShot(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>> bombshells){
    Position diffPos;
    float hitBoxBoth;
    auto bullet = bullets->begin();

    while (bullet != bullets->end()) {
        diffPos = position - bullet->get()->getPosition();
        hitBoxBoth = bullet->get()->getSize() + size;
        // sqrt(x² + y²) < n <=> x² + y² < n² :
        if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth * hitBoxBoth) {
            receiveDamage(1);
            bullet = bullets->erase(bullet);
        }
        else {
            bullet++;
        }
    }

    for(unsigned int i = 0; i < bombshells->size(); i++){
        if(!bombshells->at(i)->hasExploded() || !bombshells->at(i)->isAlly()) continue;
        
        if (std::find(bombshells->at(i)->hitEnemies.begin(), bombshells->at(i)->hitEnemies.end(), this) != bombshells->at(i)->hitEnemies.end()) {
            continue;
        }

        diffPos = position - bombshells->at(i)->getPosition();
        hitBoxBoth = size + bombshells->at(i)->getSize();
        if(diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth * hitBoxBoth){
            receiveDamage(2);
            bombshells->at(i)->hitEnemies.push_back(this);
        }
    }
}

bool Enemy::adjustPositionBasedOnWalls(std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls){
    bool positionAdjusted = false;
    Position wallPos;
    float angleEnemyWall, wallSize;
    for (unsigned int i = 0; i < walls->size(); i++) {
        wallPos = walls->at(i)->getPosition();
        angleEnemyWall = atan2(wallPos.y - position.y, position.x - wallPos.x);
        //If the enemy nearest point from the middle of the wall is in the wall
        if (walls->at(i)->isInWall(position + Position(-cos(angleEnemyWall),sin(angleEnemyWall))*size)){
            wallSize = walls->at(i)->getSize();
            if(-M_PI/4 <= angleEnemyWall && angleEnemyWall <= M_PI/4){
                position.x += size - (position.x - (wallPos.x + wallSize));
            }
            else if(angleEnemyWall >= M_PI*3/4 || angleEnemyWall <= -M_PI*3/4){
                position.x -= size - ((wallPos.x - wallSize) - position.x);
            } 
            else if(M_PI/4 <= angleEnemyWall && angleEnemyWall <= M_PI*3/4){
                position.y -= size - ((wallPos.y - wallSize) - position.y);
            } 
            //angleEnemyWall< -M_PI/4 && angleEnemyWall > -M_PI*3/4
            else{
                position.y += size - (position.y - (wallPos.y + wallSize));
            }
            positionAdjusted = true;
        }
    }
    return positionAdjusted;
}

bool Enemy::adjustPositionBasedOnEnemies(std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies){
    bool positionAdjusted = false;
    float angleEnemyEnemy, distance, moveDistance;
    Position enemyPos, diffPos;
    int enemySize;
    for (unsigned int i = 0; i < enemies->size(); i++) {
        if (enemies->at(i).get() == this) {
            //Don't check collision with itself
            continue;
        }

        enemyPos = enemies->at(i)->getPosition();
        enemySize = enemies->at(i)->getSize();
        diffPos = position - enemyPos;
        
        //If the object is inside of the enemy
        if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < (enemySize+size) * (enemySize+size)){
            angleEnemyEnemy = atan2(enemyPos.y - position.y, position.x - enemyPos.x);
            moveDistance = enemySize + size - sqrt(diffPos.x * diffPos.x + diffPos.y * diffPos.y);
            // Move the enemy gradually towards the outside
            if(enemies->at(i)->isMovable()){
                enemies->at(i)->setPosition(enemyPos + Position(cos(M_PI + angleEnemyEnemy) * moveDistance / 4,
                                                      - sin(M_PI + angleEnemyEnemy) * moveDistance / 4));
                                                      
                position += Position(cos(angleEnemyEnemy) * moveDistance / 4,
                                    - sin(angleEnemyEnemy) * moveDistance / 4);
            }
            else{
                position += Position(cos(angleEnemyEnemy) * moveDistance / 2,
                                    - sin(angleEnemyEnemy) * moveDistance / 2);
            }
            positionAdjusted = true;
        }
    }
    return positionAdjusted;
}

bool Enemy::adjustPositionBasedOnOOB(){
    bool positionAdjusted = false;

    if (position.x < size) {
        position.x = size;
        positionAdjusted = true;
    }
    else if (position.x > 1000 - size) {
        position.x = 1000 - size;
        positionAdjusted = true;
    }

    if (position.y < size) {
        position.y = size;
        positionAdjusted = true;
    }
    else if (position.y > 1000 - size) {
        position.y = 1000 - size;
        positionAdjusted = true;
    }

    return positionAdjusted;
}

float Enemy::getAngleToObject(Position objectPosition){
    return atan2(position.y - objectPosition.y, objectPosition.x - position.x);
}

float Enemy::getAngleToFuturPlayerPosition(Player player){
    Position diffPos = player.getPosition() - position;
    float timeBulletTravel = sqrt(diffPos.x*diffPos.x + diffPos.y*diffPos.y)/speedBullet;

    Position playerNewPos = player.getPosition();

    int xAxisMove = 0;
    int yAxisMove = 0;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) yAxisMove++;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) xAxisMove--;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) yAxisMove--;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) xAxisMove++;
    
    if (yAxisMove != 0 && xAxisMove != 0) {
        playerNewPos += Position(xAxisMove*sqrt(2)/2,-yAxisMove*sqrt(2)/2)*player.getSpeed()*timeBulletTravel;
    }
    else{
        playerNewPos += Position(xAxisMove,-yAxisMove)*player.getSpeed()*timeBulletTravel;
    }

    //Return the angle between the enemy and the new player position
    return getAngleToObject(playerNewPos);
}