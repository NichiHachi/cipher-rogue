#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Wall.h"

Wall::Wall(float x, float y) {
    this->x = x;
    this->y = y;
    this->size = 25;
}

void Wall::draw(sf::RenderWindow &window) {
    sf::VertexArray quad(sf::Quads, 4);
    quad[0].color = sf::Color::White;
    quad[1].color = sf::Color::White;
    quad[2].color = sf::Color::White;
    quad[3].color = sf::Color::White;

    quad[0].position = sf::Vector2f(x + size, y + size);
    quad[1].position = sf::Vector2f(x - size, y + size);
    quad[2].position = sf::Vector2f(x - size, y - size);
    quad[3].position = sf::Vector2f(x + size, y - size);
    window.draw(quad);
}

bool Wall::isInWall(float objectX, float objectY) { return (objectX < x + size && objectX > x - size && objectY < y + size && objectY > y - size); }

float Wall::getX() { return x; }

float Wall::getY() { return y; }

float Wall::getSize() { return size; }
