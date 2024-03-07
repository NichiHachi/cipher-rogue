#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <string>

#include "../Position.h"
#include "../Projectile/Bullet.h"
#include "../Projectile/Bombshell.h"
#include "../Wall.h"
#include "../Player/Player.h"

class Player;
class Bombshell;

class Enemy{
    public:
        virtual void update(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, Player player, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies, float deltaTime)=0;
        virtual void draw(sf::RenderWindow& window)=0;
        virtual void drawEffects(sf::RenderWindow& window)=0;
        virtual std::string getType()=0;
        virtual ~Enemy(){};

        void receiveDamageIfShot(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>> bombshells);
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
        bool adjustPositionBasedOnEnemies(std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies);
        bool adjustPositionBasedOnWalls(std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls);
        bool adjustPositionBasedOnOOB();
        float getAngleToObject(Position objectPosition);
        float getAngleToFuturPlayerPosition(Player player);
        void smoothTurn(float targetAngle, float turnSpeedFactor, float deltaTime);
        Position pathFinding(Position target, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls);
    
    private:
        bool hasLineOfSight(Position target, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls);
};