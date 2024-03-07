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
        void draw(sf::RenderWindow &window);


        Position getPosition() const { return position; };
        float getSize() const { return size; };
        float getAngle() const { return angle; };
        bool isDestructible() const { return destructible; };
        std::string getType() const { return "Bullet"; };

    private :
        Position position;
        float angle,speed, size;
        bool ally, destructible;
};