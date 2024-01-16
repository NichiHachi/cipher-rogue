#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Bullet.h"
#include "Wall.h"

#pragma once
class Enemy{
    public:
        virtual void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies)=0;
        virtual void draw(sf::RenderWindow& window)=0;
        virtual ~Enemy(){};

        bool receiveDamageIfShot(std::vector<Bullet*>& bullets);
        void setCoordonates(float x, float y) {this->x=x; this->y=y;};
        void receiveDamage(int damage) {hp -= damage;};
        float getX() {return x;};
        float getY() {return y;};
        int getSize() {return size;};
        bool isMovable() {return movable;};

    protected:
        float x, y, speed, angle, shootTimer;
        int hp, size;
        const bool movable;
        Enemy(float x, float y, float speed, float angle, float shootTimer, int hp, int size, bool movable);
        void adjustPositionBasedOnCollisions(std::vector<Enemy*>& enemies, std::vector<Wall> walls);
};