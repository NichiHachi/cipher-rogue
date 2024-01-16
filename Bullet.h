#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#pragma once
class Bullet{
    public:
        Bullet(float x, float y, float angle, float speed, float hitBoxRadius, bool ally, bool destructible);
        void update();
        void draw(sf::RenderWindow& window);
        float getX();
        float getY();
        float getHitBoxRadius();
        float getAngle();
        bool isDestructible();
        
    private:
        float x,y,angle,speed, hitBoxRadius;
        bool ally, destructible;
};