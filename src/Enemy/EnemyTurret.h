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

class EnemyTurret : public Enemy{
    public:
        static EnemyStats stats;

        /**
         * @brief Represents an enemy turret.
         * 
         * This class is responsible for creating and managing enemy turrets in the game.
         * It provides functionality to set the position of the turret.
         * 
         * @param position The initial position of the turret.
         */
        explicit EnemyTurret(Position position);
        /**
         * @brief Updates the enemy turret's state based on the current game conditions.
         * 
         * This function is called to update the enemy turret's position, behavior, and interactions with other game objects.
         * 
         * @param bullets A shared pointer to a vector of unique pointers to Bullet objects.
         * @param player The player object.
         * @param walls A shared pointer to a vector of unique pointers to Wall objects.
         * @param enemies A shared pointer to a vector of unique pointers to Enemy objects.
         * @param deltaTime The time elapsed since the last update.
         */
        void update(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, Player player, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells) override;
        /**
         * @brief Draws the enemy turret on the specified window.
         * 
         * @param window The SFML render window to draw the turret on.
         */
        void draw(sf::RenderWindow& window) override;
        /**
         * @brief Draws the effects of the enemy turret on the specified window.
         *
         * This function is responsible for rendering any visual effects associated with the enemy turret,
         * such as particle effects or animations. The effects are drawn on the specified SFML window.
         *
         * @param window The SFML window on which to draw the effects.
         */
        void drawEffects(sf::RenderWindow& window) override;
        /**
         * @brief Returns the type of the enemy turret.
         * 
         * @return The type of the enemy turret as a string.
         */
        std::string getType() override { return "Turret"; };

        /**
         * @brief Shoots bullets from the enemy turret.
         *
         * This function is responsible for shooting bullets from the enemy turret.
         * It takes a shared pointer to a vector of unique pointers to Bullet objects as a parameter.
         * The bullets fired by the turret will be added to this vector.
         *
         * @param bullets A shared pointer to a vector of unique pointers to Bullet objects.
         */
        void shoot(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets);

    private:
        /**
         * @brief Draws the sprite of the enemy turret on the specified window.
         * 
         * @param window The SFML render window to draw the sprite on.
         * @param sizeFactor The scaling factor for the sprite size.
         * @param color The color to apply to the sprite.
         */
        void drawSprite(sf::RenderWindow &window, float sizeFactor, sf::Color color);
};