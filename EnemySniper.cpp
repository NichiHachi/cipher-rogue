#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "EnemySniper.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"

EnemySniper::EnemySniper(float x, float y, float sizeFactor, float bulletSizeFactor) {
    this->x = x;
    this->y = y;
    this->speed = 0.4;
    this->angle = M_PI*3/2;
    this->hp = 5;
    this->shootTimer = 0;
    this->size = 19*sizeFactor;
    this->bulletSizeFactor = bulletSizeFactor;
    this->movable = true;
}

void EnemySniper::update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) {
    move(targetAngle, walls, enemies);

    // Shoot every 3.5 secondes
    shootTimer += timePassed;
    if(shootTimer > 3.5){
        shoot(bullets);
        shootTimer = 0;
    }
}

void EnemySniper::shoot(std::vector<Bullet*> &bullets){
    bullets.push_back(new Bullet(x+cos(angle)*15, y+sin(angle)*15, angle, 12, 14, false, false));
}

void EnemySniper::move(float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) {
    float angleDiff = targetAngle - angle;
    if (angleDiff > M_PI) {
        angleDiff -= 2 * M_PI;
    } 
    else if (angleDiff < -M_PI) {
        angleDiff += 2 * M_PI;
    }

    angle += angleDiff * 0.1;

    if (angle > M_PI) {
        angle -= 2 * M_PI;
    } 
    else if (angle < -M_PI) {
        angle += 2 * M_PI;
    }

    x -= cos(angle) * speed;
    y += sin(angle) * speed;
    
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
            angleEnemyEnemy = atan2(- y + enemyY, x - enemyX);
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
        //If the enemy nearest point from the wall is in the wall
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

    //If out of bounds, go back in bounds
    if (x < size) x = size;
    else if (x > 1000-size) x = 1000-size;
    if (y < size) y = size;
    else if (y > 1000-size) y = 1000-size;
}

void EnemySniper::draw(sf::RenderWindow &window) {
    int height = size*50/19;
    int width = size;
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray enemyUp(sf::Triangles, 3);
    sf::VertexArray enemyDown(sf::Triangles, 3);

    enemyUp[0].position = sf::Vector2f(height * cos(angle) + x, 
                                        -height * sin(angle) + y);
    enemyUp[1].position = sf::Vector2f(width * cos(angle + M_PI / 2) + x,
                                        -width * sin(angle + M_PI / 2) + y);
    enemyUp[2].position = sf::Vector2f(width * cos(angle - M_PI / 2) + x,
                                        -width * sin(angle - M_PI / 2) + y);

    for(unsigned int i=0; i<3; i++) enemyUp[i].color = enemiesColor;

    enemyDown[0].position = sf::Vector2f(-height * cos(angle) / 3 + x, 
                                        height * sin(angle) / 3 + y);
    enemyDown[1].position = sf::Vector2f(width * cos(angle + M_PI / 2) + x,
                                        -width * sin(angle + M_PI / 2) + y);
    enemyDown[2].position = sf::Vector2f(width * cos(angle - M_PI / 2) + x,
                                        -width * sin(angle - M_PI / 2) + y);

    for(unsigned int i=0; i<3; i++) enemyDown[i].color = enemiesColor;

    window.draw(enemyUp);
    window.draw(enemyDown);
}

bool EnemySniper::receiveDamageIfShot(std::vector<Bullet*>& bullets) {
    int diffX, diffY;
    float hitBoxBoth;
    for(auto bullet = bullets.begin(); bullet != bullets.end();){
        diffX = x - (*bullet)->getX();
        diffY = y - (*bullet)->getY();
        hitBoxBoth = (*bullet)->getHitBoxRadius() + size;
        // If the border of the bullet touches the enemy
        // sqrt(x² + y²) < n <=> x² + y² < n²:
        if (diffX * diffX + diffY * diffY < hitBoxBoth * hitBoxBoth) {
            hp -= 1;
            delete *bullet;
            bullet = bullets.erase(bullet);
        }
        else {
            bullet++;
        }
    }
    return hp <= 0;
}