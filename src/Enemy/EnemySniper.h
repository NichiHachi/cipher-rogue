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

class EnemySniper : public Enemy{
    public:
        static EnemyStats stats;

        /**
         * @brief Represents an enemy sniper.
         * 
         * The EnemySniper class is a derived class of the Enemy class. It represents an enemy character
         * that specializes in long-range attacks using a sniper rifle.
         * 
         * @param position The initial position of the enemy sniper.
         */
        explicit EnemySniper(Position position);
        /**
         * @brief Updates the state of the enemy sniper.
         * 
         * This function updates the enemy sniper's behavior based on the current game state.
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
         * @brief Draws the enemy sniper on the specified SFML window.
         * 
         * @param window The SFML window to draw the enemy sniper on.
         */
        void draw(sf::RenderWindow& window) override;
        /**
         * @brief Draws the special effects of the enemy sniper on the given SFML window.
         *
         * This function is responsible for rendering any special effects associated with the enemy sniper,
         * such as particle effects or animations. The effects are drawn on the provided SFML window.
         *
         * @param window The SFML window on which to draw the effects.
         */
        void drawEffects(sf::RenderWindow& window) override;
        /**
         * @brief Returns the type of the enemy as a string.
         * 
         * @return The type of the enemy as a string.
         */
        std::string getType() override { return "Sniper"; };

    private:
        /**
         * @brief Shoots bullets.
         *
         * This function is responsible for shooting bullets. It takes a shared pointer to a vector of unique pointers to Bullet objects as a parameter.
         *
         * @param bullets A shared pointer to a vector of unique pointers to Bullet objects.
         */
        void shoot(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets);
        /**
         * Moves the enemy sniper towards the specified target angle.
         * 
         * @param targetAngle The target angle to move towards.
         * @param walls A shared pointer to a vector of unique pointers to Wall objects.
         * @param enemies A shared pointer to a vector of unique pointers to Enemy objects.
         * @param deltaTime The time elapsed since the last frame.
         */
        void move(float targetAngle, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime) ;
        /**
         * @brief Draws the sprite of the enemy sniper on the given window.
         * 
         * @param window The SFML render window to draw the sprite on.
         * @param sizeFactor The factor by which to scale the sprite size.
         * @param color The color to apply to the sprite.
         */
        void drawSprite(sf::RenderWindow &window, float sizeFactor, sf::Color color);
};