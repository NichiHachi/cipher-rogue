#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

#include "Enemy.h"
#include "Bullet.h"
#include "EnemyShooter.h"
#include "Wall.h"

EnemyShooter::EnemyShooter(float x, float y, float sizeFactor, float bulletSizeFactor) {
    this->x = x;
    this->y = y;
    this->speed = 1;
    this->angle = M_PI*3/2;
    this->hp = 6;
    this->shootTimer = 0;
    this->size = 19*sizeFactor;
    this->bulletSizeFactor = bulletSizeFactor;
    this->movable = true;
}

void EnemyShooter::update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) {
    move(targetAngle, walls, enemies);

    // Shoot every 2 secondes
    shootTimer += timePassed;
    if(shootTimer > 2){
        shoot(bullets);
        shootTimer = 0;
    }
}

void EnemyShooter::shoot(std::vector<Bullet*> &bullets){
    bullets.push_back(new Bullet(x, y, angle, 3, 15, false, true));
}

void EnemyShooter::move(float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) {
    float angleDiff = targetAngle - angle;
    if (angleDiff > M_PI) {
        angleDiff -= 2 * M_PI;
    } 
    else if (angleDiff < -M_PI) {
        angleDiff += 2 * M_PI;
    }

    angle += angleDiff * 0.2;

    if (angle > M_PI) {
        angle -= 2 * M_PI;
    } 
    else if (angle < -M_PI) {
        angle += 2 * M_PI;
    }

    x += cos(angle) * speed;
    y -= sin(angle) * speed;

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
}

void EnemyShooter::draw(sf::RenderWindow &window) {
    float height = size*24/19;
    float width = size;
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray enemy_left_part(sf::Triangles, 3);
    sf::VertexArray enemy_right_part(sf::Triangles, 3);
    
    float angle_point_triangle_1 = atan2(height, width);
    float angle_point_triangle_2 = atan2(height, -width);
    float distance_point_triangle = sqrt(height * height + width * width);

    ////Define the coordonate of the enemy's point that are aline with the mouse
    float enemy_sprite_down_x = x - height * 0.6 * cos(angle);
    float enemy_sprite_down_y = y + height * 0.6 * sin(angle);

    float enemy_sprite_up_x = x + height * cos(angle);
    float enemy_sprite_up_y = y - height * sin(angle);

    float angle_triangle_left = angle_point_triangle_1 + angle + M_PI / 2;
    float enemy_sprite_left_x =
        x + distance_point_triangle * cos(angle_triangle_left);
    float enemy_sprite_left_y =
        y - distance_point_triangle * sin(angle_triangle_left);

    float angle_triangle_right = angle_point_triangle_2 + angle + M_PI / 2;
    float enemy_sprite_right_x =
        x + distance_point_triangle * cos(angle_triangle_right);
    float enemy_sprite_right_y =
        y - distance_point_triangle * sin(angle_triangle_right);

    ////Define the position of the left part points
    enemy_left_part[0].position =
        sf::Vector2f(enemy_sprite_left_x, enemy_sprite_left_y);
    enemy_left_part[1].position =
        sf::Vector2f(enemy_sprite_up_x, enemy_sprite_up_y);
    enemy_left_part[2].position =
        sf::Vector2f(enemy_sprite_down_x, enemy_sprite_down_y);

    ////Define the color of the left part
    for(unsigned int i = 0; i < 3; i++) enemy_left_part[i].color = enemiesColor;

    ////Define the position of right part points
    enemy_right_part[0].position =
        sf::Vector2f(enemy_sprite_right_x, enemy_sprite_right_y);
    enemy_right_part[1].position =
        sf::Vector2f(enemy_sprite_up_x, enemy_sprite_up_y);
    enemy_right_part[2].position =
        sf::Vector2f(enemy_sprite_down_x, enemy_sprite_down_y);

    ////Define the color of the right part
    for(unsigned int i = 0; i < 3; i++) enemy_right_part[i].color = enemiesColor;

    window.draw(enemy_left_part);
    window.draw(enemy_right_part);
}


bool EnemyShooter::receiveDamageIfShot(std::vector<Bullet*>& bullets) {
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