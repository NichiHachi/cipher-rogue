#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bullet.h"
#include "Player.h"
#include "Wall.h"

Player::Player() {
    this->x = 0;
    this->y = 0;
    this->speed = 5;
    this->hp = 10;
    this->shootTimer = 0;
    this->hitTimer = 2;
    this->size = 19;
}

void Player::update(std::vector<Bullet*> &bullets, sf::RenderWindow &window, float timePassed, std::vector<Wall> walls) {
    move(window, walls);

    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    angle = atan2(y - mousePosition.y, mousePosition.x - x);

    shootTimer += timePassed;
    hitTimer += timePassed;

    shoot(bullets);
}

void Player::spawn(){
    x = 500;
    y = 900;
}

void Player::move(sf::RenderWindow &window, std::vector<Wall> walls) {
    float angleMove = -1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) &&
        sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        angleMove = M_PI / 4;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        angleMove = M_PI * 3 / 4;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        angleMove = -M_PI * 3 / 4;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) &&
                sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        angleMove = -M_PI / 4;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        angleMove = M_PI / 2;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        angleMove = -M_PI / 2;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        angleMove = 0;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
        angleMove = M_PI;
    }

    if(angleMove != -1){
        x += cos(angleMove) * speed;
        y -= sin(angleMove) * speed;
        if (walls.size() != 0) {
            float wallX, wallY, anglePlayerWall;
            for (Wall wall : walls) {
                wallX = wall.getX();
                wallY = wall.getY();
                anglePlayerWall = atan2(-y + wallY, -x + wallX);
                if (wall.isInWall(x+cos(anglePlayerWall)*size, y+sin(anglePlayerWall)*size)){
                    if(anglePlayerWall<M_PI/4 && anglePlayerWall>-M_PI/4){
                        x -= size - ((wallX - wall.getSize()) - x);
                    }
                    else if(anglePlayerWall>M_PI*3/4 || anglePlayerWall<-M_PI*3/4){
                        x += size - (x - (wallX + wall.getSize()));
                    }
                    else if(anglePlayerWall>M_PI/4 && anglePlayerWall<M_PI*3/4){
                        y -= size - ((wallY - wall.getSize()) - y);
                    }
                    //anglePlayerWall<-M_PI/4 && anglePlayerWall>-M_PI*3/4
                    else{
                        y += size - (y - (wallY + wall.getSize()));
                    }
                }
            }
        }

        //If out of bounds, go back in bounds
        if (x < size) x = size;
        else if (x > 1000-size) x = 1000-size;
        if (y < size) y = size;
        else if (y > 1000-size) y = 1000-size;
    }
}

void Player::shoot(std::vector<Bullet*> &bullets) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (shootTimer > 0.2) {
            bullets.push_back(new Bullet(x, y, angle, 10, 10, true, false));
            shootTimer = 0;
        }
    }
}

void Player::draw(sf::RenderWindow &window) {
    int height = size*24/19;
    int width = size;
    sf::VertexArray player_left_part(sf::Triangles, 3);
    sf::VertexArray player_right_part(sf::Triangles, 3);
    float angle_point_triangle_1 = atan2(height, width);
    float angle_point_triangle_2 = atan2(height, -width);
    float distance_point_triangle = sqrt(height * height + width * width);

    ////Define the coordonate of the player's point that are aline with the mouse
    float player_sprite_down_x = x - height * 0.6 * cos(angle);
    float player_sprite_down_y = y + height * 0.6 * sin(angle);

    float player_sprite_up_x = x + height * cos(angle);
    float player_sprite_up_y = y - height * sin(angle);

    float angle_triangle_left = angle_point_triangle_1 + angle + M_PI / 2;
    float player_sprite_left_x = x + distance_point_triangle * cos(angle_triangle_left);
    float player_sprite_left_y = y - distance_point_triangle * sin(angle_triangle_left);

    float angle_triangle_right = angle_point_triangle_2 + angle + M_PI / 2;
    float player_sprite_right_x = x + distance_point_triangle * cos(angle_triangle_right);
    float player_sprite_right_y = y - distance_point_triangle * sin(angle_triangle_right);

    ////Define the position of the left part points
    player_left_part[0].position = sf::Vector2f(player_sprite_left_x, player_sprite_left_y);
    player_left_part[1].position = sf::Vector2f(player_sprite_up_x, player_sprite_up_y);
    player_left_part[2].position = sf::Vector2f(player_sprite_down_x, player_sprite_down_y);

    ////Define the color of the left part
    for(unsigned int i = 0; i < 3; i++) player_left_part[i].color = sf::Color::White;

    ////Define the position of right part points
    player_right_part[0].position = sf::Vector2f(player_sprite_right_x, player_sprite_right_y);
    player_right_part[1].position = sf::Vector2f(player_sprite_up_x, player_sprite_up_y);
    player_right_part[2].position = sf::Vector2f(player_sprite_down_x, player_sprite_down_y);

    ////Define the color of the right part
    for(unsigned int i = 0; i < 3; i++) player_right_part[i].color = sf::Color::White;

    window.draw(player_left_part);
    window.draw(player_right_part);
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
    int diffX, diffY, hitBoxBoth;
    for(auto bullet = bullets.begin(); bullet != bullets.end();){
        diffX = x - (*bullet)->getX();
        diffY = y - (*bullet)->getY();
        hitBoxBoth = size+(*bullet)->getHitBoxRadius();
        if (diffX * diffX + diffY * diffY < hitBoxBoth*hitBoxBoth) {
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
    float diffX, diffY, hitBoxBoth;
    for(Enemy* enemy : enemies){
        diffX = x - enemy->getX();
        diffY = y - enemy->getY();
        hitBoxBoth = size+enemy->getSize();
        if (diffX * diffX + diffY * diffY < hitBoxBoth * hitBoxBoth) receiveDamage(1);
    }
}

void Player::receiveDamage(unsigned int damage){
    if(hitTimer > 2){
        hp -= damage;
        hitTimer = 0;
    }
}