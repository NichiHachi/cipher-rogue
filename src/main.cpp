#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include <string>

#include "Game.h"

const int displayX = 1000;
const int displayY = 1000;
const sf::Color backgroundColor(0,0,0);
const int FPS = 120;

int main(){
    sf::RenderWindow window(sf::VideoMode(displayX,displayY), "Nichi Hachi");

    //Time track and Framerate
    sf::Clock clock;
    float deltaTime;
    window.setFramerateLimit(FPS);

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
    return 0;
}