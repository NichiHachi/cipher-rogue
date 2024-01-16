#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"
#include "EnemyStats.h"

#pragma once
class EnemySeeker : public Enemy{
    public :
        static EnemyStats stats;

        EnemySeeker(float x, float y, float angleSpawn);
        void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) override;
        void draw(sf::RenderWindow& window) override;

        void move(float targetAngle, std::vector<Wall> walls, std::vector<Enemy*> enemies);

    private:
};