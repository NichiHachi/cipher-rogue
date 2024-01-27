#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bombshell.h"
#include "Position.h"

Bombshell::Bombshell(Position position, Position targetPosition, float speed, float size, bool ally, bool destructible) : position(position), targetPosition(targetPosition), speed(speed), size(0), ally(ally), maxSize(size*10){}

void Bombshell::update(const float deltaTime){
    fallTime += deltaTime;
    if(fallTime > timeToFall){
        position = targetPosition;
        size = maxSize * (fallTime - timeToFall);
    }
}

void Bombshell::draw(sf::RenderWindow &window){
    sf::Color bombshellColor(255, 255, 0);
    sf::CircleShape circle;
    circle.setRadius(maxSize/10);
    circle.setFillColor(bombshellColor);
    
    if(fallTime < timeToFall/2){
        float percent = fallTime*2/timeToFall;
        circle.setPosition(position.x + (targetPosition.x - position.x)*percent/4, position.y - percent * 1000);
        window.draw(circle);
    }
    else if(fallTime < timeToFall){
        float percent = (fallTime*2 - timeToFall)/timeToFall;
        circle.setPosition(targetPosition.x - (targetPosition.x - position.x) * (1 - percent)/4 , targetPosition.y - (1 - percent) * 1000);
        window.draw(circle);
    }
    else{
        sf::CircleShape bombZone;
        bombZone.setRadius(size);
        bombZone.setFillColor(sf::Color::Transparent);
        bombZone.setOutlineColor(sf::Color::Red);
        bombZone.setOutlineThickness(15);
        bombZone.setPosition(position.x-size, position.y-size);
        window.draw(bombZone);
    }
}

bool Bombshell::isDeletable() const{
    return fallTime > timeToFall + 1;
}