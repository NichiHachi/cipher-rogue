#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bullet.h"

Bullet::Bullet(float x, float y, float angle, float speed, float hitBoxRadius, bool ally, bool destructible) {
    this->x = x;
    this->y = y;
    this->speed = speed;
    this->angle = angle;
    this->hitBoxRadius = hitBoxRadius;
    this->ally = ally;
    this->destructible = destructible;
}

void Bullet::update() {
    x += speed * cos(angle);
    y -= speed * sin(angle);
}

void Bullet::draw(sf::RenderWindow &window) {
    if(ally) {
        sf::VertexArray quad(sf::Quads, 4);

        for (int i = 0; i < 4; i++) quad[i].color = sf::Color::White;

        int height = hitBoxRadius*2;
        int width = hitBoxRadius;
        
        quad[0].position = sf::Vector2f(height * std::cos(angle) + width * std::sin(angle) + x,
                                        -height * std::sin(angle) + width * std::cos(angle) + y);
        quad[1].position = sf::Vector2f(-height * std::cos(angle) + width * std::sin(angle) + x,
                                        height * std::sin(angle) + width * std::cos(angle) + y);
        quad[2].position = sf::Vector2f(-height * std::cos(angle) - width * std::sin(angle) + x,
                                        height * std::sin(angle) - width * std::cos(angle) + y);
        quad[3].position = sf::Vector2f(height * std::cos(angle) - width * std::sin(angle) + x,
                                        -height * std::sin(angle) - width * std::cos(angle) + y);
        
        window.draw(quad);
    } 
    else {
        sf::Color enemiesBulletInvinsibleColor(255, 102, 51);
        sf::Color enemiesBulletDestructibleColor(70, 11, 102);
        sf::CircleShape circle;
        circle.setRadius(hitBoxRadius);
        circle.setFillColor(destructible ? enemiesBulletDestructibleColor : enemiesBulletInvinsibleColor);
        circle.setPosition(x - hitBoxRadius, y - hitBoxRadius);
        window.draw(circle);
    }
}

float Bullet::getX() { return x; }

float Bullet::getY() { return y; }

float Bullet::getHitBoxRadius() { return hitBoxRadius; }

float Bullet::getAngle() { return angle; }

bool Bullet::isDestructible() { return destructible; }