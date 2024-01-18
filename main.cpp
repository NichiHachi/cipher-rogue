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

const int displayX = 1000;
const int displayY = 1000;
const sf::Color backgroundColor(0,0,0);

double calcul_angle(Position start, Position end){
    return atan2(start.y - end.y, end.x - start.x);
}

//Change because it sucks :c

/// @brief Predict where the player will be when the bullet will touch him.
/// @param player The player.
/// @param enemy The enemy that shoot.
/// @return The predicted angle
float angle_shot_predict(Player player, Enemy* enemy){
    Position diffPos = player.getPosition() - enemy->getPosition();
    float timeBulletTravel = sqrt(diffPos.x*diffPos.x+diffPos.y*diffPos.y)/enemy->getSpeedBullet();

    Position playerNewPos = player.getPosition();
    float angleNew = 0;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        playerNewPos += Position(player.getSpeed() * timeBulletTravel * sqrt(2) / 2,
                                    -player.getSpeed() * timeBulletTravel * sqrt(2) / 2);
        angleNew = M_PI/4;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        playerNewPos -= Position(player.getSpeed() * timeBulletTravel * sqrt(2) / 2,
                                    player.getSpeed() * timeBulletTravel * sqrt(2) / 2);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        playerNewPos += Position(-player.getSpeed() * timeBulletTravel * sqrt(2) / 2,
                                    player.getSpeed() * timeBulletTravel * sqrt(2) / 2);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        playerNewPos += Position(player.getSpeed() * timeBulletTravel * sqrt(2) / 2,
                                    player.getSpeed() * timeBulletTravel * sqrt(2) / 2);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
        playerNewPos.y -= player.getSpeed() * timeBulletTravel;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        playerNewPos.y += player.getSpeed() * timeBulletTravel;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        playerNewPos.x += player.getSpeed() * timeBulletTravel;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        playerNewPos.x -= player.getSpeed() * timeBulletTravel;
    }

    //Return the angle between the enemy and the new player position
    return calcul_angle(enemy->getPosition(), playerNewPos);
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
            Position bulletWallPos = bulletPos + Position(cos(angleBulletWall), sin(angleBulletWall)) * bulletSize;
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

void updateLoop(sf::RenderWindow& window, sf::Clock& clock, Player& player, std::vector<Bullet*>& bulletsEnemy, 
                std::vector<Bullet*>& bulletsAlly, std::vector<Wall>walls, std::vector<Enemy*>& enemies){
        float currentTime = clock.restart().asSeconds();

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
            if(enemy->getType() == "Sniper") targetAngle = angle_shot_predict(player,enemy);
            else targetAngle = calcul_angle(enemy->getPosition(),player.getPosition());
            enemy->update(bulletsEnemy, currentTime, targetAngle, walls, enemies);
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

        player.draw(window);

        for(Wall &wall : walls){
            wall.draw(window);
        }

        player.drawHealth(window);
}

int main(void){
    sf::RenderWindow window(sf::VideoMode(displayX,displayY), "Nichi Hachi");

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
    enemies.push_back(new EnemyShooter(Position(500,600)));

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
    window.setFramerateLimit(60);

    std::vector<int> listLevel;
    player.spawn();

    while(window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)window.close();
        }

        window.clear(backgroundColor);

        updateLoop(window, clock, player, bulletsEnemy, bulletsAlly, walls, enemies);

        drawElements(window, player, bulletsEnemy, bulletsAlly, walls, enemies);

        window.display();
    }
    return 0;
}