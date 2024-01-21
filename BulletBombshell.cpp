#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "BulletBombshell.h"
#include "Position.h"

BulletBombshell::BulletBombshell(Position position, Position targetPosition, float speed, float size, bool ally, bool destructible) : Bullet(position, 0, targetPosition, speed, 0, ally, destructible), maxSize(size*10){}

void BulletBombshell::update(const float deltaTime){
    fallTime += deltaTime;
    if(fallTime > timeToFall){
        position = targetPosition;
        size = maxSize * (fallTime - timeToFall);
    }
}

void BulletBombshell::draw(sf::RenderWindow &window){
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

bool BulletBombshell::isDeletable() const{
    return fallTime > timeToFall + 1;
}