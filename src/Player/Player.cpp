#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <memory>

#include "Player.h"
#include "../Enemy/Enemy.h"
#include "../Position.h"
#include "../Projectile/Bullet.h"
#include "../Projectile/Bombshell.h"
#include "../Wall.h"
#include "PlayerStats.h"

PlayerStats Player::stats;

Player::Player() : position(Position(0,0)), speed(5*stats.speedFactor), hp(49), hpMax(10), shootTimer(0), hitTimer(2), size(19), speedBullet(10*stats.speedBulletFactor) {}

void Player::update(sf::RenderWindow& window, std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>> bombshells,
                    std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, float deltaTime) {
    move(walls, deltaTime);

    shootTimer += deltaTime;
    hitTimer += deltaTime;
        
    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    angle = atan2(position.y - mousePosition.y, mousePosition.x - position.x);
    shoot(bullets, bombshells, Position(mousePosition.x, mousePosition.y));
}

void Player::spawn(){
    position = Position(500,900);
}

void Player::move(std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, float deltaTime) {
    int xAxisMove = 0;
    int yAxisMove = 0;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) yAxisMove++;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) xAxisMove--;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) yAxisMove--;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) xAxisMove++;

    if (yAxisMove != 0 && xAxisMove != 0) {
        position += Position(xAxisMove*sqrt(2)/2,-yAxisMove*sqrt(2)/2) * speed * deltaTime * 60;
    }
    else{
        position += Position(xAxisMove,-yAxisMove) * speed * deltaTime * 60;
    }
 
    float wallSize;
    for (unsigned int i = 0; i < walls->size(); i++) {
        Position wallPos = walls->at(i)->getPosition();
        float angleWallPlayer = atan2(wallPos.y - position.y, position.x - wallPos.x);
        if (walls->at(i)->isInWall(position + Position(-cos(angleWallPlayer),sin(angleWallPlayer))*size)){
            float wallSize = walls->at(i)->getSize();
            if (-M_PI/4 <= angleWallPlayer && angleWallPlayer <= M_PI/4) {
                position.x += size - (position.x - (wallPos.x + wallSize));
            } else if (angleWallPlayer >= M_PI*3/4 || angleWallPlayer <= -M_PI*3/4) {
                position.x -= size - ((wallPos.x - wallSize) - position.x);
            } else if (M_PI/4 <= angleWallPlayer && angleWallPlayer <= M_PI*3/4) {
                position.y -= size - ((wallPos.y - wallSize) - position.y);
            } else {
                position.y += size - (position.y - (wallPos.y + wallSize));
            }
        }
    }

    //If out of bounds, go back in bounds
    if (position.x < size) position.x = size;
    else if (position.x > 1000-size) position.x = 1000-size;
    if (position.y < size) position.y = size;
    else if (position.y > 1000-size) position.y = 1000-size;
}

void Player::shoot(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>> bombshells, Position targetPosition) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (shootTimer > 0.2) {
            bullets->push_back(std::make_unique<Bullet>(position, angle, speedBullet, 10, true, true));
            //bombshells->push_back(std::make_unique<Bombshell>(position, targetPosition, speedBullet, 10, true, true));
            shootTimer = 0;
        }
    }
}

