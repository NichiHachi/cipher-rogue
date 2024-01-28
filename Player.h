#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <memory>

#include "Position.h"
#include "Bullet.h"
#include "Bombshell.h"
#include "Wall.h"
#include "PlayerStats.h"

class Enemy;
class Bombshell;

class Player{
    public:
        static PlayerStats stats;

        Player();
        void update(sf::RenderWindow &window, std::vector<std::unique_ptr<Bullet>> &bullets, std::vector<std::unique_ptr<Bombshell>> &bombshells,
                    std::vector<std::unique_ptr<Wall>> &walls, float deltaTime);
        void spawn();
        void shoot(std::vector<std::unique_ptr<Bullet>> &bullets, std::vector<std::unique_ptr<Bombshell>> &bombshells, Position positionTarget);
        void receiveDamageIfShot(std::vector<std::unique_ptr<Bullet>> &bullets, std::vector<std::unique_ptr<Bombshell>> &bombshells);
        void receiveDamageIfHit(std::vector<std::unique_ptr<Enemy>> &enemies);
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

        void move(std::vector<std::unique_ptr<Wall>> &walls);
};