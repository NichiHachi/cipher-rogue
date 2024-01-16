#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

#include "Enemy.h"
#include "Bullet.h"
#include "EnemySpawner.h"
#include "EnemySeeker.h"
#include "Wall.h"
#include "EnemyStats.h"

EnemyStats EnemySpawner::stats;

EnemySpawner::EnemySpawner(float x, float y) : Enemy(x, y, 5*stats.speedFactor, 0, 0, 10, 20*stats.sizeFactor, false){}

void EnemySpawner::update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) {
    shootTimer += timePassed;
    if (shootTimer >= 5) {
        for (int i = 0; i < 3; i++) {
            enemies.push_back(new EnemySeeker(x, y, M_PI*2*i/3));
        }
        shootTimer = 0;
    }
}

void EnemySpawner::draw(sf::RenderWindow &window) {
    float height = size;
    float width = size*7/20;
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray quad(sf::Quads, 4);

    for(unsigned int i = 0; i < 4; i++) quad[i].color = enemiesColor;

    quad[0].position = sf::Vector2f(x + height, y + width);
    quad[1].position = sf::Vector2f(x - height, y + width);
    quad[2].position = sf::Vector2f(x - height, y - width);
    quad[3].position = sf::Vector2f(x + height, y - width);

    window.draw(quad);

    quad[0].position = sf::Vector2f(x + width, y + height);
    quad[1].position = sf::Vector2f(x - width, y + height);
    quad[2].position = sf::Vector2f(x - width, y - height);
    quad[3].position = sf::Vector2f(x + width, y - height);

    window.draw(quad);
}