#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Position.h"
#include "Wall.h"

Wall::Wall(Position index) : position(index*50+Position(25,25)), size(25) {}

void Wall::draw(sf::RenderWindow &window) {
    sf::VertexArray quad(sf::Quads, 4);
    for(unsigned int i = 0; i < 4; i++) quad[i].color = sf::Color::White;

    quad[0].position = sf::Vector2f(position.x + size, position.y + size);
    quad[1].position = sf::Vector2f(position.x - size, position.y + size);
    quad[2].position = sf::Vector2f(position.x - size, position.y - size);
    quad[3].position = sf::Vector2f(position.x + size, position.y - size);

    window.draw(quad);
}

bool Wall::isInWall(Position pos) const { 
    return (pos.x - size < position.x && position.x < pos.x + size && 
            pos.y - size < position.y && position.y < pos.y + size); 
}

bool Wall::isIndexInWall(Position index) const { 
    Position newIndex = index*50+Position(25,25);
    return (newIndex.x - size < position.x && position.x < newIndex.x + size && 
            newIndex.y - size < position.y && position.y < newIndex.y + size); 
}

bool Wall::isEntityInWall(Position pos, int size) const {
    float angleEntityWall = atan2(position.y - pos.y, pos.x - position.x);
    return isInWall(pos + Position(-cos(angleEntityWall),sin(angleEntityWall))*size);
}