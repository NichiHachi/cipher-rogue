#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include "Projectile/Bullet.h"
#include "Projectile/Bombshell.h"
#include "Player/Player.h"
#include "Wall.h"
#include "Enemy/Enemy.h"

/**
 * @brief The Game class represents the game logic and state.
 * 
 * It manages the player, bullets, bombshells, walls, enemies, and the game's terminal messages.
 * The Game class provides methods for updating the game state, drawing the game, initializing levels,
 * and checking if the player is dead. It also includes various helper methods for managing game elements.
 */
class Game{
    public:
        Player player;
        
        std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bulletsEnemy;
        std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bulletsAlly;
        std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>> bombshells;
        std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls;
        std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies;
        std::vector<std::string> messageTerminal;

        /**
         * @brief Constructs a new Game object.
         */
        Game();

        /**
         * @brief Destroys the Game object.
         */
        ~Game();

        /**
         * @brief Updates the game state.
         * 
         * @param window The SFML render window.
         * @param deltaTime The time elapsed since the last update.
         */
        void update(sf::RenderWindow& window, float deltaTime);

        /**
         * @brief Draws the game.
         * 
         * @param window The SFML render window.
         * @param deltaTime The time elapsed since the last draw.
         */
        void draw(sf::RenderWindow& window, float deltaTime);

        /**
         * @brief Checks if the player is dead.
         * 
         * @return true if the player is dead, false otherwise.
         */
        bool isPlayerDead() const;

        /**
         * @brief Initializes a new level.
         */
        void initLevel();

        /**
         * @brief Return the level achieved by the player.
         * 
         * @return The level achieved by the player.
        */
        int levelAchieved() const;
    
    private:
        std::string displayedMessage;
        float displayedMessageTimer = 0;
        float cursorTimer = 0;
        unsigned int level;
        bool cursorVisible = true;
        bool levelStarted;
        int enemySpawnCost;
        double startTime;
        int mapSelectionHistory[10];
        sf::Text text;
        sf::Font font;

        /**
         * @brief Draws the fake terminal display.
         * 
         * @param window The SFML render window.
         * @param deltaTime The time elapsed since the last draw.
         */
        void drawFakeTerminal(sf::RenderWindow& window, float deltaTime);

        /**
         * @brief Draws the terminal cursor.
         * 
         * @param window The SFML render window.
         * @param deltaTime The time elapsed since the last draw.
         */
        void drawCursorTerminal(sf::RenderWindow& window, float deltaTime);

        /**
         * @brief Destroys walls that have been hit by bullets.
         */
        void destroyWalls();

        /**
         * @brief Handles collisions between bullets.
         */
        void bulletCollisions();

        /**
         * @brief Checks if bullets collide with bombshells.
         */
        void checkBulletsCollisionWithBombshells();

        /**
         * @brief Checks if bullets are out of bounds.
         */
        void checkBulletsOOB();

        /**
         * @brief Checks for collisions between ally and enemy bullets.
         */
        void checkBulletAllyCollisionBulletEnemy();

        /**
         * @brief Selects a map for the level.
         * 
         * @return The selected map index.
         */
        int selectMap();

        /**
         * @brief Puts walls between two positions.
         * 
         * @param from The starting position.
         * @param to The ending position.
         */
        void putWallFromTo(Position from, Position to);

        /**
         * @brief Fills walls between two positions.
         * 
         * @param from The starting position.
         * @param to The ending position.
         */
        void fillWallFromTo(Position from, Position to);

        /**
         * @brief Creates the map for the level.
         */
        void createMap();

        /**
         * @brief Displays a timer.
         * 
         * @param window The SFML render window.
         */
        void displayTimer(sf::RenderWindow& window);

        /**
         * @brief Spawns enemies.
         */
        void spawnEnemy();
};