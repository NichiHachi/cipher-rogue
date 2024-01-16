#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"
#include "EnemyStats.h"

class EnemyCharger : public Enemy{
    public:
        static EnemyStats stats;

        EnemyCharger(float x, float y);
        void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) override;
        void draw(sf::RenderWindow& window) override;

        void move(std::vector<Wall> walls);
        void drawWarningZone(sf::RenderWindow& window);

    private:
};