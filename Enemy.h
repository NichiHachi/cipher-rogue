#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Bullet.h"
#include "Wall.h"

#pragma once
class Enemy{
    public:
        Enemy(float x, float y, float angle, float speed, int hp, float shootTimer);
        virtual void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls)=0;
        virtual void draw(sf::RenderWindow& window)=0;
        bool getShot(std::vector<Bullet*>& bullets);
        float getX() const {return x;};
        float getY() const {return y;};

    private:
        float x,y,speed,angle,shootTimer;
        int hp;
};