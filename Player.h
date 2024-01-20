#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Position.h"
#include "Bullet.h"
#include "Wall.h"
#include "PlayerStats.h"

class Enemy;

class Player{
    public:
        static PlayerStats stats;

        Player();
        void update(std::vector<Bullet*>& bullets,sf::RenderWindow& window,float timePassed, std::vector<Wall> walls);
        void spawn();
        void move(sf::RenderWindow& window, std::vector<Wall> walls);
        void shoot(std::vector<Bullet*>& bullets);
        void receiveDamageIfShot(std::vector<Bullet*>& bullets);
        void receiveDamageIfHit(std::vector<Enemy*> enemies);
        void receiveDamage(unsigned int damage);
        void draw(sf::RenderWindow& window );
        void drawHealth(sf::RenderWindow& window );

        Position getPosition() const {return position;};
        float getSpeed() const {return speed;};
    private:
        Position position;
        float speed,shootTimer,hitTimer,speedBullet;
        int size, hp, hpMax;
        double angle;
};