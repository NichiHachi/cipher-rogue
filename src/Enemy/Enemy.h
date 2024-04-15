#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <string>

#include "../Position.h"
#include "../Projectile/Bullet.h"
#include "../Projectile/Bombshell.h"
#include "../Wall.h"
#include "../Player/Player.h"

class Player;
class Bombshell;

/**
 * @class Enemy
 * @brief Base class for enemy entities in the game.
 * 
 * The Enemy class provides an interface and common functionality for all enemy entities in the game.
 * It defines pure virtual functions for updating and drawing the enemy, as well as retrieving its type.
 * It also provides common member functions for receiving damage, setting position, and retrieving information about the enemy.
 * Derived classes must implement the pure virtual functions to define specific behavior for each enemy type.
 */
class Enemy{
    public:
        /**
         * @brief Updates the enemy's state based on the current game conditions.
         * 
         * This function is called to update the enemy's position, behavior, and any other relevant properties.
         * It takes into account the bullets fired by the player, the player's position, the walls in the game,
         * and the other enemies present in the game. The update is performed based on the specified time interval.
         * 
         * @param bullets A shared pointer to a vector of unique pointers to Bullet objects representing the bullets fired by the player.
         * @param player The player object representing the player's position and state.
         * @param walls A shared pointer to a vector of unique pointers to Wall objects representing the walls in the game.
         * @param enemies A shared pointer to a vector of unique pointers to Enemy objects representing the other enemies in the game.
         * @param deltaTime The time interval between the current and previous update calls.
         */
        virtual void update(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, Player player, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies, float deltaTime)=0;
        
        /**
         * @brief Draws the enemy on the specified SFML window.
         * 
         * This is a pure virtual function that must be implemented by derived classes.
         * It is responsible for rendering the enemy on the game window.
         * 
         * @param window The SFML window on which to draw the enemy.
         */
        virtual void draw(sf::RenderWindow& window)=0;

        /**
         * @brief Draws the effects of the enemy on the specified SFML window.
         * 
         * This function is a pure virtual function that must be implemented by derived classes.
         * It is responsible for drawing any visual effects associated with the enemy on the SFML window.
         * 
         * @param window The SFML window on which to draw the effects.
         */
        virtual void drawEffects(sf::RenderWindow& window)=0;

        /**
         * @brief Returns the type of the enemy.
         * 
         * @return std::string The type of the enemy.
         */
        virtual std::string getType()=0;

        /**
         * @brief Destructor for the Enemy class.
         */
        virtual ~Enemy(){};

        /**
         * @brief Applies damage to the enemy if hit by bullets or bombshells.
         * 
         * @param bullets A shared pointer to a vector of unique pointers to Bullet objects.
         * @param bombshells A shared pointer to a vector of unique pointers to Bombshell objects.
         */
        void receiveDamageIfShot(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells);
        
        /**
         * @brief Sets the position of the enemy.
         * 
         * @param newPosition The new position to set.
         */
        void setPosition(Position newPosition);

        /**
         * @brief Applies damage to the enemy.
         * 
         * This function is called to apply damage to the enemy. The amount of damage
         * is specified by the 'damage' parameter.
         * 
         * @param damage The amount of damage to be applied to the enemy.
         */
        void receiveDamage(int damage);

        /**
         * @brief Get the position of the enemy.
         * 
         * @return Position The position of the enemy.
         */
        Position getPosition() const;


        /**
         * @brief Get the size of the enemy.
         * 
         * @return int The size of the enemy.
         */
        int getSize() const;

        /**
         * @brief Checks if the enemy is movable.
         * 
         * @return true if the enemy is movable, false otherwise.
         */
        bool isMovable() const;

        /**
         * @brief Checks if the enemy is dead.
         * 
         * @return true if the enemy is dead, false otherwise.
         */
        bool isDead() const;

