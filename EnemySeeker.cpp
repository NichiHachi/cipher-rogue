#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

#include "Enemy.h"
#include "Bullet.h"
#include "EnemySeeker.h"
#include "Wall.h"
#include "EnemyStats.h"

EnemyStats EnemySeeker::stats;

EnemySeeker::EnemySeeker(float x, float y, float angleSpawn) : Enemy(x, y, 3.75*stats.speedFactor, angleSpawn, 0, 4, 15*stats.sizeFactor, true) {}

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

    adjustPositionBasedOnCollisions(enemies, walls);
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
