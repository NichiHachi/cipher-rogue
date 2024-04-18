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

class EnemySpawner : public Enemy{
    public :
        static EnemyStats stats;

        /**
         * @brief EnemySpawner class responsible for spawning enemies at a given position.
         */
        explicit EnemySpawner(Position position);
        /**
         * @brief Updates the enemy spawner.
         * 
         * This function updates the enemy spawner by spawning new enemies, checking for collisions with bullets, walls, and the player,
         * and updating the positions and states of the enemies.
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
         * @brief Draws the enemy spawner on the specified window.
         * 
         * @param window The SFML render window to draw on.
         */
        void draw(sf::RenderWindow& window) override;
        /**
         * @brief Draws the effects of the enemy spawner on the specified window.
         * 
         * @param window The SFML render window to draw the effects on.
         */
        void drawEffects(sf::RenderWindow& window) override;
        /**
         * @brief Returns the type of the enemy spawner.
         * 
         * @return The type of the enemy spawner as a string.
         */
        std::string getType() override { return "Spawner"; };

    private:
    /**
     * @brief Draws the sprite of the enemy on the specified window.
     * 
     * @param window The SFML RenderWindow to draw the sprite on.
     * @param sizeFactor The factor by which to scale the sprite size.
     * @param color The color to apply to the sprite.
     */
       void drawSprite(sf::RenderWindow &window, float sizeFactor, sf::Color color);
};