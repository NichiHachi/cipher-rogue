#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "Position.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"
#include "EnemyStats.h"

class EnemyTurret : public Enemy{
    public:
        static EnemyStats stats;

        EnemyTurret(Position position);
        void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls, std::vector<Enemy*>& enemies) override;
        void draw(sf::RenderWindow& window) override;
        void drawEffects(sf::RenderWindow& window) override;
        std::string getType() override { return "Turret"; };

        void move(std::vector<Wall> walls);
        void shoot(std::vector<Bullet*> &bullets);

    private:
};