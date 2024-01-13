#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Enemy.h"
#include "Bullet.h"
#include "Wall.h"

class EnemySeeker{
    public:
        EnemySeeker(float x, float y, float angle);
        void update(float targetAngle, std::vector<Wall> walls);
        void draw(sf::RenderWindow& window);

        void move(float targetAngle, std::vector<Wall> walls);

    private:
        float x,y,angle,speed;
        int hp;
};

class EnemySpawner : public Enemy{
    public :
        EnemySpawner(float x, float y);
        void update(std::vector<Bullet*>& bullets, float timePassed, float targetAngle, std::vector<Wall> walls);
        void draw(sf::RenderWindow& window);

    private:
        float x,y,angle,speed,shootTimer;
        int hp;
        std::vector<EnemySeeker*> spawns;
};