#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bombshell.h"
#include "Position.h"

Bombshell::Bombshell(Position position, Position positionTarget, float speed, float size, bool ally, bool destructible) : position(position), positionTarget(positionTarget), speed(speed), size(0), ally(ally), sizeMax(size*10){}

void Bombshell::update(const float deltaTime){
    fallTime += deltaTime;
    if(fallTime > timeToFall){
        position = positionTarget;
        size = sizeMax * (fallTime - timeToFall);
    }
}

void Bombshell::draw(sf::RenderWindow &window){
    //Draw the bombshell itself
    if(fallTime < timeToFall){
        float percent;
        sf::Vector2f bombshellShotDrawPos;
        float angleBombshellDraw;
        sf::Color colorBombshell = ally ? sf::Color::White : sf::Color(255, 102, 51);

        if(fallTime < timeToFall/2){
            //Off set it because it feels weird if it launch in the center of the player 
            percent = fallTime*2/timeToFall + 0.01;
            bombshellShotDrawPos = sf::Vector2f(position.x + (positionTarget.x - position.x)*percent/3, position.y - percent * 1000);
            angle = std::atan2(position.y - bombshellShotDrawPos.y, bombshellShotDrawPos.x - position.x);
        } else {
            //Same off set here, we want the tip to be the center of the exposion
            percent = (fallTime*2 - timeToFall)/timeToFall - 0.01;
            bombshellShotDrawPos = sf::Vector2f(positionTarget.x - (positionTarget.x - position.x) * (1 - percent)/3 , positionTarget.y - (1 - percent) * 1000);
            angle = std::atan2(bombshellShotDrawPos.y - positionTarget.y, positionTarget.x - bombshellShotDrawPos.x);
        }

        sf::VertexArray bombshellBody(sf::Quads, 4);
        for (int i = 0; i < 4; i++) bombshellBody[i].color = colorBombshell;
        int height = sizeMax/7;
        int width = sizeMax/10;
        
        bombshellBody[0].position = sf::Vector2f(height * std::cos(angle) + width * std::sin(angle) + bombshellShotDrawPos.x,
                                        -height * std::sin(angle) + width * std::cos(angle) + bombshellShotDrawPos.y);
        bombshellBody[1].position = sf::Vector2f(-height * std::cos(angle) + width * std::sin(angle) + bombshellShotDrawPos.x,
                                        height * std::sin(angle) + width * std::cos(angle) + bombshellShotDrawPos.y);
        bombshellBody[2].position = sf::Vector2f(-height * std::cos(angle) - width * std::sin(angle) + bombshellShotDrawPos.x,
                                        height * std::sin(angle) - width * std::cos(angle) + bombshellShotDrawPos.y);
        bombshellBody[3].position = sf::Vector2f(height * std::cos(angle) - width * std::sin(angle) + bombshellShotDrawPos.x,
                                        -height * std::sin(angle) - width * std::cos(angle) + bombshellShotDrawPos.y);

        window.draw(bombshellBody);


        sf::CircleShape bombshellTip;
        bombshellTip.setRadius(width);
        bombshellTip.setFillColor(colorBombshell);

        if(fallTime < timeToFall/2){
            bombshellTip.setPosition(height * std::cos(angle) + bombshellShotDrawPos.x - width, 
                               -height * std::sin(angle) + bombshellShotDrawPos.y - width);
        } else {
            bombshellTip.setPosition(height * std::cos(angle) + bombshellShotDrawPos.x - width, 
                               -height * std::cos(angle) + bombshellShotDrawPos.y + width);
        }
        
        window.draw(bombshellTip);
    

        sf::VertexArray bombshellTail(sf::Quads,4);
        for (int i = 0; i < 4; i++) bombshellTail[i].color = colorBombshell;

        bombshellTail[0].position = sf::Vector2f(-height*1.5 * std::cos(angle) + width * std::sin(angle) + bombshellShotDrawPos.x,
                                        height*1.5 * std::sin(angle) + width * std::cos(angle) + bombshellShotDrawPos.y);
        
        bombshellTail[1].position = sf::Vector2f(-height * std::cos(angle) + bombshellShotDrawPos.x,
                                        height * std::sin(angle) + bombshellShotDrawPos.y);
        bombshellTail[2].position = sf::Vector2f(-height*2 * std::cos(angle) + bombshellShotDrawPos.x,
                                        height*2 * std::sin(angle) + bombshellShotDrawPos.y);

        bombshellTail[3].position = sf::Vector2f(-height*2.5 * std::cos(angle) + width * std::sin(angle) + bombshellShotDrawPos.x,
                                        height*2.5 * std::sin(angle) + width * std::cos(angle) + bombshellShotDrawPos.y);

        window.draw(bombshellTail);


        bombshellTail[0].position = sf::Vector2f(-height*1.5 * std::cos(angle) - width * std::sin(angle) + bombshellShotDrawPos.x,
                                        height*1.5 * std::sin(angle) - width * std::cos(angle) + bombshellShotDrawPos.y);
        

        bombshellTail[3].position = sf::Vector2f(-height*2.5 * std::cos(angle) - width * std::sin(angle) + bombshellShotDrawPos.x,
                                        height*2.5 * std::sin(angle) - width * std::cos(angle) + bombshellShotDrawPos.y);

        window.draw(bombshellTail);
    }
}

void Bombshell::drawExplosion(sf::RenderWindow &window){  
    sf::Color colorExplosion = ally ? sf::Color(0, 255, 255) : sf::Color(255, 0, 0);
    
    if(fallTime < timeToFall){ 
        //Draw the danger zone
        sf::CircleShape dangerZone;
        dangerZone.setRadius(sizeMax);
        colorExplosion.a = 75;
        dangerZone.setFillColor(colorExplosion);
        dangerZone.setPosition(positionTarget.x-sizeMax, positionTarget.y-sizeMax);
        window.draw(dangerZone);

        //Draw the timer
        float percent = fallTime/timeToFall;
        sf::CircleShape dangerZoneTimer;
        dangerZoneTimer.setRadius(sizeMax * percent);
        dangerZoneTimer.setFillColor(colorExplosion);
        dangerZoneTimer.setPosition(positionTarget.x-sizeMax * percent, positionTarget.y-sizeMax * percent);
        window.draw(dangerZoneTimer);
    }
    else{
        //Draw the exposion
        sf::CircleShape bombZone;
        bombZone.setRadius(size);
        bombZone.setFillColor(sf::Color::Transparent);
        bombZone.setOutlineColor(colorExplosion);
        bombZone.setOutlineThickness(15);
        bombZone.setPosition(position.x-size, position.y-size);
        window.draw(bombZone);
    }
}

bool Bombshell::isDeletable() const{
    return fallTime > timeToFall + 1;
}