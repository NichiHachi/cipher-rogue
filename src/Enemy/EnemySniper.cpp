#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "../Position.h"
#include "EnemySniper.h"
#include "Enemy.h"
#include "../Projectile/Bullet.h"
#include "../Projectile/Bullet.h"
#include "../Wall.h"
#include "EnemyStats.h"
#include "../Player/Player.h"

EnemyStats EnemySniper::stats;

EnemySniper::EnemySniper(Position position) : Enemy(position, 0.4*stats.speedFactor, M_PI*3/2, 0, 12*stats.speedBulletFactor, 5, 19*stats.sizeFactor, true) {}

void EnemySniper::update(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, Player player, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies, float deltaTime) {
    float targetAngle = getAngleToFuturPlayerPosition(player);
    move(targetAngle, walls, enemies);

    // Shoot every 3.5 secondes
    shootTimer += deltaTime;
    if(shootTimer > 3.5){
        shoot(bullets);
        shootTimer = 0;
    }
}

void EnemySniper::shoot(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets){
    bullets->push_back(std::make_unique<Bullet>(position + Position(cos(angle),sin(angle))*size, angle, speedBullet, 14, false, false));
}

void EnemySniper::move(float targetAngle, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies) {
    float angleDiff = targetAngle - angle;
    if (angleDiff > M_PI) {
        angleDiff -= 2 * M_PI;
    } 
    else if (angleDiff < -M_PI) {
        angleDiff += 2 * M_PI;
    }

    angle += angleDiff * 0.1 * stats.turnSpeedFactor;

    if (angle > M_PI) {
        angle -= 2 * M_PI;
    } 
    else if (angle < -M_PI) {
        angle += 2 * M_PI;
    }

    position += Position(-cos(angle),sin(angle))*speed;
    
    adjustPositionBasedOnEnemies(enemies);
    adjustPositionBasedOnWalls(walls);
    adjustPositionBasedOnOOB();
}

void EnemySniper::draw(sf::RenderWindow &window) {
    int height = size*50/19;
    int width = size;
    sf::Color enemiesColor(100, 100, 100);

    sf::VertexArray enemy_half_part(sf::Triangles, 3);
    for(unsigned int i=0; i<3; i++) enemy_half_part[i].color = enemiesColor;

    //Init the left and right points of the enemy
    enemy_half_part[0].position = sf::Vector2f(position.x + cos(angle + M_PI / 2) * width,
                                               position.y - sin(angle + M_PI / 2) * width);


    enemy_half_part[1].position = sf::Vector2f(position.x + cos(angle - M_PI / 2) * width,
                                               position.y - sin(angle - M_PI / 2) * width);

    //Draw the top part
    enemy_half_part[2].position = sf::Vector2f(position.x + cos(angle) * height, 
                                               position.y - sin(angle) * height);

    window.draw(enemy_half_part);

    //Draw the bottom part
    enemy_half_part[2].position = sf::Vector2f(position.x - cos(angle) * height / 3, 
                                               position.y + sin(angle) * height / 3);

    window.draw(enemy_half_part);
}

void EnemySniper::drawEffects(sf::RenderWindow &window){
    //Nothing
}