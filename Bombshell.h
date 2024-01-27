#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "Enemy.h"
#include "Position.h"

class Enemy;

class Bombshell{
    public:
        Bombshell(Position position, Position targetPosition, float speed, float size, bool ally, bool destructible);
        ~Bombshell() = default;
        void update(const float deltaTime);
        void draw(sf::RenderWindow &window);
        bool isDeletable() const;

        Position getPosition() const { return position; };
        float getSize() const { return size; };
        float getAngle() const { return angle; };
        std::string getType() const { return "Bombshell"; };
        bool isAlly() const { return ally; };
        bool hasExploded() const { return size!=0; };
        
        bool hitPlayer = false;
        std::vector<Enemy*> hitEnemies;

    private:
        float maxSize;
        float fallTime = 0;
        float timeToFall = 3;
        Position position, targetPosition;
        float angle, speed, size;
        bool ally;
};