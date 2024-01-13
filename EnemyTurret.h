#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"

class EnemyTurret : public Enemy{
    public:
        EnemyTurret(float x, float y);
        void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls);
        void draw(sf::RenderWindow& window);

        void move(std::vector<Wall> walls);
        void shoot(std::vector<Bullet*> &bullets);

    private:
        float x,y,speed,angle,shootTimer;
        int hp;
};