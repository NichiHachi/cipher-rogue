#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "../Position.h"
#include "EnemySniper.h"
#include "Enemy.h"
#include "EnemyStats.h"

EnemyStats EnemySniper::stats;

EnemySniper::EnemySniper(Position position) : Enemy(position, 0.4, M_PI*3/2, 0, 12, 5, 19, true) {}

void EnemySniper::update(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, Player player, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime) {
    float targetAngle = getAngleToFuturPlayerPosition(player);
    move(targetAngle, walls, enemies, deltaTime);

    // Shoot every 3.5 secondes
    shootTimer += deltaTime;
    if(shootTimer > 3.5){
        shoot(bullets);
        shootTimer = 0;
    }
}

void EnemySniper::shoot(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets){
    bullets->push_back(std::make_unique<Bullet>(position + Position(std::cos(angle),std::sin(angle))*size, angle, speedBullet, 14, false, false));
}

void EnemySniper::move(float targetAngle, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime) {
    smoothTurn(targetAngle, 0.1, deltaTime);

    position += Position(-std::cos(angle),std::sin(angle)) * speed * deltaTime * 60;
    
    adjustPositionBasedOnEnemies(enemies);
    adjustPositionBasedOnWalls(walls);
    adjustPositionBasedOnOOB();
}

void EnemySniper::draw(sf::RenderWindow &window) {
    drawSprite(window, 1, sf::Color::Red);
    drawSprite(window, 0.5, sf::Color::Black);
}

void EnemySniper::drawSprite(sf::RenderWindow &window, float sizeFactor, sf::Color color){
    int height = size * 50/19 * sizeFactor;
    int width = size * sizeFactor;

    sf::VertexArray enemy_half_part(sf::Triangles, 3);
    for(unsigned int i=0; i<3; i++) enemy_half_part[i].color = color;

    //Init the left and right points of the enemy
    enemy_half_part[0].position = sf::Vector2f(position.x + std::cos(angle + M_PI / 2) * width,
                                               position.y - std::sin(angle + M_PI / 2) * width);


    enemy_half_part[1].position = sf::Vector2f(position.x + std::cos(angle - M_PI / 2) * width,
                                               position.y - std::sin(angle - M_PI / 2) * width);

    //Draw the top part
    enemy_half_part[2].position = sf::Vector2f(position.x + std::cos(angle) * height,
                                               position.y - std::sin(angle) * height);

    window.draw(enemy_half_part);

    //Draw the bottom part
    enemy_half_part[2].position = sf::Vector2f(position.x - std::cos(angle) * height / 3,
                                               position.y + std::sin(angle) * height / 3);

    window.draw(enemy_half_part);
}

void EnemySniper::drawEffects(sf::RenderWindow &window){
    //Nothing
}