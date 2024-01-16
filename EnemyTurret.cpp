#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"
#include "EnemyTurret.h"
#include "EnemyStats.h"

EnemyStats EnemyTurret::stats;

EnemyTurret::EnemyTurret(float x, float y) : Enemy(x, y, 5*stats.speedFactor, 0, 0, 10, 30*stats.sizeFactor, false) {}

void EnemyTurret::update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) {
    shootTimer += timePassed;
    if(shootTimer >= 0.5){
        shoot(bullets);
        shootTimer = 0;
    }
}

void EnemyTurret::shoot(std::vector<Bullet*> &bullets){
    for(int i=0;i<4;i++){
        bullets.push_back(new Bullet(x, y, angle+M_PI*i/2, 3, 15, false, true));
    }
    angle += M_PI/6;
}

void EnemyTurret::move(std::vector<Wall> walls){
    // Do nothing
}

void EnemyTurret::draw(sf::RenderWindow &window) {
    float radius = size;
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray enemy_part(sf::Triangles, 3);

    for(unsigned int i = 0; i < 3; i++) enemy_part[i].color = enemiesColor;
    enemy_part[0].position = sf::Vector2f(x, y);

    for (unsigned int i = 0; i < 8; i++) {
        enemy_part[1].position = sf::Vector2f(x + radius * cos((i + 1) * M_PI / 4 - angle), y + radius * sin((i + 1) * M_PI / 4 - angle));
        enemy_part[2].position = sf::Vector2f(x + radius * cos((i + 2) * M_PI / 4 - angle), y + radius * sin((i + 2) * M_PI / 4 - angle));
        window.draw(enemy_part);
    }
}