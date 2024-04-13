#include <vector>
#include <memory>
#include <string>
#include <unordered_map>

#include "Projectile/Bullet.h"
#include "Projectile/Bombshell.h"
#include "Player/Player.h"
#include "Wall.h"
#include "Enemy/Enemy.h"

class Game{
    public:
        Player player;
        
        std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bulletsEnemy;
        std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bulletsAlly;
        std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>> bombshells;
        std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls;
        std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies;
        std::vector<std::string> messageTerminal;

        Game();
        ~Game();

        void update(sf::RenderWindow& window, float deltaTime);
        void draw(sf::RenderWindow& window, float deltaTime);
        void initLevel();
    
    private:
        std::string displayedMessage;
        float displayedMessageTimer = 0;
        float cursorTimer = 0;
        unsigned int level;
        bool cursorVisible = true;
        bool levelStarted;
        double startTime;
        int mapSelectionHistory[10];
        sf::Text text;
        sf::Font font;

        void drawFakeTerminal(sf::RenderWindow& window, float deltaTime);
        void drawCursorTerminal(sf::RenderWindow& window, float deltaTime);
        void destroyWalls();
        void bulletCollisions();
        void checkBulletsCollisionWithBombshells();
        void checkBulletsOOB();
        void checkBulletAllyCollisionBulletEnemy();
        int selectMap();
        void putWallFromTo(Position from, Position to);
        void fillWallFromTo(Position from, Position to);
        void createMap();
        void displayTimer(sf::RenderWindow& window);
};