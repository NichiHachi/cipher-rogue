#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "EnemySniper.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"
#include "EnemyStats.h"

EnemyStats EnemySniper::stats;

EnemySniper::EnemySniper(float x, float y) : Enemy(x, y, 0.4*stats.speedFactor, M_PI*3/2, 0, 5, 19*stats.sizeFactor, true) {}

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
    
    
    adjustPositionBasedOnCollisions(enemies, walls);
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