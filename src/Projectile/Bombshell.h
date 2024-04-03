#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "../Enemy/Enemy.h"
#include "../Position.h"

class Enemy;

class Bombshell{
    public:
        Bombshell(Position position, Position positionTarget, float speed, float size, bool ally);
        ~Bombshell() = default;
        void update(const float deltaTime);
        void draw(sf::RenderWindow &window);
        void drawExplosion(sf::RenderWindow &window) const;
        bool isDeletable() const;

        Position getPosition() const;
        float getSize() const;
        float getAngle() const;
        std::string getType() const;
        bool isAlly() const;
        bool hasExploded() const;
        
        bool hitPlayer = false;
        std::vector<Enemy*> hitEnemies;

    private:
        float sizeMax;
        float fallTime = 0;
        float timeToFall = 3;
        Position position, positionTarget;
        float angle{}, speed, size;
        bool ally;
};