#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "Position.h"
#include "Bullet.h"
#include "Wall.h"
#include "Player.h"

#pragma once
class Enemy{
    public:
        virtual void update(std::vector<Bullet*>& bullets, float timePassed, Player player, std::vector<Wall> walls, std::vector<Enemy*>& enemies)=0;
        virtual void draw(sf::RenderWindow& window )=0;
        virtual void drawEffects(sf::RenderWindow& window )=0;
        virtual std::string getType()=0;
        virtual ~Enemy(){};

        void receiveDamageIfShot(std::vector<Bullet*>& bullets);
        void setPosition(Position newPosition) {this->position = newPosition;};
        void receiveDamage(int damage) {hp -= damage;};

        Position getPosition() const { return position; }
        int getSize() const {return size;};
        bool isMovable() const {return movable;};
        bool isDead() const {return hp <= 0;};
        float getSpeedBullet() const {return speedBullet;};     

    protected:
        Position position;
        float speed, angle, shootTimer, speedBullet;
        int hp, size;
        const bool movable;

        Enemy(Position position, float speed, float angle, float shootTimer, float speedBullet, int hp, int size, bool movable);
        bool adjustPositionBasedOnEnemies(std::vector<Enemy*>& enemies);
        bool adjustPositionBasedOnWalls(std::vector<Wall> walls);
        bool adjustPositionBasedOnOOB();
        float getAngleToObject(Position objectPosition);
        float getAngleToFuturPlayerPosition(Player player);
};