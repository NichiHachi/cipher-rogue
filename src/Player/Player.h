#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <memory>

#include "../Position.h"
#include "../Projectile/Bullet.h"
#include "../Projectile/Bombshell.h"
#include "../Wall.h"
#include "PlayerStats.h"

class Enemy;
class Bombshell;

class Player{
    public:
        static PlayerStats stats;

        Player();
        void update(sf::RenderWindow &window, std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>> bombshells,
                    std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, float deltaTime);
        void spawn();
        void shoot(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>> bombshells, Position positionTarget);
        void receiveDamageIfShot(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>> bombshells);
        void receiveDamageIfHit(std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies);
        void receiveDamage(unsigned int damage);
        void draw(sf::RenderWindow& window);
        void drawHealth(sf::RenderWindow& window);

        Position getPosition() const {return position;};
        float getSpeed() const {return speed;};
        
    private:
        Position position;
        float speed, shootTimer, hitTimer, speedBullet;
        int size, hp, hpMax;
        double angle;

        void move(std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, float deltaTime);
};