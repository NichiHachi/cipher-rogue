#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"

class EnemySniper : public Enemy{
    public:
        EnemySniper(float x, float y);
        void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls);
        void draw(sf::RenderWindow& window);
        bool getShot(std::vector<Bullet*>& bullets) override;
        float getX() override {return x;};
        float getY() override {return y;};

        void shoot(std::vector<Bullet*> &bullets);
        void move(float targetAngle, std::vector<Wall> walls);

    private:
        float x,y,speed,angle,shootTimer;
        int hp;
};