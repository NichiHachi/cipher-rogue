#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"

class EnemyCharger : public Enemy{
    public:
        EnemyCharger(float x, float y);
        void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls) override;
        void draw(sf::RenderWindow& window) override;
        bool getShot(std::vector<Bullet*>& bullets) override;
        float getX() override {return x;};
        float getY() override {return y;};

        void move(std::vector<Wall> walls);
        void drawWarningZone(sf::RenderWindow& window);

    private:
        float x,y,speed,angle,shootTimer;
        int hp;
};