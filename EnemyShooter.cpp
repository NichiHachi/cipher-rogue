#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

#include "Enemy.h"
#include "Bullet.h"
#include "EnemyShooter.h"
#include "Wall.h"
#include "EnemyStats.h"

EnemyStats EnemyShooter::stats;

EnemyShooter::EnemyShooter(float x, float y) : Enemy(x, y, 1*stats.speedFactor, M_PI*3/2, 0, 6, 19*stats.sizeFactor, true) {}

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
    
    adjustPositionBasedOnCollisions(enemies, walls);
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