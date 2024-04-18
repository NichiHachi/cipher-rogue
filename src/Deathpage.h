#pragma once

#include <SFML/Graphics.hpp>

class Deathpage {
private:
    int level;
    sf::Font font;
    sf::Text message;
    sf::Text levelText;
    sf::RectangleShape menuButton;
    sf::RectangleShape quitButton;
    sf::Text menuButtonText;
    sf::Text quitButtonText;

public:
    Deathpage(int levelAchieved);

    void draw(sf::RenderWindow& window);

    int handleClick(sf::RenderWindow& window);
};