#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <memory>

#include "../Position.h"
#include "../Projectile/Bullet.h"
#include "../Projectile/Bombshell.h"
#include "../Wall.h"
#include "PlayerStats.h"

class Enemy;
class Bombshell;

/**
 * @brief The Player class represents the player character in the game.
 * 
 * It contains functions for updating the player's state, handling player actions,
 * receiving damage, and drawing the player on the game window.
 */
class Player{
    public:
        static PlayerStats stats; /**< The static instance of PlayerStats for the player. */

        /**
         * @brief Constructs a new Player object.
         */
        Player();

        /**
         * @brief Updates the player's state based on the given parameters.
         * 
         * @param window The game window.
         * @param bullets The vector of bullets.
         * @param bombshells The vector of bombshells.
         * @param walls The vector of walls.
         * @param deltaTime The time elapsed since the last update.
         */
        void update(sf::RenderWindow &window, const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells,
                    const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, float deltaTime);

        /**
         * @brief Spawns the player at a random position.
         */
        void spawn();

        /**
         * @brief Shoots a bullet or bombshell towards the given target position.
         * 
         * @param bullets The vector of bullets.
         * @param bombshells The vector of bombshells.
         * @param positionTarget The target position to shoot at.
         */
        void shoot(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells, Position positionTarget);

        /**
         * @brief Checks if the player has been shot by a bullet or bombshell and receives damage if true.
         * 
         * @param bullets The vector of bullets.
         * @param bombshells The vector of bombshells.
         */
        void receiveDamageIfShot(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells);

        /**
         * @brief Checks if the player has been hit by an enemy and receives damage if true.
         * 
         * @param enemies The vector of enemies.
         */
        void receiveDamageIfHit(const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies);

        /**
         * @brief Receives damage and reduces the player's health points.
         * 
         * @param damage The amount of damage to receive.
         */
        void receiveDamage(int damage);

        /**
         * @brief Draws the player on the game window.
         * 
         * @param window The game window.
         */
        void draw(sf::RenderWindow& window);

        /**
         * @brief Draws the player's health bar on the game window.
         * 
         * @param window The game window.
         */
        void drawHealth(sf::RenderWindow& window) const;

        /**
         * @brief Checks if the player is dead.
         * 
         * @return true if the player is dead, false otherwise.
         */
        bool isDead() const;

        /**
         * @brief Gets the position of the player.
         * 
         * @return The position of the player.
         */
        Position getPosition() const;

        /**
         * @brief Gets the speed of the player.
         * 
         * @return The speed of the player.
         */
        float getSpeed() const;
        
    private:
        Position position;
        float speed;
        float shootTimer;
        float hitTimer;
        float speedBullet;
        int size;
        int hp;
        int hpMax;
        double angle;

        /**
         * @brief Moves the player based on the given parameters.
         * 
         * @param walls The vector of walls.
         * @param deltaTime The time elapsed since the last update.
         */
        void move(const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, float deltaTime);
};