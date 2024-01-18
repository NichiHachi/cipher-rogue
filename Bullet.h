#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Position.h"

class Bullet{
    public:
        Bullet(Position position, float angle, float speed, float size, bool ally, bool destructible);
        void update();
        void draw(sf::RenderWindow& window);
        Position getPosition() const { return position; };
        float getSize() const { return size; };
        float getAngle() const { return angle; };
        bool isDestructible() const { return destructible; };
        
    private:
        Position position;
        float angle,speed, size;
        bool ally, destructible;
};