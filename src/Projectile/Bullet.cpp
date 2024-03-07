#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bullet.h"
#include "../Position.h"

Bullet::Bullet(Position position, float angle, float speed, float size, bool ally, bool destructible) : position(position), angle(angle), speed(speed), size(size), ally(ally), destructible(destructible){}

void Bullet::update(float deltaTime) {
    position += Position(cos(angle) , -sin(angle)) * speed * deltaTime * 60;
}

void Bullet::draw(sf::RenderWindow &window) {
    if(ally) {
        sf::VertexArray quad(sf::Quads, 4);

        for (int i = 0; i < 4; i++) quad[i].color = sf::Color::White;

        int height = size*2;
        int width = size;
        
        quad[0].position = sf::Vector2f(height * std::cos(angle) + width * std::sin(angle) + position.x,
                                        -height * std::sin(angle) + width * std::cos(angle) + position.y);
        quad[1].position = sf::Vector2f(-height * std::cos(angle) + width * std::sin(angle) + position.x,
                                        height * std::sin(angle) + width * std::cos(angle) + position.y);
        quad[2].position = sf::Vector2f(-height * std::cos(angle) - width * std::sin(angle) + position.x,
                                        height * std::sin(angle) - width * std::cos(angle) + position.y);
        quad[3].position = sf::Vector2f(height * std::cos(angle) - width * std::sin(angle) + position.x,
                                        -height * std::sin(angle) - width * std::cos(angle) + position.y);
        
        window.draw(quad);
    } 
    else {
        sf::Color enemiesBulletInvinsibleColor(255, 102, 51);
        sf::Color enemiesBulletDestructibleColor(70, 11, 102);
        sf::CircleShape circle;
        
        circle.setRadius(size);
        circle.setFillColor(destructible ? enemiesBulletDestructibleColor : enemiesBulletInvinsibleColor);
        circle.setPosition(position.x - size, position.y - size);

        window.draw(circle);
    }
}