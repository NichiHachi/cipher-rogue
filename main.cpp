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
const int FPS = 30;

int main(void){
    sf::RenderWindow window(sf::VideoMode(displayX,displayY), "Nichi Hachi");

    //Time track and Framerate
    sf::Clock clock;
    float deltaTime;
    window.setFramerateLimit(FPS);

    Game game(FPS);

    while(window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)window.close();
        }

        window.clear(backgroundColor);

        deltaTime = clock.restart().asSeconds();

        game.update(window, deltaTime);
        
        game.draw(window,deltaTime);

        window.display();
    }
    return 0;
}