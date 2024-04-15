#include <iostream>

#include "Homepage.h"


Homepage::Homepage() {
    
    if (!font.loadFromFile("./fonts/FiraCode.ttf")) {
        std::cout << "Error loading font" << std::endl;
    }

    // Set up the title
    title.setFont(font);
    title.setString("Cipher Rogue");
    title.setCharacterSize(50);  
    title.setFillColor(sf::Color::White);
    title.setPosition(320, 240);  

    // Set up the play button
    playButton.setSize(sf::Vector2f(200, 50));  
    playButton.setPosition(400, 320);  
    playButtonText.setFont(font);
    playButtonText.setString("PLAY");
    playButtonText.setCharacterSize(40);  
    playButtonText.setFillColor(sf::Color::Black);
    playButtonText.setPosition(450, 320);  

    // Set up the quit button
    quitButton.setSize(sf::Vector2f(200, 50));  
    quitButton.setPosition(400, 400);  
    quitButtonText.setFont(font);
    quitButtonText.setString("QUIT");
    quitButtonText.setCharacterSize(40);  
    quitButtonText.setFillColor(sf::Color::Black);
    quitButtonText.setPosition(450, 400);  

    // Set up the FPS slider
    fpsSlider.setPosition(320, 480);  
    fpsSlider.setRange(30, 240);  
}

void Homepage::draw(sf::RenderWindow& window) {
    window.draw(title);
    window.draw(playButton);
    window.draw(quitButton);
    window.draw(playButtonText);
    window.draw(quitButtonText);
    //fpsSlider.draw(window);
}

int Homepage::handleClick(sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (playButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            std::cout << "Play button clicked" << std::endl;
            return 1;
        }

        if (quitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            std::cout << "Quit button clicked" << std::endl;
            return 2;
        }
    }
    return 0;
}

Slider::Slider() {
    track.setSize(sf::Vector2f(200, 10));  
    track.setFillColor(sf::Color::White);

    handle.setRadius(15);  
    handle.setFillColor(sf::Color::Red);

    minValue = 0;
    maxValue = 100;
    value = minValue;
}

void Slider::setPosition(float x, float y) {
    track.setPosition(x, y);
    handle.setPosition(x, y);
}

void Slider::setRange(int min, int max) {
    minValue = min;
    maxValue = max;
    value = minValue;
}

void Slider::draw(sf::RenderWindow& window) {
    window.draw(track);
    window.draw(handle);
}