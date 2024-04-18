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

class EnemyBomber : public Enemy{
    public:
        static EnemyStats stats;

        /**
         * @brief Enemybomber class represents an enemy that shoots projectiles.
         * 
         * This class inherits from the Enemy class and provides additional functionality
         * for shooting projectiles at the player.
         * 
         * @param position The initial position of the enemy.
         */
        explicit EnemyBomber(Position position);
        /**
         * @brief Updates the enemy bomber's state based on the current game conditions.
         * 
         * This function updates the enemy bomber's position, behavior, and interactions with other game objects.
         * 
         * @param bullets A shared pointer to a vector of unique pointers to Bullet objects.
         * @param player The player object.
         * @param walls A shared pointer to a vector of unique pointers to Wall objects.
         * @param enemies A shared pointer to a vector of unique pointers to Enemy objects.
         * @param deltaTime The time elapsed since the last update.
         * @param bombshells A shared pointer to a vector of unique pointers to Bombshell objects.
         */
        void update(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, Player player, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells) override;
        /**
         * @brief Draws the enemy bomber on the specified window.
         * 
         * @param window The SFML render window to draw on.
         */
        void draw(sf::RenderWindow& window) override;
        /**
         * @brief Draws the effects of the enemy bomber on the specified window.
         * 
         * @param window The SFML RenderWindow to draw the effects on.
         */
        void drawEffects(sf::RenderWindow& window) override;
        /**
         * @brief Returns the type of the enemy bomber.
         * 
         * @return The type of the enemy bomber as a string.
         */
        std::string getType() override { return "bomber"; };

    private:
        /**
         * @brief Shoots bombshells.
         *
         * This function is responsible for shooting bombshells from the enemy bomber.
         *
         * @param bullets A shared pointer to a vector of unique pointers to Bullet objects.
         */
        void shoot(const Position target, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells);
        /**
         * Moves the enemy bomber based on the target angle, walls, enemies, and delta time.
         * 
         * @param targetAngle The target angle to move towards.
         * @param walls A shared pointer to a vector of unique pointers to Wall objects.
         * @param enemies A shared pointer to a vector of unique pointers to Enemy objects.
         * @param deltaTime The time elapsed since the last frame.
         */
        void move(const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime);
};