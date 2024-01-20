#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

#include "Position.h"
#include "Enemy.h"
#include "Bullet.h"
#include "EnemySeeker.h"
#include "Wall.h"
#include "EnemyStats.h"
#include "Player.h"

EnemyStats EnemySeeker::stats;

EnemySeeker::EnemySeeker(Position position, float angleSpawn) : Enemy(position, 3.75*stats.speedFactor, angleSpawn, 0, 5*stats.speedBulletFactor, 4, 15*stats.sizeFactor, true) {}

void EnemySeeker::update(std::vector<Bullet*>& bullets, float timePassed, Player player, std::vector<Wall> walls, std::vector<Enemy*>& enemies) {
    float targetAngle = getAngleToObject(player.getPosition());
    move(targetAngle, walls, enemies);
}

void EnemySeeker::move(float targetAngle, std::vector<Wall> walls, std::vector<Enemy*> enemies) {
    float angleDiff = targetAngle - angle;

    // Make sure the angle is between -PI and PI
    if (angleDiff > M_PI) 
        angleDiff -= 2*M_PI;
    else if (angleDiff < -M_PI) 
        angleDiff += 2*M_PI;

    angle += angleDiff * 0.05 * stats.turnSpeedFactor;

    if (angle > M_PI) 
        angle -= 2*M_PI;
    else if (angle < -M_PI) 
        angle += 2*M_PI;

    position += Position(cos(angle),-sin(angle))*speed;

    adjustPositionBasedOnEnemies(enemies);
    adjustPositionBasedOnWalls(walls);
    adjustPositionBasedOnOOB();
}

void EnemySeeker::draw(sf::RenderWindow &window ) {
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray enemy(sf::Triangles, 3);
    for(unsigned int i = 0; i < 3; i++) enemy[i].color = enemiesColor;

    enemy[0].position = sf::Vector2f(position.x + cos(angle) * size, 
                                     position.y - sin(angle) * size);
    enemy[1].position = sf::Vector2f(position.x + cos(angle + M_PI * 2 / 3) * size,
                                     position.y - sin(angle + M_PI * 2 / 3) * size);
    enemy[2].position = sf::Vector2f(position.x + cos(angle - M_PI * 2 / 3) * size,
                                     position.y - sin(angle - M_PI * 2 / 3) * size);

    window.draw(enemy);
}

void EnemySeeker::drawEffects(sf::RenderWindow &window ){
    //Nothing
}