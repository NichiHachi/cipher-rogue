#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

#include "Enemy.h"
#include "Bullet.h"
#include "EnemyShooter.h"
#include "Wall.h"

EnemyShooter::EnemyShooter(float x, float y) {
    this->x = x;
    this->y = y;
    this->speed = 1;
    this->angle = M_PI*3/2;
    this->hp = 6;
    this->shootTimer = 0;
}

void EnemyShooter::update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls) {
    move(targetAngle, walls);

    // Shoot every 2 secondes
    shootTimer += timePassed;
    if(shootTimer > 2){
        shoot(bullets);
        shootTimer = 0;
    }
}

void EnemyShooter::shoot(std::vector<Bullet*> &bullets){
    bullets.push_back(new Bullet(x, y, angle, 3, false, true));
}

void EnemyShooter::move(float targetAngle, std::vector<Wall> walls) {
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

    bool xInWall = false;
    bool yInWall = false;
    if (walls.size() != 0) {
        for (Wall wall : walls) {
            if (wall.isInWall(x + cos(angle) * speed, y)) {
                xInWall = true;
            }
            if (wall.isInWall(x, y - sin(angle) * speed)) {
                yInWall = true;
            }
        }
    }
    if (!xInWall) {
        x += cos(angle) * speed;
    }
    if (!yInWall) {
        y -= sin(angle) * speed;
    }
}

void EnemyShooter::draw(sf::RenderWindow &window) {
    int hauteur = 24;
    int rayon = 19;
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray enemy_left_part(sf::Triangles, 3);
    sf::VertexArray enemy_right_part(sf::Triangles, 3);
    float angle_point_triangle_1 = atan2(hauteur, rayon);
    float angle_point_triangle_2 = atan2(hauteur, -rayon);
    float distance_point_triangle = sqrt(hauteur * hauteur + rayon * rayon);

    ////Define the coordonate of the enemy's point that are aline with the mouse
    float enemy_sprite_down_x = x - hauteur * 0.6 * cos(angle);
    float enemy_sprite_down_y = y + hauteur * 0.6 * sin(angle);

    float enemy_sprite_up_x = x + hauteur * cos(angle);
    float enemy_sprite_up_y = y - hauteur * sin(angle);

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
    enemy_left_part[0].color = enemiesColor;
    enemy_left_part[1].color = enemiesColor;
    enemy_left_part[2].color = enemiesColor;

    ////Define the position of right part points
    enemy_right_part[0].position =
        sf::Vector2f(enemy_sprite_right_x, enemy_sprite_right_y);
    enemy_right_part[1].position =
        sf::Vector2f(enemy_sprite_up_x, enemy_sprite_up_y);
    enemy_right_part[2].position =
        sf::Vector2f(enemy_sprite_down_x, enemy_sprite_down_y);

    ////Define the color of the right part
    enemy_right_part[0].color = enemiesColor;
    enemy_right_part[1].color = enemiesColor;
    enemy_right_part[2].color = enemiesColor;

    window.draw(enemy_left_part);
    window.draw(enemy_right_part);
}


bool EnemyShooter::getShot(std::vector<Bullet*>& bullets) {
    int diffX, diffY;
    for(auto bullet = bullets.begin(); bullet != bullets.end();){
        diffX = x - (*bullet)->getX();
        diffY = y - (*bullet)->getY();
        // If the border of the bullet touches the enemy
        // sqrt(x² + y²) < 34 :
        if (diffX * diffX + diffY * diffY < 34*34) {
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