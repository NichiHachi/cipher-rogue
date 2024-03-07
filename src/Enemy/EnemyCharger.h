#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "../Position.h"
#include "Enemy.h"
#include "../Projectile/Bullet.h"
#include "../Wall.h"
#include "EnemyStats.h"
#include "../Player/Player.h"

class EnemyCharger : public Enemy{
    public:
        static EnemyStats stats;

        EnemyCharger(Position position);
        void update(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, Player player, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies, float deltaTime) override;
        void draw(sf::RenderWindow& window) override;
        void drawEffects(sf::RenderWindow& window) override;
        std::string getType() override { return "Charger"; };

        void drawWarningZone(sf::RenderWindow& window);

    private:
        void move(std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, float deltaTime);
};