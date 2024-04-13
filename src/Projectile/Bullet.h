#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "../Position.h"

class Bullet{
    public:
        Bullet(Position position, float angle, float speed, float size, bool ally, bool destructible);
        ~Bullet() = default;
        void update(float deltaTime);
        void draw(sf::RenderWindow &window) const;


        Position getPosition() const;
        float getSize() const;
        float getAngle() const;
        bool isDestructible() const;
        int getDamage() const;

    private :
        Position position;
        float angle,speed, size;
        bool ally, destructible;
};