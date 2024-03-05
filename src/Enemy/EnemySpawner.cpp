#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

#include "../Position.h"
#include "Enemy.h"
#include "../Projectile/Bullet.h"
#include "EnemySpawner.h"
#include "EnemySeeker.h"
#include "../Wall.h"
#include "EnemyStats.h"
#include "../Player/Player.h"

EnemyStats EnemySpawner::stats;

EnemySpawner::EnemySpawner(Position position) : Enemy(position, 5*stats.speedFactor, 0, 0, 0, 10, 20*stats.sizeFactor, false){}

void EnemySpawner::update(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, Player player, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies, float deltaTime) {
    shootTimer += deltaTime;
    if (shootTimer >= 5) {
        enemies->push_back(std::make_unique<EnemySeeker>(position, M_PI*2/3));
        shootTimer = 0;
    }
}

void EnemySpawner::draw(sf::RenderWindow &window) {
    float height = size;
    float width = size*7/20;
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray quad(sf::Quads, 4);

    for(unsigned int i = 0; i < 4; i++) quad[i].color = enemiesColor;

    quad[0].position = sf::Vector2f(position.x + height, position.y + width);
    quad[1].position = sf::Vector2f(position.x - height, position.y + width);
    quad[2].position = sf::Vector2f(position.x - height, position.y - width);
    quad[3].position = sf::Vector2f(position.x + height, position.y - width);

    window.draw(quad);

    quad[0].position = sf::Vector2f(position.x + width, position.y + height);
    quad[1].position = sf::Vector2f(position.x - width, position.y + height);
    quad[2].position = sf::Vector2f(position.x - width, position.y - height);
    quad[3].position = sf::Vector2f(position.x + width, position.y - height);

    window.draw(quad);
}

void EnemySpawner::drawEffects(sf::RenderWindow &window){
    //Nothing
}