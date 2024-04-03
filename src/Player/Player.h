#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <memory>

#include "../Position.h"
#include "../Projectile/Bullet.h"
#include "../Projectile/Bombshell.h"
#include "../Wall.h"
#include "PlayerStats.h"

class Enemy;
class Bombshell;

class Player{
    public:
        static PlayerStats stats;

        Player();
        void update(sf::RenderWindow &window, const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells,
                    const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, float deltaTime);
        void spawn();
        void shoot(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells, Position positionTarget);
        void receiveDamageIfShot(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells);
        void receiveDamageIfHit(const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies);
        void receiveDamage(int damage);
        void draw(sf::RenderWindow& window);
        void drawHealth(sf::RenderWindow& window) const;

        Position getPosition() const;
        float getSpeed() const;
        
    private:
        Position position;
        float speed, shootTimer, hitTimer, speedBullet;
        int size, hp, hpMax;
        double angle;

        void move(const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, float deltaTime);
};