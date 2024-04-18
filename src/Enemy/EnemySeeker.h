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

        /**
         * @class EnemySeeker
         * @brief Represents an enemy seeker.
         *
         * The EnemySeeker class is responsible for representing an enemy seeker in the game.
         * It inherits from the Enemy class and adds specific behavior for seeking the player.
         *
         * @param position The initial position of the enemy seeker.
         * @param angleSpawn The angle at which the enemy seeker is spawned.
         */
        EnemySeeker(Position position, float angleSpawn);
        /**
         * @brief Updates the state of the EnemySeeker.
         * 
         * This function updates the state of the EnemySeeker based on the current game conditions.
         * It takes into account the bullets, player, walls, enemies, and the time elapsed since the last update.
         * 
         * @param bullets A shared pointer to a vector of unique pointers to Bullet objects.
         * @param player The player object.
         * @param walls A shared pointer to a vector of unique pointers to Wall objects.
         * @param enemies A shared pointer to a vector of unique pointers to Enemy objects.
         * @param deltaTime The time elapsed since the last update.
         */
        void update(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, Player player, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime) override;
        /**
         * @brief Draws the EnemySeeker object on the specified window.
         * 
         * @param window The SFML RenderWindow to draw the object on.
         */
        void draw(sf::RenderWindow& window) override;
        /**
         * @brief Draws the effects of the enemy on the specified window.
         * 
         * This function is responsible for rendering any visual effects associated with the enemy
         * on the given SFML window.
         * 
         * @param window The SFML window to draw the effects on.
         */
        void drawEffects(sf::RenderWindow& window) override;
        /**
         * @brief Get the type of the enemy.
         * 
         * @return std::string The type of the enemy.
         */
        std::string getType() override { return "Seeker"; };

    private:
        /**
         * Moves the enemy towards the specified target position.
         * 
         * @param target The target position to move towards.
         * @param walls A shared pointer to a vector of unique pointers to Wall objects.
         * @param enemies A shared pointer to a vector of unique pointers to Enemy objects.
         * @param deltaTime The time elapsed since the last frame update.
         */
        void move(Position target, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime);
        /**
         * @brief Draws the sprite of the enemy seeker.
         * 
         * This function draws the sprite of the enemy seeker on the specified SFML window.
         * The size of the sprite is scaled by the given sizeFactor.
         * The color parameter specifies the color of the sprite.
         * 
         * @param window The SFML window to draw the sprite on.
         * @param sizeFactor The factor by which to scale the size of the sprite.
         * @param color The color of the sprite.
         */
        void drawSprite(sf::RenderWindow &window, float sizeFactor, sf::Color color);
};