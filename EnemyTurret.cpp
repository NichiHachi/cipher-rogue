#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"
#include "EnemyTurret.h"

EnemyTurret::EnemyTurret(float x, float y) : Enemy() {
    this->x = x;
    this->y = y;
    this->speed = 0;
    this->angle = 0;
    this->hp = 10;
    this->shootTimer = 0;
}

void EnemyTurret::update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls) {
    shootTimer += timePassed;
    if(shootTimer >= 0.5){
        shoot(bullets);
        shootTimer = 0;
    }
}

void EnemyTurret::shoot(std::vector<Bullet*> &bullets){
    for(int i=0;i<4;i++){
        bullets.push_back(new Bullet(x,y,angle+M_PI*i/2,3,false, true));
    }
    angle += M_PI/6;
}

void EnemyTurret::move(std::vector<Wall> walls){
    // Do nothing
}

void EnemyTurret::draw(sf::RenderWindow &window) {
    sf::Color enemiesColor(100, 100, 100);
    sf::VertexArray enemy_part(sf::Triangles, 3);
    int radius = 30;
    enemy_part[0].color = enemiesColor;
    enemy_part[1].color = enemiesColor;
    enemy_part[2].color = enemiesColor;
    for (int i = 0; i < 8; i++) {
        enemy_part[0].position = sf::Vector2f(x, y);
        enemy_part[1].position = sf::Vector2f(x + radius * cos((i + 1) * M_PI / 4 - angle), y + radius * sin((i + 1) * M_PI / 4 - angle));
        enemy_part[2].position = sf::Vector2f(x + radius * cos((i + 2) * M_PI / 4 - angle), y + radius * sin((i + 2) * M_PI / 4 - angle));
        window.draw(enemy_part);
    }
}

bool EnemyTurret::getShot(std::vector<Bullet*>& bullets) {
    int diffX, diffY;
    for(auto bullet = bullets.begin(); bullet != bullets.end();){
        diffX = x - (*bullet)->getX();
        diffY = y - (*bullet)->getY();
        // If the border of the bullet touches the enemy
        // sqrt(x² + y²) < 34 :
        if (diffX * diffX + diffY * diffY < 34*34) {
            hp -= 1;
            delete *bullet;
            bullet = bullets.erase(bullet);
        }
        else {
            bullet++;
        }
    }
    return hp <= 0;
}