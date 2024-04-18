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

class EnemyCharger : public Enemy{
    public:
        static EnemyStats stats;

        /**
         * @brief Represents an enemy charger.
         * 
         * The EnemyCharger class is responsible for defining the behavior and attributes of an enemy charger in the game.
         * It inherits from the Enemy class and represents a type of enemy that charges towards the player.
         * 
         * @param position The initial position of the enemy charger.
         */
        explicit EnemyCharger(Position position);
        
        /**
         * @brief Updates the state of the EnemyCharger.
         * 
         * This function updates the position and behavior of the EnemyCharger based on the current game state.
         * It takes into account the player's position, the bullets on the screen, the walls, and other enemies.
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
         * @brief Draws the EnemyCharger object on the specified window.
         * 
         * @param window The SFML RenderWindow to draw the object on.
         */
        void draw(sf::RenderWindow& window) override;
        /**
         * @brief Draws the effects of the enemy charger on the specified window.
         * 
         * @param window The SFML render window to draw the effects on.
         */
        void drawEffects(sf::RenderWindow& window) override;
        /**
         * @brief Get the type of the enemy.
         * 
         * @return std::string The type of the enemy.
         */
        std::string getType() override { return "Charger"; };

        /**
         * @brief Draws the warning zone for the enemy charger.
         * 
         * This function is responsible for drawing the warning zone around the enemy charger.
         * The warning zone indicates the area where the enemy charger will attack the player.
         * 
         * @param window The SFML render window to draw on.
         */
        void drawWarningZone(sf::RenderWindow& window);

    private:
        /**
         * Moves the enemy charger based on the given walls and deltaTime.
         *
         * @param walls A shared pointer to a vector of unique pointers to Wall objects.
         * @param deltaTime The time elapsed since the last frame.
         */
        void move(const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, float deltaTime);
        /**
         * @brief Draws the sprite of the enemy charger.
         * 
         * @param window The SFML render window to draw the sprite on.
         * @param sizeFactor The factor to scale the sprite size.
         * @param color The color to apply to the sprite.
         */
        void drawSprite(sf::RenderWindow &window, float sizeFactor, sf::Color color);
};