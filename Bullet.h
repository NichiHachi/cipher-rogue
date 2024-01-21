#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "Position.h"
#include "Wall.h"

class Bullet{
    public:
        virtual void update(const float deltaTime) = 0;
        virtual void draw(sf::RenderWindow& window) = 0;
        virtual std::string getType() const = 0;
        virtual bool isDeletable() const = 0;
        virtual ~Bullet(){};

        Position getPosition() const { return position; };
        float getSize() const { return size; };
        float getAngle() const { return angle; };
        bool isDestructible() const { return destructible; };
        bool isInWall() const;
        
    protected:
        Position position, targetPosition;
        float angle,speed, size;
        bool ally, destructible;

        Bullet(Position position, float angle, Position targetPosition, float speed, float size, bool ally, bool destructible);
};