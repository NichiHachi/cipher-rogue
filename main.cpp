#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <iostream>

#include "Bullet.h"
#include "Player.h"
#include "Wall.h"
#include "Enemy.h"
#include "EnemyShooter.h"
#include "EnemyTurret.h"
#include "EnemySpawner.h"
#include "EnemySniper.h"
#include "EnemyCharger.h"
#include "EnemySeeker.h"
#include "EnemyStats.h"
#include "PlayerStats.h"
#include <string>

const int displayX = 1000;
const int displayY = 1000;
const sf::Color backgroundColor(0,0,0);
const int FPS = 60;

float calcul_angle(Position start, Position end){
    return atan2(start.y - end.y, end.x - start.x);
}

void destroy_in_wall_or_out_of_bounds(std::vector<Bullet*>& bullets, std::vector<Wall> walls) {
    Position bulletPos;
    float bulletSize, angleBulletWall;
    bool destroyed;

    for (auto bullet = bullets.begin(); bullet != bullets.end();) {
        destroyed = false;
        bulletPos = (*bullet)->getPosition();
        bulletSize = (*bullet)->getSize();

        // If the bullet is out of bounds
        if (bulletPos.x < -bulletSize * 2 || bulletPos.y < -bulletSize * 2 ||
            bulletPos.x > displayX + bulletSize * 2 || bulletPos.y > displayY + bulletSize * 2) {
            delete *bullet;
            bullet = bullets.erase(bullet);
            continue;
        } 

        // If the bullet touches a wall
        for (const Wall wall : walls) {
            angleBulletWall = calcul_angle(wall.getPosition(), bulletPos);
            Position bulletWallPos = bulletPos + Position(-cos(angleBulletWall), sin(angleBulletWall)) * bulletSize;
            if (wall.isInWall(bulletWallPos)) {
                delete *bullet;
                bullet = bullets.erase(bullet);
                destroyed = true;
                break;
            }
        }

        if(!destroyed) bullet++;
    }
}

bool cursorVisible = true;
float timeCursor = 0;

void drawCursor(sf::RenderWindow& window, sf::Text& text) {
    if (cursorVisible) {
        sf::RectangleShape cursor(sf::Vector2f(2, text.getCharacterSize()));
        cursor.setPosition(text.getPosition().x + text.getLocalBounds().width + 5, text.getPosition().y + 2);
        cursor.setFillColor(sf::Color::White);
        window.draw(cursor);
    }

    if (timeCursor >= 0.5) {
        cursorVisible = !cursorVisible;
        timeCursor = 0;
    }
}

std::vector<std::string> deadEnemies;
std::string displayedMessage = "";
float timeDrawn = 0;


void printDeadEnemies(std::string& displayedMessage, sf::RenderWindow& window, float timer) {
    timeCursor += timer;
    if(deadEnemies.empty()) return;

    timeDrawn += timer;

    std::string fullMessage = "Delete [] " + deadEnemies[0];

    if(displayedMessage.size() < fullMessage.size() && timeDrawn > 0.1) {
        displayedMessage += fullMessage[displayedMessage.size()];
        timeDrawn = 0;
    }
    else if(timeDrawn > 1) {
        timeDrawn = 0;
        deadEnemies.erase(deadEnemies.begin());
        displayedMessage = "";
    }
    

    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("./fonts/FiraCode.ttf")) {}
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setPosition(10, 975);
    text.setString(displayedMessage);
    text.setCharacterSize(15);
    drawCursor(window, text);
    window.draw(text);
}

void updateLoop(sf::RenderWindow& window, float currentTime, Player& player, std::vector<Bullet*>& bulletsEnemy, 
                std::vector<Bullet*>& bulletsAlly, std::vector<Wall>walls, std::vector<Enemy*>& enemies){

        //-----PLAYER UPDATE-----
        player.update(bulletsAlly,window,currentTime,walls);

        //-----BULLETS UPDATE-----
        //ENEMY'S BULLETS
        for(Bullet* bullet : bulletsEnemy){
            bullet->update();
        }

        destroy_in_wall_or_out_of_bounds(bulletsEnemy,walls);

        //ALLY'S BULLET
        for(Bullet* bullet : bulletsAlly){
            bullet->update();
        }

        destroy_in_wall_or_out_of_bounds(bulletsAlly,walls);

        Position diffPos;
        float hitBoxBoth;
        bool allyBulletDestroyed;
        for(auto bulletAlly = bulletsAlly.begin(); bulletAlly != bulletsAlly.end();){
            //If the ally bullet touch a enemy bullet that is destructible -> destroy both bullets
            allyBulletDestroyed = false;
            for(auto bulletEnemy = bulletsEnemy.begin(); bulletEnemy != bulletsEnemy.end();){
                if (!(*bulletEnemy)->isDestructible()) {
                    bulletEnemy++;
                    continue;
                }

                diffPos = (*bulletAlly)->getPosition() - (*bulletEnemy)->getPosition();
                hitBoxBoth = (*bulletAlly)->getSize() + (*bulletEnemy)->getSize();
                if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth*hitBoxBoth) {
                    delete *bulletAlly;
                    bulletAlly = bulletsAlly.erase(bulletAlly);
                    delete *bulletEnemy;
                    bulletEnemy = bulletsEnemy.erase(bulletEnemy);
                    allyBulletDestroyed = true;
                    break;
                }
                
                bulletEnemy++;
            }

            if(!allyBulletDestroyed) bulletAlly++;
        }

        //-----PLAYER AND ENEMY BULLETS-----
        player.receiveDamageIfShot(bulletsEnemy);

        //-----ENEMIES UPDATE-----
        float targetAngle;
        for(Enemy* enemy : enemies){
            enemy->update(bulletsEnemy, currentTime, player, walls, enemies);
        }

        //-----ENEMIES AND ALLY BULLETS-----
        for(Enemy* enemy : enemies) {
            enemy->receiveDamageIfShot(bulletsAlly);
        }

        //-----PLAYER AND ENEMIES-----
        player.receiveDamageIfHit(enemies);

        //-----ENEMIES DEAD-----
        for(auto enemy = enemies.begin(); enemy != enemies.end();) {
            if((*enemy)->isDead()){
                deadEnemies.push_back((*enemy)->getType());
                delete *enemy;
                enemy = enemies.erase(enemy);
            } 
            else enemy++;
        }
}

