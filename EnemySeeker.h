#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"

#pragma once
class EnemySeeker : public Enemy{
    public :
        EnemySeeker(float x, float y, float angle, float sizeFactor);
        void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) override;
        void draw(sf::RenderWindow& window) override;
        bool getShot(std::vector<Bullet*>& bullets) override;
        float getX() override {return x;};
        float getY() override {return y;};

        void move(float targetAngle, std::vector<Wall> walls, std::vector<Enemy*> enemies);

    private:
        float x,y,angle,speed;
        int hp,size;
};