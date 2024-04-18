#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>
#include <string>

#include "Game.h"
#include "Homepage.h"
#include "Deathpage.h"

const int displayX = 1000;
const int displayY = 1000;
const sf::Color backgroundColor(0,0,0);
const int FPS = 120;

int gameLoop(sf::RenderWindow& window){
    sf::Clock clock;
    float deltaTime;
    Game game;
    game.initLevel();
    while(window.isOpen() && !game.isPlayerDead()){
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

    return game.levelAchieved();
}

void deathLoop(sf::RenderWindow& window, int levelAchieved){
    Deathpage deathpage(levelAchieved);
    bool returnToMenu = false;
    while(window.isOpen() && !returnToMenu){
        sf::Event event{};
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(backgroundColor);
        deathpage.draw(window);
        
        switch(deathpage.handleClick(window)){
            case 1:
                returnToMenu = true;
                break;
            case 2:
                window.close();
                break;
        }

        window.display();
    }
}

int main(){
    sf::RenderWindow window(sf::VideoMode(displayX,displayY), "Cipher Rogue");

    //Time track and Framerate
    window.setFramerateLimit(FPS);

    Homepage homepage;
    int level;

    while(window.isOpen()){
        sf::Event event{};
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed) window.close();
        }

        window.clear(backgroundColor);
        homepage.draw(window);
        
        switch(homepage.handleClick(window)){
            case 1:
                level = gameLoop(window);
                deathLoop(window, level);
                break;
            case 2:
                window.close();
                break;
        }

        window.display();
    }
    return 0;
}