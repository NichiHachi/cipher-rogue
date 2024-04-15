#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Position.h"

/**
 * @brief The Wall class represents a wall object in the game.
 */
class Wall{
    public:
        /**
         * @brief Constructs a Wall object with the given position.
         * @param position The position of the wall.
         */
        explicit Wall(Position position);

        /**
         * @brief Draws the wall on the given window.
         * @param window The SFML render window to draw on.
         */
        void draw(sf::RenderWindow& window) const;

        /**
         * @brief Checks if the given object position is inside the wall.
         * @param objectPos The position of the object to check.
         * @return True if the object is inside the wall, false otherwise.
         */
        bool isInWall(Position objectPos) const;

        /**
         * @brief Returns the position of the wall.
         * @return The position of the wall.
         */
        Position getPosition() const {return position;};

        /**
         * @brief Returns the size of the wall.
         * @return The size of the wall.
         */
        int getSize() const {return size;};

        /**
         * @brief Returns the point at the given index on the wall.
         * @param index The index of the point to retrieve.
         * @return The position of the point at the given index.
         */
        Position getPoint(unsigned int index) const;

    private:
        int size;
        Position position;
};