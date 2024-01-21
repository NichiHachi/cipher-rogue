#include <vector>
#include <memory>
#include <string>

#include "Bullet.h"
#include "Player.h"
#include "Wall.h"
#include "Enemy.h"

class Game{
    public:
        Player player;
        std::vector<std::unique_ptr<Bullet>> bulletsEnemy;
        std::vector<std::unique_ptr<Bullet>> bulletsAlly;
        std::vector<std::unique_ptr<Wall>> walls;
        std::vector<std::unique_ptr<Enemy>> enemies;
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
        void destroyInWallOrOutOfBounds(std::vector<std::unique_ptr<Bullet>>& bullets);
        void setStatsScaleWithFPS(int FPS);
        void fakeTerminalDraw(sf::RenderWindow& window, float deltaTime);
        void drawCursor(sf::RenderWindow& window, float deltaTime);
};