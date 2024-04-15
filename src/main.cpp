#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include <string>

#include "Game.h"
#include "Homepage.h"

const int displayX = 1000;
const int displayY = 1000;
const sf::Color backgroundColor(0,0,0);
const int FPS = 120;

void gameLoop(sf::RenderWindow& window){
    sf::Clock clock;
    float deltaTime;
    Game game;
    game.initLevel();
    while(window.isOpen()){
        sf::Event event{};
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(backgroundColor);
        deltaTime = clock.restart().asSeconds();

        game.update(window, deltaTime>0.2f ? 0.2f : deltaTime);
        game.draw(window, deltaTime>0.2f ? 0.2f : deltaTime);

        window.display();
    }
}

int main(){
    sf::RenderWindow window(sf::VideoMode(displayX,displayY), "Cipher Rogue");

    //Time track and Framerate
    window.setFramerateLimit(FPS);

    Homepage homepage;

    while(window.isOpen()){
        sf::Event event{};
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed) window.close();
        }

        homepage.draw(window);
        switch(homepage.handleClick(window)){
            case 1:
                gameLoop(window);
                break;
            case 2:
                window.close();
                break;
        }

        window.display();
    }
    return 0;
}