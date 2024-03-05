#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "Position.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"
#include "EnemyStats.h"
#include "Player.h"

class EnemyShooter : public Enemy{
    public:
        static EnemyStats stats;

        EnemyShooter(Position position);
        void update(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, Player player, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies, float deltaTime) override;
        void draw(sf::RenderWindow& window) override;
        void drawEffects(sf::RenderWindow& window) override;
        std::string getType() override { return "Shooter"; };

        void shoot(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets);
        void move(float targetAngle, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies);

    private:
};