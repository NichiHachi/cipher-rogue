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
        bool getShot(std::vector<Bullet*>& bullets) override;
        float getX() override {return x;};
        float getY() override {return y;};

        void shoot(std::vector<Bullet*> &bullets);
        void move(float targetAngle, std::vector<Wall> walls);

    private:
        float x,y,speed,angle,shootTimer,bulletSizeFactor;
        int hp,size;
};