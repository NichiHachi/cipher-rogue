#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Position.h"

class Wall{
    public:
        Wall(Position position);
        void draw(sf::RenderWindow& window);
        bool isInWall(Position objectPos) const;
        bool isIndexInWall(Position index) const;
        bool isEntityInWall(Position pos, int size) const;
        Position getPosition() const {return position;};
        int getSize() const {return size;};

    private:
        int size;
        Position position;
};