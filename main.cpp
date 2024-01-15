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

const int displayX = 1000;
const int displayY = 1000;
const sf::Color backgroundColor(0,0,0);

double calcul_angle(float startX, float startY, float endX, float endY){
    return atan2(startY-endY,endX-startX);
}

double angle_shot_predict(Player player, float objectX, float objectY, float speedBullet, float playerSpeed){
    float diffX = player.getX()-objectX;
    float diffY = player.getY()-objectY;
    float distance = sqrt(diffX*diffX+diffY*diffY);
    float timeBulletTravel = distance/speedBullet;

    // Prédire la nouvelle position du joueur
    float playerNewX = player.getX();
    float playerNewY = player.getY();
    float angleNew = 0;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        playerNewX += playerSpeed * timeBulletTravel * sqrt(2) / 2;
        playerNewY -= playerSpeed * timeBulletTravel * sqrt(2) / 2;
        angleNew = M_PI/4;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        playerNewX -= playerSpeed * timeBulletTravel * sqrt(2) / 2;
        playerNewY -= playerSpeed * timeBulletTravel * sqrt(2) / 2;
        angleNew = M_PI*3/4;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        playerNewX -= playerSpeed * timeBulletTravel * sqrt(2) / 2;
        playerNewY += playerSpeed * timeBulletTravel * sqrt(2) / 2;
        angleNew = -M_PI*3/4;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        playerNewX += playerSpeed * timeBulletTravel * sqrt(2) / 2;
        playerNewY += playerSpeed * timeBulletTravel * sqrt(2) / 2;
        angleNew = -M_PI/4;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
        playerNewY -= playerSpeed * timeBulletTravel;
        angleNew = M_PI/2;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        playerNewY += playerSpeed * timeBulletTravel;
        angleNew = -M_PI/2;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        playerNewX += playerSpeed * timeBulletTravel;
        angleNew = 0;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        playerNewX -= playerSpeed * timeBulletTravel;
        angleNew = M_PI;
    }

    // Calculer l'angle entre l'objet et la nouvelle position du joueur
    float diffXNew = playerNewX - objectX;
    float diffYNew = playerNewY - objectY;
    float angleShot = atan2(diffYNew, diffXNew);

    return angleShot;
}

void destroy_in_wall_or_out_of_bounds(std::vector<Bullet*>& bullets,std::vector<Wall> walls){
    int bulletX, bulletY;
    float bulletHitBoxRadius, angleBulletWall;
    bool destroyed;
    for(auto bullet = bullets.begin(); bullet != bullets.end();){
        bulletX = (*bullet)->getX();
        bulletY = (*bullet)->getY();
        bulletHitBoxRadius = (*bullet)->getHitBoxRadius();
        destroyed = false;
        //If the bullet get out of bounds
        if(bulletX < -bulletHitBoxRadius*2 || bulletY<-bulletHitBoxRadius*2 || 
        bulletX>displayX+bulletHitBoxRadius*2 || bulletY>displayY+bulletHitBoxRadius*2){
            delete *bullet;
            bullet = bullets.erase(bullet);
            destroyed = true;
        }
        else{
            //If the bullet touch a wall
            for(unsigned int idWall = 0; idWall < walls.size(); idWall++){
                angleBulletWall = calcul_angle(walls[idWall].getX(),walls[idWall].getY(),bulletX,bulletY);
                if(walls[idWall].isInWall(bulletX+cos(angleBulletWall)*bulletHitBoxRadius,bulletY+sin(angleBulletWall)*bulletHitBoxRadius)){
                    delete *bullet;
                    bullet = bullets.erase(bullet);
                    destroyed = true;
                    break;
                }
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

        bool destroyed;
        for(auto bulletAlly = bulletsAlly.begin(); bulletAlly != bulletsAlly.end();){
            //If the ally bullet touch a enemy bullet that is destructible -> destroy both bullets
            int diffX, diffY;
            float hitBoxBoth;
            destroyed = false;
            for(auto bulletEnemy = bulletsEnemy.begin(); bulletEnemy != bulletsEnemy.end();){
                if ((*bulletEnemy)->isDestructible()) {
                    diffX = (*bulletAlly)->getX() - (*bulletEnemy)->getX();
                    diffY = (*bulletAlly)->getY() - (*bulletEnemy)->getY();
                    hitBoxBoth = (*bulletAlly)->getHitBoxRadius()+(*bulletEnemy)->getHitBoxRadius();
                    if (diffX * diffX + diffY * diffY < hitBoxBoth*hitBoxBoth) {
                        delete *bulletAlly;
                        bulletAlly = bulletsAlly.erase(bulletAlly);
                        delete *bulletEnemy;
                        bulletEnemy = bulletsEnemy.erase(bulletEnemy);
                        destroyed = true;
                        break;
                    }
                    else bulletEnemy++;
                }
                else bulletEnemy++;
            }
            if(!destroyed) bulletAlly++;
        }

        //-----ENEMIES AND ALLY BULLETS-----
        for(auto it = enemies.begin(); it != enemies.end();) {
            if((*it)->receiveDamageIfShot(bulletsAlly)) {
                delete *it;
                it = enemies.erase(it);
            } 
            else {
                it++;
            }
        }

        //-----PLAYER AND ENEMY BULLETS-----
        player.receiveDamageIfShot(bulletsEnemy);

        //-----ENEMIES UPDATE-----
        float angleEnemyToPlayer;
        for(Enemy* enemy : enemies){
            angleEnemyToPlayer = calcul_angle(enemy->getX(),enemy->getY(),player.getX(),player.getY());
            enemy->update(bulletsEnemy, currentTime, angleEnemyToPlayer, walls, enemies);
        }

        //-----PLAYER AND ENEMIES-----
        player.receiveDamageIfHit(enemies);
}

void drawElements(sf::RenderWindow& window, Player player, std::vector<Bullet*>bulletsEnemy, 
    std::vector<Bullet*>bulletsAlly, std::vector<Wall>walls, std::vector<Enemy*>enemies){

        for(Bullet* bullet : bulletsAlly){
            bullet->draw(window);
        }

        for(Bullet* bullet : bulletsEnemy){
            bullet->draw(window);
        }

        //ENEMIES
        for(Enemy* enemy : enemies){
            (*enemy).draw(window);
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
    enemies.push_back(new EnemyCharger(400,400,2,1));
    
    //Init EnemyShooter
    //enemies.push_back(new EnemyShooter(500,500,1,1));
    enemies.push_back(new EnemyShooter(400,500,1,1));
    enemies.push_back(new EnemyShooter(500,600,1,1));

    //Init EnemyTurret
    //enemies.push_back(new EnemyTurret(450,785,1,1));

    //Init EnemySpawner
    enemies.push_back(new EnemySpawner(400,100,1,1));
    enemies.push_back(new EnemySpawner(400,200,1,1));
    enemies.push_back(new EnemySpawner(400,300,1,1));
    enemies.push_back(new EnemySpawner(400,400,1,1));
    enemies.push_back(new EnemySpawner(400,500,1,1));

    //Init EnemySniper
    //enemies.push_back(new EnemySniper(600,400,1,1));

    //Init Seeker
    //enemies.push_back(new EnemySeeker(400,400,0,1));
    //enemies.push_back(new EnemySeeker(400,420,0,1));

    //Init Wall Array
    std::vector<Wall> walls;
    walls.emplace_back(250,400);
    walls.emplace_back(200,400);
    walls.emplace_back(200,450);

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