#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"
#include "EnemyStats.h"

class EnemySniper : public Enemy{
    public:
        static EnemyStats stats;

        EnemySniper(float x, float y);
        void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) override;
        void draw(sf::RenderWindow& window) override;

        void shoot(std::vector<Bullet*> &bullets);
        void move(float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies);

    private:
};