void drawElements(sf::RenderWindow& window, Player player, std::vector<Bullet*>bulletsEnemy, 
    std::vector<Bullet*>bulletsAlly, std::vector<Wall>walls, std::vector<Enemy*>enemies){

        for(Enemy* enemy : enemies){
            enemy->drawEffects(window);
        }

        for(Bullet* bullet : bulletsAlly){
            bullet->draw(window);
        }

        for(Bullet* bullet : bulletsEnemy){
            bullet->draw(window);
        }

        //ENEMIES
        for(Enemy* enemy : enemies){
            enemy->draw(window);
        }

        player.draw(window );

        for(Wall &wall : walls){
            wall.draw(window);
        }

        player.drawHealth(window );
}

void setStatsScaleWithFPS(int FPS) {
    float ratioFPS = (float)60/FPS;

    Player::stats.speedFactor = ratioFPS;
    Player::stats.speedBulletFactor = ratioFPS;

    EnemyShooter::stats.speedFactor = ratioFPS;
    EnemyShooter::stats.turnSpeedFactor = ratioFPS;
    EnemyShooter::stats.speedBulletFactor = ratioFPS;

    EnemyCharger::stats.speedFactor = ratioFPS;
    EnemyCharger::stats.turnSpeedFactor = ratioFPS;
    EnemyCharger::stats.speedBulletFactor = ratioFPS;

    EnemySniper::stats.speedFactor = ratioFPS;
    EnemySniper::stats.turnSpeedFactor = ratioFPS;
    EnemySniper::stats.speedBulletFactor = ratioFPS;

    EnemySeeker::stats.speedFactor = ratioFPS;
    EnemySeeker::stats.turnSpeedFactor = ratioFPS;
    EnemySeeker::stats.speedBulletFactor = ratioFPS;

    EnemySpawner::stats.speedFactor = ratioFPS;
    EnemySpawner::stats.turnSpeedFactor = ratioFPS;
    EnemySpawner::stats.speedBulletFactor = ratioFPS;

    EnemyTurret::stats.speedFactor = ratioFPS;
    EnemyTurret::stats.turnSpeedFactor = ratioFPS;
    EnemyTurret::stats.speedBulletFactor = ratioFPS;
}

int main(void){
    sf::RenderWindow window(sf::VideoMode(displayX,displayY), "Nichi Hachi");

    setStatsScaleWithFPS(FPS);

    //Init Player
    Player player;

    //Init Enemy Array
    std::vector<Enemy*> enemies;

    //Init Bullet Array
    std::vector<Bullet*> bulletsEnemy;
    std::vector<Bullet*> bulletsAlly;

    //Init EnemyCharger
    enemies.push_back(new EnemyCharger(Position(400,400)));
    
    //Init EnemyShooter
    //enemies.push_back(new EnemyShooter(500,500,1,1));
    enemies.push_back(new EnemyShooter(Position(400,500)));
    //enemies.push_back(new EnemyShooter(Position(500,600)));

    //Init EnemyTurret
    enemies.push_back(new EnemyTurret(Position(450,785)));

    //Init EnemySpawner
    enemies.push_back(new EnemySpawner(Position(400,100)));
    //enemies.push_back(new EnemySpawner(400,200,1,1));
    //enemies.push_back(new EnemySpawner(400,300,1,1));
    //enemies.push_back(new EnemySpawner(400,400,1,1));
    //enemies.push_back(new EnemySpawner(400,500,1,1));

    //Init EnemySniper
    enemies.push_back(new EnemySniper(Position(600,400)));

    //Init Seeker
    //enemies.push_back(new EnemySeeker(400,400,0,1));
    //enemies.push_back(new EnemySeeker(400,420,0,1));

    //Init Wall Array
    std::vector<Wall> walls;
    walls.emplace_back(Position(250,400));
    walls.emplace_back(Position(200,400));
    walls.emplace_back(Position(200,450));

    //Time track and Framerate
    sf::Clock clock;
    float currentTime;
    window.setFramerateLimit(FPS);

    player.spawn();

    while(window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)window.close();
        }

        window.clear(backgroundColor);

        currentTime = clock.restart().asSeconds();

        updateLoop(window, currentTime, player, bulletsEnemy, bulletsAlly, walls, enemies);

        printDeadEnemies(displayedMessage,window, currentTime);

        drawElements(window, player, bulletsEnemy, bulletsAlly, walls, enemies);

        window.display();
    }
    return 0;
}