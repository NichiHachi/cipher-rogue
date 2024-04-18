#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Enemy.h"
#include "EnemyBomber.h"
#include "EnemyStats.h"

EnemyStats EnemyBomber::stats;

EnemyBomber::EnemyBomber(Position position) : Enemy(position, 1*stats.speedFactor, static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2 * M_PI, std::rand()%4, 7*stats.bulletSpeedFactor, 6*stats.healthFactor, 19*stats.sizeFactor, true) {}

void EnemyBomber::update(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, Player player, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells) {
    move(walls, enemies, deltaTime);

    // Shoot every 3 secondes
    shootTimer += deltaTime;
    if(shootTimer > 3){
        shoot(player.getPosition(), bombshells);
        shootTimer = 0;
    }
}

void EnemyBomber::shoot(const Position target, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells){
    bombshells->push_back(std::make_unique<Bombshell>(position, target, speedBullet, 10, false));
}

void EnemyBomber::move(const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime) {
    position += Position(std::cos(angle),-std::sin(angle)) * speed * deltaTime * 60;
    
    adjustPositionBasedOnEnemies(enemies);
    if(adjustPositionBasedOnWalls(walls) || adjustPositionBasedOnOOB()){
        angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2 * M_PI;
    }
}

void EnemyBomber::draw(sf::RenderWindow &window) {
    sf::CircleShape circle(size - 4);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineThickness(8);
    circle.setOutlineColor(sf::Color::Red);

    circle.setPosition(position.x - circle.getRadius(), position.y - circle.getRadius());

    window.draw(circle);
}

void EnemyBomber::drawEffects(sf::RenderWindow &window){
    //Nothing
}