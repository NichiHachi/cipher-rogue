#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bullet.h"
#include "EnemyCharger.h"
#include "Wall.h"
#include "EnemyStats.h"

EnemyStats EnemyCharger::stats;

EnemyCharger::EnemyCharger(float x, float y) : Enemy(x, y, 13*stats.speedFactor, M_PI*3/2, 0, 20, 30*stats.sizeFactor, false) {}

void EnemyCharger::update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) {
    shootTimer += timePassed;
    if (shootTimer > 10) {
        move(walls);
    }
    else {
        angle = targetAngle;
    }
}

void EnemyCharger::move(std::vector<Wall> walls) {
    x += cos(angle) * speed;
    y -= sin(angle) * speed;

    //If in wall, move it in front of wall
    float wallX, wallY, angleEnemyWall;
    for (Wall wall : walls) {
        wallX = wall.getX();
        wallY = wall.getY();
        angleEnemyWall = atan2(-y + wallY, -x + wallX);
        //If the enemy nearest point from the wall is in the wall
        if (wall.isInWall(x+cos(angleEnemyWall)*size, y+sin(angleEnemyWall)*size)){
            if(angleEnemyWall<M_PI/4 && angleEnemyWall>-M_PI/4){
                x -= size - ((wallX - wall.getSize()) - x);
            }
            else if(angleEnemyWall>M_PI*3/4 || angleEnemyWall<-M_PI*3/4){
                x += size - (x - (wallX + wall.getSize()));
            }
            else if(angleEnemyWall>M_PI/4 && angleEnemyWall<M_PI*3/4){
                y -= size - ((wallY - wall.getSize()) - y);
            }
            //angleEnemyWall<-M_PI/4 && angleEnemyWall>-M_PI*3/4
            else{
                y += size - (y - (wallY + wall.getSize()));
            }
            shootTimer = 0;
        }
    }

    //If out of bounds, go back in bounds
    if (x < size){
        x = size;
        shootTimer = 0;
    }
    else if (x > 1000-size){
        x = 1000-size;
        shootTimer = 0;
    }
    if (y < size){
        y = size;
        shootTimer = 0;
    }
    else if (y > 1000-size){
        y = 1000-size;
        shootTimer = 0;
    }
}

void EnemyCharger::drawWarningZone(sf::RenderWindow &window) {
        float length = 1500 * (shootTimer - 7) / 3;
        sf::VertexArray warningZone(sf::Quads, 4);

        for(unsigned int i = 0; i < 4; i++) warningZone[i].color = sf::Color::Red;

        warningZone[0].position = sf::Vector2f(x + size * cos(angle + 2 * M_PI / 5),
                                    y - size * sin(angle + 2 * M_PI / 5));
        warningZone[1].position = sf::Vector2f(length * std::cos(angle) + x + size * cos(angle +  2 * M_PI / 5),
                                    -length * sin(angle) + y - size * sin(angle + 2 * M_PI / 5));
        warningZone[2].position = sf::Vector2f(length * std::cos(angle) + x + size * cos(angle +  8 * M_PI / 5),
                                    -length * std::sin(angle) + y - size * sin(angle + 8 * M_PI / 5));
        warningZone[3].position = sf::Vector2f(x + size * cos(angle +  8 * M_PI / 5),
                                    y - size * sin(angle + 8 * M_PI / 5));

        window.draw(warningZone);
}

void EnemyCharger::draw(sf::RenderWindow &window) {
    //Draw the warning zone
    if(shootTimer > 7){
        drawWarningZone(window);
    }

    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray side(sf::Triangles, 3);
    for(unsigned int i = 0; i < 3; i++) side[i].color = enemiesColor;

    side[0].position = sf::Vector2f(x, y);
    //rayon = size
    for (unsigned int i = 0; i < 5; i++) {
        side[1].position = sf::Vector2f(x + size * cos(angle + i * 2 * M_PI / 5),
                                        y + -size * sin(angle + i * 2 * M_PI / 5));
        side[2].position =sf::Vector2f(x + size * cos(angle + (i + 1) * 2 * M_PI / 5),
                                      y - size * sin(angle + (i + 1) * 2 * M_PI / 5));
        window.draw(side);
    }
}