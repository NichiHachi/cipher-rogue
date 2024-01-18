#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <algorithm>

#include "Position.h"
#include "Bullet.h"
#include "Player.h"
#include "Wall.h"

Player::Player() : position(Position(0,0)), speed(5), hp(10), shootTimer(0), hitTimer(2), size(19) {}

void Player::update(std::vector<Bullet*> &bullets, sf::RenderWindow &window, float timePassed, std::vector<Wall> walls) {
    move(window, walls);

    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    angle = atan2(position.y - mousePosition.y, mousePosition.x - position.x);

    shootTimer += timePassed;
    hitTimer += timePassed;

    shoot(bullets);
}

void Player::spawn(){
    position = Position(500,900);
}

void Player::move(sf::RenderWindow &window, const std::vector<Wall> walls) {
    int xAxisMove = 0;
    int yAxisMove = 0;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) yAxisMove++;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) xAxisMove--;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) yAxisMove--;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) xAxisMove++;
    
    if (yAxisMove != 0 && xAxisMove != 0) {
        position += Position(xAxisMove*sqrt(2)/2,-yAxisMove*sqrt(2)/2)*speed;
    }
    else{
        position += Position(xAxisMove,-yAxisMove)*speed;
    }
 
    //Refaire 
    for (Wall wall : walls) {
        Position wallPos = wall.getPosition();
        float anglePlayerWall = atan2(wallPos.y - position.y, wallPos.x - position.x);
        if (wall.isInWall(position + Position(cos(anglePlayerWall),sin(anglePlayerWall))*size)){
            if (anglePlayerWall < M_PI / 4 && anglePlayerWall > -M_PI / 4)
                position.x -= size - ((wallPos.x - wall.getSize()) - position.x);

            else if (anglePlayerWall > M_PI * 3 / 4 || anglePlayerWall < -M_PI * 3 / 4)
                position.x += size - (position.x - (wallPos.x + wall.getSize()));

            else if (anglePlayerWall > M_PI / 4 && anglePlayerWall < M_PI * 3 / 4) 
                position.y -= size - ((wallPos.y - wall.getSize()) - position.y);

            else 
                position.y += size - (position.y - (wallPos.y + wall.getSize()));
            }
        }

        //If out of bounds, go back in bounds
        if (position.x < size) position.x = size;
        else if (position.x > 1000-size) position.x = 1000-size;
        if (position.y < size) position.y = size;
        else if (position.y > 1000-size) position.y = 1000-size;
    }

void Player::shoot(std::vector<Bullet*> &bullets) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (shootTimer > 0.2) {
            bullets.push_back(new Bullet(position, angle, 10, 10, true, false));
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
    sf::CircleShape circle;
    circle.setRadius(10);
    circle.setFillColor(sf::Color::Red);
    sf::VertexArray quad(sf::Quads, 4);
    for(unsigned int i = 0; i < 4; i++) quad[i].color = sf::Color::Red;

    for (int i = 0; i < hp; i++) {
        circle.setPosition(10 * cos(M_PI*3/4) + 40*i + 20,
                        10 * sin(M_PI*3/4) + 10);
        window.draw(circle);
        circle.setPosition(10 * cos(M_PI/4) + 40*i + 20,
                        10 * sin(M_PI/4) + 10);
        window.draw(circle);

        quad[0].position = sf::Vector2f(40*i + 30, 21);
        quad[1].position = sf::Vector2f(40*i + 44, 35);
        quad[2].position = sf::Vector2f(40*i + 30, 49);
        quad[3].position = sf::Vector2f(40*i + 16, 35);
        window.draw(quad);
    }
}

void Player::receiveDamageIfShot(std::vector<Bullet*> &bullets) {
    Position diffPos;
    int hitBoxBoth;
    for(auto bullet = bullets.begin(); bullet != bullets.end();){
        diffPos = position - (*bullet)->getPosition();
        hitBoxBoth = size+(*bullet)->getSize();
        if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth*hitBoxBoth) {
            receiveDamage(1);
            delete *bullet;
            bullet = bullets.erase(bullet);
        }
        else {
            bullet++;
        }
    }
}

void Player::receiveDamageIfHit(std::vector<Enemy*> enemies) {
    Position diffPos;
    int hitBoxBoth;
    for(Enemy* enemy : enemies){
        diffPos = position - enemy->getPosition();
        hitBoxBoth = size+enemy->getSize();
        if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth * hitBoxBoth) receiveDamage(1);
    }
}

void Player::receiveDamage(unsigned int damage){
    if(hitTimer > 2){
        hp -= damage;
        hitTimer = 0;
    }
}