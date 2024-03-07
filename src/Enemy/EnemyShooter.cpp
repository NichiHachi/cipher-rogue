#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

#include "Enemy.h"
#include "EnemyShooter.h"
#include "EnemyStats.h"
#include "../Position.h"
#include "../Wall.h"
#include "../Projectile/Bullet.h"
#include "../Player/Player.h"

EnemyStats EnemyShooter::stats;

EnemyShooter::EnemyShooter(Position position) : Enemy(position, 1*stats.speedFactor, M_PI*3/2, 0, 3*stats.speedBulletFactor, 6, 19*stats.sizeFactor, true) {}

void EnemyShooter::update(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, Player player, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies, float deltaTime) {
    float targetAngle = getAngleToObject(player.getPosition());
    move(targetAngle, walls, enemies, deltaTime);

    // Shoot every 2 secondes
    shootTimer += deltaTime;
    if(shootTimer > 2){
        shoot(bullets);
        shootTimer = 0;
    }
}

void EnemyShooter::shoot(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets){
    bullets->push_back(std::make_unique<Bullet>(position, angle, speedBullet, 15, false, true));
}

void EnemyShooter::move(float targetAngle, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies, float deltaTime) {
    smoothTurn(targetAngle, 0.2, deltaTime);

    position += Position(cos(angle),-sin(angle)) * speed * deltaTime * 60;
    
    adjustPositionBasedOnEnemies(enemies);
    adjustPositionBasedOnWalls(walls);
    adjustPositionBasedOnOOB();
}

void EnemyShooter::draw(sf::RenderWindow &window) {
    int height = size*24/19;
    int width = size;
    float angle_point_triangle_1 = atan2(height, width);
    float angle_point_triangle_2 = atan2(height, -width);
    float distance_point_triangle = sqrt(height * height + width * width);
    sf::Color enemiesColor(100, 100, 100);

    sf::VertexArray shooter_half_part(sf::Triangles, 3);

    //Define the color of the shooter
    for(unsigned int i = 0; i < 3; i++) shooter_half_part[i].color = enemiesColor;

    //Define the coordonate of the shooter's point that are aline with the mouse
    Position shooter_down = position + Position(-cos(angle),sin(angle)) * (float)(height*0.6);
    Position shooter_up = position + Position(cos(angle),-sin(angle)) * height;

    float angle_triangle_left = angle_point_triangle_1 + angle + M_PI / 2;
    Position shooter_left = position + Position(cos(angle_triangle_left),-sin(angle_triangle_left)) * distance_point_triangle;

    float angle_triangle_right = angle_point_triangle_2 + angle + M_PI / 2;
    Position shooter_right = position + Position(cos(angle_triangle_right),-sin(angle_triangle_right)) * distance_point_triangle;

    shooter_half_part[1].position = sf::Vector2f(shooter_up.x, shooter_up.y);
    shooter_half_part[2].position = sf::Vector2f(shooter_down.x, shooter_down.y);

    //Define the position of the left part points
    shooter_half_part[0].position = sf::Vector2f(shooter_left.x, shooter_left.y);

    //Draw the left part
    window.draw(shooter_half_part);

    //Define the position of right part points
    shooter_half_part[0].position = sf::Vector2f(shooter_right.x, shooter_right.y);

    //Draw the right part
    window.draw(shooter_half_part);
}

void EnemyShooter::drawEffects(sf::RenderWindow &window){
    //Nothing
}