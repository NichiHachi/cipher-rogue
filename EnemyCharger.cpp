#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Position.h"
#include "Bullet.h"
#include "EnemyCharger.h"
#include "Wall.h"
#include "EnemyStats.h"
#include "Player.h"

EnemyStats EnemyCharger::stats;

EnemyCharger::EnemyCharger(Position position) : Enemy(position, 13*stats.speedFactor, M_PI*3/2, 0, 5*stats.speedBulletFactor, 20, 30*stats.sizeFactor, false) {}

void EnemyCharger::update(std::vector<std::unique_ptr<Bullet>>& bullets, Player player, std::vector<std::unique_ptr<Wall>> &walls, std::vector<std::unique_ptr<Enemy>>& enemies, float deltaTime) {
    shootTimer += deltaTime;
    if (shootTimer > 10) 
        move(walls);
    else 
        angle = getAngleToObject(player.getPosition());
}

void EnemyCharger::move(std::vector<std::unique_ptr<Wall>> &walls) {
    position += Position(cos(angle), -sin(angle))*speed;

    //If the enemy touch a wall or the screen border, it stops
    if(adjustPositionBasedOnWalls(walls)) shootTimer = 0;
    if(adjustPositionBasedOnOOB()) shootTimer = 0;
}

void EnemyCharger::drawWarningZone(sf::RenderWindow &window) {
        float length = 500 * (shootTimer - 7);
        sf::VertexArray warningZone(sf::Quads, 4);

        for(unsigned int i = 0; i < 4; i++) warningZone[i].color = sf::Color(255, 0, 0, 100);

        sf::Vector2f lengthWarningZone = sf::Vector2f(cos(angle), -sin(angle))*length;
        sf::Vector2f totalLengthWarningZone = sf::Vector2f(cos(angle), -sin(angle))*(float)1500;

        warningZone[0].position = sf::Vector2f(position.x + cos(angle + 2 * M_PI / 5)*size,
                                               position.y - sin(angle + 2 * M_PI / 5)*size);

        warningZone[1].position = sf::Vector2f(position.x + cos(angle + 8 * M_PI / 5)*size,
                                               position.y - sin(angle + 8 * M_PI / 5)*size);
        
        warningZone[2].position = warningZone[1].position + totalLengthWarningZone;
        warningZone[3].position = warningZone[0].position + totalLengthWarningZone;

        window.draw(warningZone);


        //We extand the point that start from the charger to the length of the warning zone
        warningZone[2].position = warningZone[1].position + lengthWarningZone;
        warningZone[3].position = warningZone[0].position + lengthWarningZone;

        window.draw(warningZone);
}

void EnemyCharger::draw(sf::RenderWindow &window) {
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray side(sf::Triangles, 3);
    for(unsigned int i = 0; i < 3; i++) side[i].color = enemiesColor;

    sf::Vector2f chargerCenter = sf::Vector2f(position.x,position.y);

    side[0].position = chargerCenter;
    side[2].position = chargerCenter + sf::Vector2f(cos(angle), -sin(angle))*(float)size;

    //side[1].position = side[2].position because we take the last point of the previous triangle as the first point of the next triangle
    //We could do it like this :
    //side[1].position = chargerCenter + sf::Vector2f(cos(angle + (i-1) * 2 * M_PI / 5), -sin(angle + (i-1) + 2 * M_PI / 5))*(float)size;
    //But it's less efficient

    for (unsigned int i = 1; i < 6; i++) {
        side[1].position = side[2].position;

        side[2].position = chargerCenter + sf::Vector2f(cos(angle + i * 2 * M_PI / 5), 
                                                        -sin(angle + i * 2 * M_PI / 5))*(float)size;

        window.draw(side);
    }
}

void EnemyCharger::drawEffects(sf::RenderWindow &window) {
    if(shootTimer > 7){
        drawWarningZone(window);
    }
}