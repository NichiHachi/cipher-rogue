#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Enemy.h"
#include "EnemyShooter.h"
#include "EnemyStats.h"

EnemyStats EnemyShooter::stats;

EnemyShooter::EnemyShooter(Position position) : Enemy(position, 1, M_PI*3/2, 0, 7, 6, 19, true) {}

void EnemyShooter::update(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, Player player, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime) {
    float targetAngle = getAngleToTarget(player.getPosition());
    move(targetAngle, walls, enemies, deltaTime);

    // Shoot every 2 secondes
    shootTimer += deltaTime;
    if(shootTimer > 2){
        shoot(bullets);
        shootTimer = 0;
    }
}

void EnemyShooter::shoot(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets){
    bullets->push_back(std::make_unique<Bullet>(position, angle, speedBullet, 15, false, true));
}

void EnemyShooter::move(float targetAngle, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime) {
    smoothTurn(targetAngle, 0.2, deltaTime);

    position += Position(std::cos(angle),-std::sin(angle)) * speed * deltaTime * 60;
    
    adjustPositionBasedOnEnemies(enemies);
    adjustPositionBasedOnWalls(walls);
    adjustPositionBasedOnOOB();
}

void EnemyShooter::draw(sf::RenderWindow &window) {
    int height = size*24/19;
    int width = size;
    double angle_point_triangle_1 = std::atan2(height, width);
    double angle_point_triangle_2 = std::atan2(height, -width);
    double distance_point_triangle = std::sqrt(height * height + width * width);
    sf::Color enemiesColor(100, 100, 100);

    sf::VertexArray shooter_half_part(sf::Triangles, 3);

    //Define the color of the shooter
    for(unsigned int i = 0; i < 3; i++) shooter_half_part[i].color = enemiesColor;

    //Define the coordonate of the shooter's point that are aline with the mouse
    Position shooter_down = position + Position(-std::cos(angle),std::sin(angle)) * height * 0.6f;
    Position shooter_up = position + Position(std::cos(angle),-std::sin(angle)) * height;

    double angle_triangle_left = angle_point_triangle_1 + angle + M_PI / 2;
    Position shooter_left = position + Position(std::cos(angle_triangle_left),-std::sin(angle_triangle_left)) * distance_point_triangle;

    double angle_triangle_right = angle_point_triangle_2 + angle + M_PI / 2;
    Position shooter_right = position + Position(std::cos(angle_triangle_right),-std::sin(angle_triangle_right)) * distance_point_triangle;

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