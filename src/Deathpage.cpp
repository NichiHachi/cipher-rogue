#include <iostream>

#include "Deathpage.h"

Deathpage::Deathpage(int levelAchieved) : level(levelAchieved-1) {
    if (!font.loadFromFile("./fonts/FiraCode.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }

    // Set up the title
    message.setFont(font);
    message.setString("You lost!");
    message.setCharacterSize(50);  
    message.setFillColor(sf::Color::White);
    message.setPosition(380, 340);  

    levelText.setFont(font);
    levelText.setString("Level achieved: " + std::to_string(level));
    levelText.setCharacterSize(50);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(250, 420); 

    // Set up the play button
    menuButton.setSize(sf::Vector2f(200, 50));  
    menuButton.setPosition(400, 600);
    menuButtonText.setFont(font);
    menuButtonText.setString("MENU");
    menuButtonText.setCharacterSize(40);  
    menuButtonText.setFillColor(sf::Color::Black);
    menuButtonText.setPosition(450, 600);

    // Set up the quit button
    quitButton.setSize(sf::Vector2f(200, 50));  
    quitButton.setPosition(400, 680);
    quitButtonText.setFont(font);
    quitButtonText.setString("QUIT");
    quitButtonText.setCharacterSize(40);  
    quitButtonText.setFillColor(sf::Color::Black);
    quitButtonText.setPosition(450, 680);
}

void Deathpage::draw(sf::RenderWindow& window) {
    window.draw(message);
    window.draw(levelText);
    window.draw(menuButton);
    window.draw(quitButton);
    window.draw(menuButtonText);
    window.draw(quitButtonText);
}

int Deathpage::handleClick(sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (menuButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            std::cout << "Menu button clicked" << std::endl;
            return 1;
        }

        if (quitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            std::cout << "Quit button clicked" << std::endl;
            return 2;
        }
    }
    return 0;
}
