#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>

#include "../Position.h"
#include "Enemy.h"
#include "../Projectile/Bullet.h"
#include "../Wall.h"
#include "EnemyStats.h"
#include "../Player/Player.h"

class EnemySeeker : public Enemy{
    public :
        static EnemyStats stats;

        EnemySeeker(Position position, float angleSpawn);
        void update(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, Player player, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime) override;
        void draw(sf::RenderWindow& window) override;
        void drawEffects(sf::RenderWindow& window) override;
        std::string getType() override { return "Seeker"; };

    private:
        void move(Position target, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime);
};