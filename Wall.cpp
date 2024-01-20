#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Position.h"
#include "Wall.h"

Wall::Wall(Position position) : position(position), size(25) {}

void Wall::draw(sf::RenderWindow &window ) {
    sf::VertexArray quad(sf::Quads, 4);
    for(unsigned int i = 0; i < 4; i++) quad[i].color = sf::Color::White;

    quad[0].position = sf::Vector2f(position.x + size, position.y + size);
    quad[1].position = sf::Vector2f(position.x - size, position.y + size);
    quad[2].position = sf::Vector2f(position.x - size, position.y - size);
    quad[3].position = sf::Vector2f(position.x + size, position.y - size);

    window.draw(quad);
}

bool Wall::isInWall(Position objectPos) const { 
    return (objectPos.x - size < position.x && position.x < objectPos.x + size && 
            objectPos.y - size < position.y && position.y < objectPos.y + size); 
}
