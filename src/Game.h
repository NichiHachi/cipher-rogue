#include <vector>
#include <memory>
#include <string>

#include "Bullet.h"
#include "Bombshell.h"
#include "Player.h"
#include "Wall.h"
#include "Enemy.h"

class Game{
    public:
        Player player;
        
        std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bulletsEnemy;
        std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bulletsAlly;
        std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>> bombshells;
        std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls;
        std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies;
        std::vector<std::string> messageTerminal;

        Game(int FPS);
        ~Game();

        void update(sf::RenderWindow& window, float deltaTime);
        void draw(sf::RenderWindow& window, float deltaTime);
    
    private:
        std::string displayedMessage = "";
        float displayedMessageTimer = 0;
        float cursorTimer = 0;
        bool cursorVisible = true;
        sf::Text text;
        sf::Font font;
        
        void setStatsScaleWithFPS(int FPS);
        void drawFakeTerminal(sf::RenderWindow& window, float deltaTime);
        void drawCursorTerminal(sf::RenderWindow& window, float deltaTime);
        void destroyWalls();
        void bulletCollisions();
};