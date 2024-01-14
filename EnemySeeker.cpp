#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

#include "Enemy.h"
#include "Bullet.h"
#include "EnemySeeker.h"
#include "Wall.h"

EnemySeeker::EnemySeeker(float x, float y, float angleSpawn, float sizeFactor){
    this->x = x;
    this->y = y;
    this->angle = angleSpawn;
    this->speed = 3.75;
    this->hp = 4;
    this->size = 15*sizeFactor;
}

void EnemySeeker::update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) {
    move(targetAngle, walls, enemies);
}

void EnemySeeker::move(float targetAngle, std::vector<Wall> walls, std::vector<Enemy*> enemies) {
    float angleDiff = targetAngle - angle;
    if (angleDiff > M_PI) {
        angleDiff -= 2*M_PI;
    } 
    else if (angleDiff < -M_PI) {
        angleDiff += 2*M_PI;
    }

    angle += angleDiff*0.05;
    if (angle > M_PI) {
        angle -= 2*M_PI;
    } 
    else if (angle < -M_PI) {
        angle += 2*M_PI;
    }

    x += cos(angle) * speed;
    y -= sin(angle) * speed;

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

void EnemySeeker::draw(sf::RenderWindow &window) {
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray enemy(sf::Triangles, 3);

    enemy[0].position = sf::Vector2f(x + cos(angle) * size, y - sin(angle) * size);
    enemy[1].position = sf::Vector2f(x + cos(angle + M_PI * 2 / 3) * size,
                                    y - sin(angle + M_PI * 2 / 3) * size);
    enemy[2].position = sf::Vector2f(x + cos(angle - M_PI * 2 / 3) * size,
                                    y - sin(angle - M_PI * 2 / 3) * size);

    for(unsigned int i = 0; i < 3; i++) enemy[i].color = enemiesColor;

    window.draw(enemy);
}

bool EnemySeeker::getShot(std::vector<Bullet*>& bullets) {
    float diffX, diffY, hitBoxBoth;
    for(auto bullet = bullets.begin(); bullet != bullets.end();){
        diffX = x - (*bullet)->getX();
        diffY = y - (*bullet)->getY();
        hitBoxBoth = (*bullet)->getHitBoxRadius() + size;
        // If the border of the bullet touches the enemy
        // sqrt(x² + y²) < n <=> x² + y² < n² :
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