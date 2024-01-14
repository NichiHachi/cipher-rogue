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
}

void EnemySniper::update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) {
    move(targetAngle, walls);

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

void EnemySniper::move(float targetAngle, std::vector<Wall> walls) {
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

    //If in wall, replace in front of wall
    if (walls.size() != 0) {
        float wallX, wallY, angleEnemyWall;
        for (Wall wall : walls) {
            wallX = wall.getX();
            wallY = wall.getY();
            angleEnemyWall = atan2(-y + wallY, -x + wallX);
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

bool EnemySniper::getShot(std::vector<Bullet*>& bullets) {
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