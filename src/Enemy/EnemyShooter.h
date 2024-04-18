#pragma once

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

        /**
         * @brief EnemyShooter class represents an enemy that shoots projectiles.
         * 
         * This class inherits from the Enemy class and provides additional functionality
         * for shooting projectiles at the player.
         * 
         * @param position The initial position of the enemy.
         */
        explicit EnemyShooter(Position position);
        /**
         * @brief Updates the enemy shooter's state based on the current game conditions.
         * 
         * This function updates the enemy shooter's position, behavior, and interactions with other game objects.
         * 
         * @param bullets A shared pointer to a vector of unique pointers to Bullet objects.
         * @param player The player object.
         * @param walls A shared pointer to a vector of unique pointers to Wall objects.
         * @param enemies A shared pointer to a vector of unique pointers to Enemy objects.
         * @param deltaTime The time elapsed since the last update.
         */
        void update(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, Player player, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime) override;
        /**
         * @brief Draws the enemy shooter on the specified window.
         * 
         * @param window The SFML render window to draw on.
         */
        void draw(sf::RenderWindow& window) override;
        /**
         * @brief Draws the effects of the enemy shooter on the specified window.
         * 
         * @param window The SFML RenderWindow to draw the effects on.
         */
        void drawEffects(sf::RenderWindow& window) override;
        /**
         * @brief Returns the type of the enemy shooter.
         * 
         * @return The type of the enemy shooter as a string.
         */
        std::string getType() override { return "Shooter"; };

    private:
        /**
         * @brief Shoots bullets.
         *
         * This function is responsible for shooting bullets from the enemy shooter.
         *
         * @param bullets A shared pointer to a vector of unique pointers to Bullet objects.
         */
        void shoot(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets);
        /**
         * Moves the enemy shooter based on the target angle, walls, enemies, and delta time.
         * 
         * @param targetAngle The target angle to move towards.
         * @param walls A shared pointer to a vector of unique pointers to Wall objects.
         * @param enemies A shared pointer to a vector of unique pointers to Enemy objects.
         * @param deltaTime The time elapsed since the last frame.
         */
        void move(float targetAngle, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime);
        /**
         * @brief Draws the sprite of the enemy shooter.
         * 
         * @param window The SFML render window to draw the sprite on.
         * @param sizeFactor The factor to scale the size of the sprite.
         * @param color The color to apply to the sprite.
         */
        void drawSprite(sf::RenderWindow &window, float sizeFactor, sf::Color color);
};