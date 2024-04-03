#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Position.h"

class Wall{
    public:
        explicit Wall(Position position);
        void draw(sf::RenderWindow& window) const;
        bool isInWall(Position objectPos) const;
        bool isIndexInWall(Position index) const;
        Position getPosition() const {return position;};
        int getSize() const {return size;};
        Position getPoint(unsigned int index) const;

    private:
        int size;
        Position position;
};