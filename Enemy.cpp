#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Position.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(Position position, float speed, float angle, float shootTimer, float speedBullet, int hp, int size, bool movable) : position(position), speed(speed), angle(angle), shootTimer(shootTimer), speedBullet(speedBullet), hp(hp), size(size), movable(movable){}

/// @brief If a bullet touches the enemy, the enemy receives damage and the bullet destroyed.
/// @param bullets Array of all the ally bullets.
void Enemy::receiveDamageIfShot(std::vector<Bullet*>& bullets) {
    Position diffPos;
    float hitBoxBoth;
    for(auto bullet = bullets.begin(); bullet != bullets.end();){
        diffPos = position - (*bullet)->getPosition();
        hitBoxBoth = (*bullet)->getSize() + size;
        // sqrt(x² + y²) < n <=> x² + y² < n² :
        if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth * hitBoxBoth) {
            receiveDamage(1);
            delete *bullet;
            bullet = bullets.erase(bullet);
        }
        else {
            bullet++;
        }
    }
}

/// @brief If the enemy is inside a wall, it is pushed out of it.
/// @param walls Array of all the walls.
/// @return True if the position of the enemy has been adjusted, false otherwise.
bool Enemy::adjustPositionBasedOnWalls(std::vector<Wall> walls){
    bool positionAdjusted = false;
    Position wallPos;
    float angleEnemyWall;
    for (Wall wall : walls) {
        wallPos = wall.getPosition();
        angleEnemyWall = atan2(wallPos.y - position.y, position.x - wallPos.x);
        //If the enemy nearest point from the middle of the wall is in the wall
        if (wall.isInWall(position + Position(-cos(angleEnemyWall),sin(angleEnemyWall))*size)){
            if(-M_PI/4 <= angleEnemyWall && angleEnemyWall <= M_PI/4){
                position.x += size - (position.x - (wallPos.x + wall.getSize()));
            }
            else if(angleEnemyWall >= M_PI*3/4 || angleEnemyWall <= -M_PI*3/4){
                position.x -= size - ((wallPos.x - wall.getSize()) - position.x);
            } 
            else if(M_PI/4 <= angleEnemyWall && angleEnemyWall <= M_PI*3/4){
                position.y -= size - ((wallPos.y - wall.getSize()) - position.y);
            } 
            //angleEnemyWall< -M_PI/4 && angleEnemyWall > -M_PI*3/4
            else{
                position.y += size - (position.y - (wallPos.y + wall.getSize()));
            }
            positionAdjusted = true;
        }
    }
    return positionAdjusted;
}


/// @brief If the enemy is inside an other enemy, push both of them.
/// @param enemies Array of all the enemies.
/// @return True if the position of the enemy has been adjusted, false otherwise.
bool Enemy::adjustPositionBasedOnEnemies(std::vector<Enemy *> &enemies){
    bool positionAdjusted = false;
    float angleEnemyEnemy, distance, moveDistance;
    Position enemyPos, diffPos;
    int enemySize;
    for(Enemy* enemy : enemies){
        if(enemy == this) continue; //Don't check collision with itself
        enemyPos = enemy->getPosition();
        enemySize = enemy->getSize();
        diffPos = position - enemyPos;
        //If the object is inside of the enemy
        if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < (enemySize+size) * (enemySize+size)){
            angleEnemyEnemy = atan2(enemyPos.y - position.y, position.x - enemyPos.x);
            moveDistance = enemySize + size - sqrt(diffPos.x * diffPos.x + diffPos.y * diffPos.y);
            // Move the enemy gradually towards the outside
            if(enemy->isMovable()){
                enemy->setPosition(enemyPos + Position(cos(M_PI + angleEnemyEnemy) * moveDistance / 4,
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

/// @brief If the enemy is out of bounds, push it back in bounds.
/// @return True if the position of the enemy has been adjusted, false otherwise.
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