#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "Enemy.h"
#include "EnemyStats.h"
#include "../Position.h"
#include "../Wall.h"
#include "../Projectile/Bullet.h"
#include "../Player/Player.h"

class EnemyShooter : public Enemy{
    public:
        static EnemyStats stats;

        explicit EnemyShooter(Position position);
        void update(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, Player player, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime) override;
        void draw(sf::RenderWindow& window) override;
        void drawEffects(sf::RenderWindow& window) override;
        std::string getType() override { return "Shooter"; };

    private:
        void shoot(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets);
        void move(float targetAngle, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime);
        void drawSprite(sf::RenderWindow &window, float sizeFactor, sf::Color color);
};