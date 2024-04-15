#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Position.h"
#include "Wall.h"

Wall::Wall(Position index) : position(index*50+Position(25,25)), size(25) {}

void Wall::draw(sf::RenderWindow &window) const {
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

Position Wall::getPoint(unsigned int index) const {
    switch(index){
        case 0:
            return {position.x + size, position.y + size};
        case 1:
            return {position.x - size, position.y + size};
        case 2:
            return {position.x - size, position.y - size};
        case 3:
            return {position.x + size, position.y - size};
        default:
            return {0,0};
    }
}