#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "../Position.h"
#include "Enemy.h"
#include "EnemySeeker.h"
#include "EnemyStats.h"

EnemyStats EnemySeeker::stats;

EnemySeeker::EnemySeeker(Position position, float angleSpawn) : Enemy(position, 3.75, angleSpawn, 0, 5, 4, 15, true) {}

void EnemySeeker::update(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, Player player, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime) {
    move(player.getPosition(), walls, enemies, deltaTime);
}

void EnemySeeker::move(Position target, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime) {
    angle = pathFinding(target, walls, deltaTime);
    smoothTurn(angle, 0.05, deltaTime);

    adjustPositionBasedOnEnemies(enemies);
    adjustPositionBasedOnWalls(walls);
    adjustPositionBasedOnOOB();
}

void EnemySeeker::draw(sf::RenderWindow &window) {
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray enemy(sf::Triangles, 3);
    for(unsigned int i = 0; i < 3; i++) enemy[i].color = enemiesColor;

    enemy[0].position = sf::Vector2f(static_cast<float>(position.x + std::cos(angle) * static_cast<float>(size)),
                                     static_cast<float>(position.y - std::sin(angle) * static_cast<float>(size)));
    enemy[1].position = sf::Vector2f(static_cast<float>(position.x + std::cos(angle + M_PI * 2 / 3) * size),
                                     static_cast<float>(position.y - std::sin(angle + M_PI * 2 / 3) * size));
    enemy[2].position = sf::Vector2f(static_cast<float>(position.x + std::cos(angle - M_PI * 2 / 3) * size),
                                     static_cast<float>(position.y - std::sin(angle - M_PI * 2 / 3) * size));

    window.draw(enemy);
}

void EnemySeeker::drawEffects(sf::RenderWindow &window){
    //Nothing
}