        /**
         * @brief Gets the speed of the enemy's bullets.
         * 
         * @return The speed of the enemy's bullets.
         */
        float getSpeedBullet() const;

        /**
         * @brief Get the current HP of the enemy.
         * 
         * @return int The current HP of the enemy.
         */
        int getHp() const;   

        /**
         * Adjusts the position of the enemy based on the positions of other enemies.
         * 
         * @param enemies A shared pointer to a vector of unique pointers to Enemy objects.
         * @return True if the position was adjusted successfully, false otherwise.
         */
        bool adjustPositionBasedOnEnemies(const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies);
        /**
         * Adjusts the position of the enemy based on the presence of walls.
         *
         * @param walls A shared pointer to a vector of unique pointers to Wall objects.
         * @return True if the enemy's position was successfully adjusted, false otherwise.
         */
        bool adjustPositionBasedOnWalls(const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls);
        /**
         * @brief Adjusts the position of the enemy based on out-of-bounds (OOB) conditions.
         * 
         * This function checks if the enemy's current position is outside the valid bounds of the game world.
         * If the enemy is out of bounds, it adjusts the position to a valid location within the bounds.
         * 
         * @return true if the position was adjusted, false otherwise.
         */
        bool adjustPositionBasedOnOOB();

    protected:
        Position position;
        float speed, angle, shootTimer, speedBullet;
        int hp, size;
        const bool movable;

        /**
         * @brief Represents an enemy in the game.
         * 
         * This class encapsulates the properties and behavior of an enemy character in the game.
         * It includes information such as position, speed, shooting capabilities, health points, and size.
         * 
         * @param position The initial position of the enemy.
         * @param speed The movement speed of the enemy.
         * @param angle The initial angle of the enemy's movement.
         * @param shootTimer The timer for shooting bullets.
         * @param speedBullet The speed of the bullets shot by the enemy.
         * @param hp The health points of the enemy.
         * @param size The size of the enemy.
         * @param movable Indicates whether the enemy is movable or not.
         */
        Enemy(Position position, float speed, float angle, float shootTimer, float speedBullet, int hp, int size, bool movable);
        /**
         * Calculates the angle between the enemy and the target position.
         * 
         * @param target The position of the target.
         * @return The angle between the enemy and the target in radians.
         */
        float getAngleToTarget(Position target) const;
        /**
         * Calculates the angle between the enemy and the future position of the player.
         * 
         * @param player The player object representing the future position.
         * @return The angle in radians between the enemy and the player's future position.
         */
        float getAngleToFuturPlayerPosition(Player player);
        /**
         * Smoothly turns the enemy towards the target angle.
         * 
         * @param targetAngle The target angle to turn towards.
         * @param turnSpeedFactor The speed factor for the turn.
         * @param deltaTime The time elapsed since the last frame.
         */
        void smoothTurn(float targetAngle, float turnSpeedFactor, float deltaTime);
        /**
         * Calculates the path to the target position using a aStar algorithm.
         * 
         * @param target The target position to pathfind to.
         * @param walls A shared pointer to a vector of unique pointers to Wall objects representing the obstacles in the environment.
         * @param deltaTime The time elapsed since the last frame update.
         * @return The calculated angle to turn.
         */
        float pathFinding(Position target, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, float deltaTime);
    
    private:
        /**
         * Checks if the enemy has a line of sight to the target position.
         * 
         * @param target The position to check line of sight to.
         * @param walls A shared pointer to a vector of unique pointers to Wall objects representing the walls in the environment.
         * @return True if the enemy has line of sight to the target position, false otherwise.
         */
        bool hasLineOfSight(Position target, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls);

        /**
         * @brief A* algorithm that calculates the path to reach the target position.
         * 
         * @param target The target position to reach.
         * @param walls A shared pointer to a vector of unique pointers to Wall objects.
         * @return std::vector<Position> The list of positions representing the path to take.
         */
        std::vector<Position> aStar(Position target, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls);
};