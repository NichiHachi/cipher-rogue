#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"
#include "EnemyStats.h"

class EnemyTurret : public Enemy{
    public:
        static EnemyStats stats;

        EnemyTurret(float x, float y);
        void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) override;
        void draw(sf::RenderWindow& window) override;

        void move(std::vector<Wall> walls);
        void shoot(std::vector<Bullet*> &bullets);

    private:
        float x,y,speed,angle,shootTimer,bulletSizeFactor;
        int hp,size;
        bool movable;
};