void Player::draw(sf::RenderWindow &window) {
    int height = size*24/19;
    int width = size;
    float angle_point_triangle_1 = atan2(height, width);
    float angle_point_triangle_2 = atan2(height, -width);
    float distance_point_triangle = sqrt(height * height + width * width);

    sf::VertexArray player_half_part(sf::Triangles, 3);

    //Define the color of the player
    for(unsigned int i = 0; i < 3; i++) player_half_part[i].color = sf::Color::White;

    //Define the coordonate of the player's point that are aline with the mouse
    Position player_down = position + Position(-cos(angle),sin(angle)) * (float)(height*0.6);
    Position player_up = position + Position(cos(angle),-sin(angle)) * height;

    float angle_triangle_left = angle_point_triangle_1 + angle + M_PI / 2;
    Position player_left = position + Position(cos(angle_triangle_left),-sin(angle_triangle_left)) * distance_point_triangle;

    float angle_triangle_right = angle_point_triangle_2 + angle + M_PI / 2;
    Position player_right = position + Position(cos(angle_triangle_right),-sin(angle_triangle_right)) * distance_point_triangle;

    //Set the position of the down and top points
    player_half_part[1].position = sf::Vector2f(player_up.x, player_up.y);
    player_half_part[2].position = sf::Vector2f(player_down.x, player_down.y);

    //Define the position of the left part points
    player_half_part[0].position = sf::Vector2f(player_left.x, player_left.y);

    //Draw the left part
    window.draw(player_half_part);

    //Define the position of right part points
    player_half_part[0].position = sf::Vector2f(player_right.x, player_right.y);

    //Draw the right part
    window.draw(player_half_part);
}

void Player::drawHealth(sf::RenderWindow &window) {
    int height = 30;
    int length = 30;
    int numberPerLine = 15;
    sf::Vector2f startPosition(950,950);

    sf::Color colorLayer[6] = {
        sf::Color::Red,
        sf::Color(255, 165, 0), // Orange
        sf::Color::Green,
        sf::Color::Cyan,
        sf::Color::Blue,
        sf::Color::Magenta
    };

    sf::VertexArray quad(sf::Quads, 4);

    for(int hpNumber=0; hpNumber<hp; hpNumber++){
        if(hpNumber%numberPerLine == 0) {
            for(unsigned int i = 0; i < 4; i++) quad[i].color = colorLayer[std::min(hpNumber/numberPerLine,5)];
        }

        quad[0].position = startPosition + sf::Vector2f(length/2,-height/2) - sf::Vector2f(length * (hpNumber%numberPerLine) + 10 , 0);
        quad[1].position = startPosition + sf::Vector2f(length/6,height/2) - sf::Vector2f(length * (hpNumber%numberPerLine) + 10 , 0);
        quad[2].position = startPosition + sf::Vector2f(-length/2,height/2) - sf::Vector2f(length * (hpNumber%numberPerLine) + 10 , 0);
        quad[3].position = startPosition + sf::Vector2f(-length/6,-height/2) - sf::Vector2f(length * (hpNumber%numberPerLine) + 10 , 0);
        
        window.draw(quad); 
    }
}

void Player::receiveDamageIfShot(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>> bombshells) {
    Position diffPos;
    float hitBoxBoth;
    auto bullet = bullets->begin();
    while (bullet != bullets->end()) {
        diffPos = position - (*bullet)->getPosition();
        hitBoxBoth = size+(*bullet)->getSize();
        if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth*hitBoxBoth) {
            receiveDamage(1);
            bullet = bullets->erase(bullet);
        }
        else {
            bullet++;
        }
    }
    
    for(unsigned int i = 0; i < bombshells->size(); i++){
        if(!bombshells->at(i)->hasExploded()) continue;
        if( bombshells->at(i)->isAlly() || bombshells->at(i)->hitPlayer) continue;

        diffPos = position - bombshells->at(i)->getPosition();
        hitBoxBoth = size + bombshells->at(i)->getSize();
        if(diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth * hitBoxBoth){
            receiveDamage(2);
            bombshells->at(i)->hitPlayer = true;
        }
    }
}

void Player::receiveDamageIfHit(std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies) {
    Position diffPos;
    int hitBoxBoth;
    for(unsigned int i = 0; i < enemies->size(); i++){
        diffPos = position - enemies->at(i)->getPosition();
        hitBoxBoth = size + enemies->at(i)->getSize();
        if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth * hitBoxBoth) 
            receiveDamage(1);
    }
}

void Player::receiveDamage(unsigned int damage){
    if(hitTimer > 2){
        hp -= damage;
        hitTimer = 0;
    }
}