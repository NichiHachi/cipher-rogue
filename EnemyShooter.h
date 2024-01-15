#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"

class EnemyShooter : public Enemy{
    public:
        EnemyShooter(float x, float y, float sizeFactor, float bulletSizeFactor);
        void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) override;
        void draw(sf::RenderWindow& window) override;
        bool receiveDamageIfShot(std::vector<Bullet*>& bullets) override;
        float getX() override {return x;};
        float getY() override {return y;};
        int getSize() override {return size;};
        bool isMovable() override {return movable;};
        void setCoordonates(float x, float y) override {this->x = x; this->y = y;};

        void shoot(std::vector<Bullet*> &bullets);
        void move(float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies);

    private:
        float x,y,speed,angle,shootTimer,bulletSizeFactor;
        int hp,size;
        bool movable;
};