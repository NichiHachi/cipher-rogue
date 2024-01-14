#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"

class EnemySpawner : public Enemy{
    public :
        EnemySpawner(float x, float y, float sizeFactor, float bulletSizeFactor);
        void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies);
        void draw(sf::RenderWindow& window);
        bool getShot(std::vector<Bullet*>& bullets) override;
        float getX() override {return x;};
        float getY() override {return y;};

    private:
        float x,y,angle,speed,shootTimer,bulletSizeFactor;
        int hp,size;
};