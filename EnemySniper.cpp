#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "EnemySniper.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"

EnemySniper::EnemySniper(float x, float y) : Enemy(x, y, M_PI*3/2, 0.4, 5, 0){}

void EnemySniper::update(std::vector<Bullet*> &bullets, float timePassed, float targetAngle, std::vector<Wall> walls) {
    move(targetAngle, walls);

    // Shoot every 3.5 secondes
    shootTimer += timePassed;
    if(shootTimer > 3.5){
        shoot(bullets);
        shootTimer = 0;
    }
}

void EnemySniper::shoot(std::vector<Bullet*> &bullets){
    bullets.push_back(new Bullet(x+cos(angle)*15, y+sin(angle)*15, angle, 12, false, false));
}

void EnemySniper::move(float targetAngle, std::vector<Wall> walls) {
    float angleDiff = targetAngle - angle;
    if (angleDiff > M_PI) {
        angleDiff -= 2 * M_PI;
    } 
    else if (angleDiff < -M_PI) {
        angleDiff += 2 * M_PI;
    }

    angle += angleDiff * 0.1;

    if (angle > M_PI) {
        angle -= 2 * M_PI;
    } 
    else if (angle < -M_PI) {
        angle += 2 * M_PI;
    }

    bool xInWall = false;
    bool yInWall = false;
    if (walls.size() != 0) {
        for (Wall wall : walls) {
        if (wall.isInWall(x - cos(angle) * speed, y)) {
            xInWall = true;
        }
        if (wall.isInWall(x, y + sin(angle) * speed)) {
            yInWall = true;
        }
        }
    }
    if (!xInWall) {
        x -= cos(angle) * speed;
    }
    if (!yInWall) {
        y += sin(angle) * speed;
    }

    if (x < 19) {
        x = 19;
    } 
    else if (x > 981) {
        x = 981;
    }

    if (y < 19) {
        y = 19;
    } 
    else if (y > 981) {
        y = 981;
    }
}

void EnemySniper::draw(sf::RenderWindow &window) {
    int hauteur = 50;
    int rayon = 19;
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray enemyUp(sf::Triangles, 3);
    sf::VertexArray enemyDown(sf::Triangles, 3);

    enemyUp[0].position =
        sf::Vector2f(hauteur * cos(angle) + x, -hauteur * sin(angle) + y);
    enemyUp[1].position = sf::Vector2f(rayon * cos(angle + M_PI / 2) + x,
                                        -rayon * sin(angle + M_PI / 2) + y);
    enemyUp[2].position = sf::Vector2f(rayon * cos(angle - M_PI / 2) + x,
                                        -rayon * sin(angle - M_PI / 2) + y);

    enemyUp[0].color = enemiesColor;
    enemyUp[1].color = enemiesColor;
    enemyUp[2].color = enemiesColor;

    enemyDown[0].position =
        sf::Vector2f(-hauteur * cos(angle) / 3 + x, hauteur * sin(angle) / 3 + y);
    enemyDown[1].position = sf::Vector2f(rayon * cos(angle + M_PI / 2) + x,
                                        -rayon * sin(angle + M_PI / 2) + y);
    enemyDown[2].position = sf::Vector2f(rayon * cos(angle - M_PI / 2) + x,
                                        -rayon * sin(angle - M_PI / 2) + y);

    enemyDown[0].color = enemiesColor;
    enemyDown[1].color = enemiesColor;
    enemyDown[2].color = enemiesColor;

    window.draw(enemyUp);
    window.draw(enemyDown);
}