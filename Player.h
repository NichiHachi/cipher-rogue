#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>

#include "Bullet.h"
#include "Wall.h"
#include "Enemy.h"

class Player{
    public:
        Player();
        void update(std::vector<Bullet*>& bullets,sf::RenderWindow& window,float timePassed, std::vector<Wall> walls);
        void spawn();
        void move(sf::RenderWindow& window, std::vector<Wall> walls);
        void shoot(std::vector<Bullet*>& bullets);
        void receiveDamageIfShot(std::vector<Bullet*>& bullets);
        void receiveDamageIfHit(std::vector<Enemy*> enemies);
        void receiveDamage(unsigned int damage);
        void draw(sf::RenderWindow& window);
        void drawHealth(sf::RenderWindow& window);
        float getX() {return x;};
        float getY() {return y;};

    private:
        float x,y,speed,shootTimer,hitTimer;
        int hp,size;
        double angle;
};