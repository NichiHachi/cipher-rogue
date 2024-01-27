#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Position.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Bullet.h"
#include "Wall.h"
#include "EnemyTurret.h"
#include "EnemyStats.h"

EnemyStats EnemyTurret::stats;

EnemyTurret::EnemyTurret(Position position) : Enemy(position, 5*stats.speedFactor, 0, 0, 3, 10, 30*stats.sizeFactor, false) {}

void EnemyTurret::update(std::vector<std::unique_ptr<Bullet>>& bullets, Player player, std::vector<std::unique_ptr<Wall>> &walls, std::vector<std::unique_ptr<Enemy>>& enemies, float deltaTime) {
    shootTimer += deltaTime;
    if(shootTimer >= 0.5){
        shoot(bullets);
        shootTimer = 0;
    }
}

void EnemyTurret::shoot(std::vector<std::unique_ptr<Bullet>> &bullets){
    for(int i=0;i<4;i++){
        bullets.push_back(std::make_unique<Bullet>(position, angle+M_PI*i/2, speedBullet, 15, false, true));
    }
    angle += M_PI/6;
}

void EnemyTurret::move(std::vector<std::unique_ptr<Wall>> &walls){
    //Nothing
}

void EnemyTurret::draw(sf::RenderWindow &window) {
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray enemy_part(sf::Triangles, 3);

    for(unsigned int i = 0; i < 3; i++) enemy_part[i].color = enemiesColor;
    enemy_part[0].position = sf::Vector2f(position.x, position.y);

    enemy_part[2].position = sf::Vector2f(position.x + cos(angle + M_PI / 4) * size, 
                                          position.y + sin(angle + M_PI / 4) * size);

    //side[1].position = side[2].position because we take the last point of the previous triangle as the first point of the next triangle
    //We could do it like this :
    //enemy_part[1].position = sf::Vector2f(position.x + radius * cos((i - 1) * M_PI / 4 - angle), position.y + radius * sin((i - 1) * M_PI / 4 - angle));
    //But it's less efficient

    for (unsigned int i = 2; i < 10; i++) {
        enemy_part[1].position = enemy_part[2].position;

        enemy_part[2].position = sf::Vector2f(position.x + cos(angle + i * M_PI / 4) * size, 
                                              position.y + sin(angle + i * M_PI / 4) * size);

        window.draw(enemy_part);
    }
}

void EnemyTurret::drawEffects(sf::RenderWindow &window){
    //Nothing
}