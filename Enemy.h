#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Bullet.h"
#include "Wall.h"

#pragma once
class Enemy{
    public:
        virtual void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies)=0;
        virtual void draw(sf::RenderWindow& window)=0;
        virtual float getX()=0;
        virtual float getY()=0;
        virtual bool receiveDamageIfShot(std::vector<Bullet*>& bullets)=0;
        virtual int getSize()=0;
        virtual bool isMovable()=0;
        virtual void setCoordonates(float x, float y)=0;
};