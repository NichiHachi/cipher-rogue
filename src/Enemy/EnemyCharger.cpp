#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "../Position.h"
#include "../Projectile/Bullet.h"
#include "EnemyCharger.h"
#include "EnemyStats.h"

EnemyStats EnemyCharger::stats;

EnemyCharger::EnemyCharger(Position position) : Enemy(position, 13, M_PI*3/2, 0, 5, 20, 30, false) {}

void EnemyCharger::update(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, Player player,
                          const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime) {
    
    shootTimer += deltaTime;
    if (shootTimer > 10) 
        move(walls, deltaTime);
    else 
        angle = getAngleToTarget(player.getPosition());
}

void EnemyCharger::move(const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, float deltaTime) {
    position += Position(std::cos(angle), -std::sin(angle)) * speed * deltaTime * 60;

    //If the enemy touch the screen border, it stops
    Position wallPos;
    float angleEnemyWall;
    int wallSize;
    for (auto wall = walls->begin(); wall != walls->end();) {
        wallPos = (*wall)->getPosition();
        angleEnemyWall = std::atan2(wallPos.y - position.y, position.x - wallPos.x);
        //If the enemy nearest point from the middle of the wall is in the wall
        if ((*wall)->isInWall(position + Position(-std::cos(angleEnemyWall), std::sin(angleEnemyWall)) * size)) {
            wall = walls->erase(wall);
        }
        else{
            wall++;
        }
    }

    if (adjustPositionBasedOnOOB()) {
        shootTimer = 0;
    }
}

void EnemyCharger::drawWarningZone(sf::RenderWindow &window) {
        float length = 500 * (shootTimer - 7);
        sf::VertexArray warningZone(sf::Quads, 4);

        for(unsigned int i = 0; i < 4; i++) warningZone[i].color = sf::Color(255, 0, 0, 100);

        sf::Vector2f lengthWarningZone = sf::Vector2f(std::cos(angle), -std::sin(angle))*length;
        sf::Vector2f totalLengthWarningZone = sf::Vector2f(std::cos(angle), -std::sin(angle))*(float)1500;

        warningZone[0].position = sf::Vector2f(static_cast<float>(position.x + std::cos(angle + 2 * M_PI / 5)*size),
                                               static_cast<float>(position.y - std::sin(angle + 2 * M_PI / 5)*size));

        warningZone[1].position = sf::Vector2f(static_cast<float>(position.x + std::cos(angle + 8 * M_PI / 5)*size),
                                               static_cast<float>(position.y - std::sin(angle + 8 * M_PI / 5)*size));
        
        warningZone[2].position = warningZone[1].position + totalLengthWarningZone;
        warningZone[3].position = warningZone[0].position + totalLengthWarningZone;

        window.draw(warningZone);


        //We extand the point that start from the charger to the length of the warning zone
        warningZone[2].position = warningZone[1].position + lengthWarningZone;
        warningZone[3].position = warningZone[0].position + lengthWarningZone;

        window.draw(warningZone);
}

void EnemyCharger::draw(sf::RenderWindow &window) {
    drawSprite(window, 1, sf::Color::Magenta);
    drawSprite(window, 0.5, sf::Color::Black);
}

void EnemyCharger::drawSprite(sf::RenderWindow &window, float sizeFactor, sf::Color color){
    sf::VertexArray side(sf::Triangles, 3);
    for(unsigned int i = 0; i < 3; i++) side[i].color = color;

    sf::Vector2f chargerCenter = sf::Vector2f(position.x,position.y);

    side[0].position = chargerCenter;
    side[2].position = chargerCenter + sf::Vector2f(std::cos(angle), -std::sin(angle)) * static_cast<float>(size) * sizeFactor;

    //side[1].position = side[2].position because we take the last point of the previous triangle as the first point of the next triangle
    //We could do it like this :
    //side[1].position = chargerCenter + sf::Vector2f(cos(angle + (i-1) * 2 * M_PI / 5), -sin(angle + (i-1) + 2 * M_PI / 5))*(float)size;
    //But it's less efficient

    for (unsigned int i = 1; i < 6; i++) {
        side[1].position = side[2].position;

        side[2].position = chargerCenter + sf::Vector2f(static_cast<float>(std::cos(angle + i * 2 * M_PI / 5) * static_cast<float>(size) * sizeFactor),
                                                        static_cast<float>(-std::sin(angle + i * 2 * M_PI / 5) * static_cast<float>(size) * sizeFactor));

        window.draw(side);
    }
}

void EnemyCharger::drawEffects(sf::RenderWindow &window) {
    if(shootTimer > 7){
        drawWarningZone(window);
    }
}