#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bullet.h"
#include "Enemy.h"

Enemy::Enemy(float x, float y, float speed, float angle, float shootTimer, int hp, int size, bool movable) : x(x), y(y), speed(speed), angle(angle), shootTimer(shootTimer), hp(hp), size(size), movable(movable){}

/// @brief If a bullet touches the enemy, the enemy receives damage.
/// @param bullets Array of all the ally bullets.
/// @return If the enemy dies, return true.
bool Enemy::receiveDamageIfShot(std::vector<Bullet*>& bullets) {
    int diffX, diffY;
    float hitBoxBoth;
    for(auto bullet = bullets.begin(); bullet != bullets.end();){
        diffX = x - (*bullet)->getX();
        diffY = y - (*bullet)->getY();
        hitBoxBoth = (*bullet)->getHitBoxRadius() + size;
        // If the border of the bullet touches the enemy
        // sqrt(x² + y²) < n <=> x² + y² < n² :
        if (diffX * diffX + diffY * diffY < hitBoxBoth * hitBoxBoth) {
            receiveDamage(1);
            delete *bullet;
            bullet = bullets.erase(bullet);
        }
        else {
            bullet++;
        }
    }
    return hp <= 0;
}

/// @brief Adjust the position of the enemy if it's in a wall, in another enemy or out of bounds.
/// @param enemies Array of all the enemies.
/// @param walls Array of all the walls.
void Enemy::adjustPositionBasedOnCollisions(std::vector<Enemy*>& enemies, std::vector<Wall> walls){
    //If in enemy, collide with it
    float enemyX, enemyY, angleEnemyEnemy, diffX, diffY, distance, moveDistance;
    int enemySize;
    for(Enemy* enemy : enemies){
        if(enemy == this) continue; //Don't check collision with itself
        enemyX = enemy->getX();
        enemyY = enemy->getY();
        enemySize = enemy->getSize();
        diffX = x - enemyX;
        diffY = y - enemyY;
        //If the object is inside of the enemy
        if (diffX * diffX + diffY * diffY < (enemySize+size) * (enemySize+size)){
            angleEnemyEnemy = atan2(enemyY - y, x - enemyX);
            moveDistance = enemySize + size - sqrt(diffX * diffX + diffY * diffY);
            // Move the enemy gradually towards the outside
            if(enemy->isMovable()){
                enemy->setCoordonates(enemyX + cos(M_PI + angleEnemyEnemy) * moveDistance / 4,
                                      enemyY - sin(M_PI + angleEnemyEnemy) * moveDistance / 4);
                x += cos(angleEnemyEnemy) * moveDistance / 4;
                y -= sin(angleEnemyEnemy) * moveDistance / 4;
            }
            else{
                x += cos(angleEnemyEnemy) * moveDistance / 2;
                y -= sin(angleEnemyEnemy) * moveDistance / 2;
            }
        }
    }

    //If in wall, move it in front of wall
    float wallX, wallY, angleEnemyWall;
    for (Wall wall : walls) {
        wallX = wall.getX();
        wallY = wall.getY();
        angleEnemyWall = atan2(-y + wallY, -x + wallX);
        //If the enemy nearest point from the middle of the wall is in the wall
        if (wall.isInWall(x+cos(angleEnemyWall)*size, y+sin(angleEnemyWall)*size)){
            if(angleEnemyWall<M_PI/4 && angleEnemyWall>-M_PI/4){
                x -= size - ((wallX - wall.getSize()) - x);
            }
            else if(angleEnemyWall>M_PI*3/4 || angleEnemyWall<-M_PI*3/4){
                x += size - (x - (wallX + wall.getSize()));
            }
            else if(angleEnemyWall>M_PI/4 && angleEnemyWall<M_PI*3/4){
                y -= size - ((wallY - wall.getSize()) - y);
            }
            //angleEnemyWall<-M_PI/4 && angleEnemyWall>-M_PI*3/4
            else{
                y += size - (y - (wallY + wall.getSize()));
            }
        }
    }

    if (x < size) x = size;
    else if (x > 1000-size) x = 1000-size;
    if (y < size) y = size;
    else if (y > 1000-size) y = 1000-size;
}