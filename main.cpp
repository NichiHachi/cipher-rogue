#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <set>

#include "Bullet.h"
#include "Player.h"
#include "Wall.h"
#include "Enemy.h"
#include "EnemyShooter.h"
#include "EnemyTurret.h"
#include "EnemySpawner.h"
#include "EnemySniper.h"
#include "EnemyCharger.h"

const int displayX = 1000;
const int displayY = 1000;
const int bulletRadius = 15;
const sf::Color backgroundColor(0,0,0);

double calcul_angle(float startX, float startY, float endX, float endY){
    return atan2(startY-endY,endX-startX);
}

double angle_shot_predict(Player player, float objectX, float objectY, float speedBullet){
    float diffX = player.getX()-objectX;
    float diffY = player.getY()-objectY;
    float distance = sqrt(diffX*diffX+diffY*diffY);
    float timeBulletTravel = distance/speedBullet*1.5;
    float playerNewX, playerNewY;
    float angleNew = -1;
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        angleNew = M_PI/4;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        angleNew = M_PI*3/4;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        angleNew = -M_PI*3/4;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        angleNew = -M_PI/4;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){
        angleNew = M_PI/2;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)){
        angleNew = -M_PI/2;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)){
        angleNew = 0;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){
        angleNew = M_PI;
    }
            
    if(angleNew==-1){
        playerNewX = player.getX();
        playerNewY = player.getY();
    }
    else{
        playerNewX = player.getX()+cos(angleNew)*player.getSpeed()*timeBulletTravel;
        playerNewY = player.getY()-sin(angleNew)*player.getSpeed()*timeBulletTravel;
    }

    return calcul_angle(objectX,objectY,playerNewX,playerNewY);
}

bool is_in_wall_or_out_of_bounds(Bullet* bullet,std::vector<Wall> walls){
    for(unsigned int idWall = 0; idWall < walls.size(); idWall++){
        if(walls[idWall].isInWall(bullet->getX()+bulletRadius,bullet->getY()+bulletRadius)){
            return true;
        }
        else if(walls[idWall].isInWall(bullet->getX()-bulletRadius,bullet->getY()+bulletRadius)){
            return true;
        }
        else if(walls[idWall].isInWall(bullet->getX()-bulletRadius,bullet->getY()-bulletRadius)){
            return true;
        }
        else if(walls[idWall].isInWall(bullet->getX()+bulletRadius,bullet->getY()-bulletRadius)){
            return true;
        }
        idWall ++;
    }
    if(bullet->getX()<0 || bullet->getY()<0 || bullet->getX()>displayX || bullet->getY()>displayY){
        return true;
    }
    return false;
}

void updateLoop(sf::RenderWindow& window, sf::Clock& clock, Player& player, std::vector<Bullet*>& bulletsEnemy, 
                std::vector<Bullet*>& bulletsAlly, std::vector<Wall>walls, std::vector<Enemy*>& enemies){
        float currentTime = clock.restart().asSeconds();

        //-----PLAYER UPDATE-----
        player.update(bulletsAlly,window,currentTime,walls);

        //-----BULLETS UPDATE-----
        //ENEMY'S BULLETS
        for(auto bullet = bulletsEnemy.begin(); bullet != bulletsEnemy.end();){
            (*bullet)->update();
            ////If the enemy bullet touch the player HitBox: -1HP and destroy the bullet
            if(player.getHit((*bullet)->getX(),(*bullet)->getY())){
                //delete the bullet
                delete *bullet;
                bullet = bulletsEnemy.erase(bullet);
            }
            else if(is_in_wall_or_out_of_bounds(*bullet,walls)){
                delete *bullet;
                bullet = bulletsEnemy.erase(bullet);
            }
            else{
                bullet++;
            }
        }

        //ALLY'S BULLET
        bool gotDestoyed;
        for(auto bulletAlly = bulletsAlly.begin(); bulletAlly != bulletsAlly.end();){
            (*bulletAlly)->update();
            gotDestoyed = false;
            
            //If the ally bullet touch a enemy bullet that is destructible -> destroy both bullets
            int diffX, diffY;
            for(auto bulletEnemy = bulletsEnemy.begin(); bulletEnemy != bulletsEnemy.end();){
                if ((*bulletEnemy)->isDestructible()) {
                    diffX = (*bulletAlly)->getX() - (*bulletEnemy)->getX();
                    diffY = (*bulletAlly)->getY() - (*bulletEnemy)->getY();
                    if (-bulletRadius < diffX  && diffX < bulletRadius && -bulletRadius > diffY && diffY < bulletRadius) {
                        delete *bulletAlly;
                        bulletAlly = bulletsAlly.erase(bulletAlly);
                        delete *bulletEnemy;
                        bulletEnemy = bulletsEnemy.erase(bulletEnemy);
                        gotDestoyed = true;
                        break;
                    }
                    else bulletEnemy++;
                }
                else bulletEnemy++;
            }
            if(!gotDestoyed){
                if(is_in_wall_or_out_of_bounds(*bulletAlly,walls)){
                    delete *bulletAlly;
                    bulletAlly = bulletsAlly.erase(bulletAlly);
                }
                else{
                    bulletAlly++;
                }
            }
        }

        //Look if the enemies get shot. If they have 0HP, delete them
        for(auto it = enemies.begin(); it != enemies.end();) {
            if((*it)->getShot(bulletsAlly)) {
                delete *it;
                it = enemies.erase(it);
            } 
            else {
                it++;
            }
        }

        //-----ENEMIES UPDATE-----
        float angleEnemyToPlayer;
        for(Enemy* enemy : enemies){
            angleEnemyToPlayer = calcul_angle(enemy->getX(),enemy->getY(),player.getX(),player.getY());
            enemy->update(bulletsEnemy,currentTime,angleEnemyToPlayer,walls);
        }
}

void drawElements(sf::RenderWindow& window, Player player, std::vector<Bullet*>bulletsEnemy, 
    std::vector<Bullet*>bulletsAlly, std::vector<Wall>walls, std::vector<Enemy*>enemies){

        player.draw(window);
        
        for(Wall &wall : walls){
            wall.draw(window);
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

    //Init EnemyShooter
    enemies.push_back(new EnemyShooter(600,700));

    //Init EnemyTurret
    enemies.push_back(new EnemyTurret(450,785));

    //Init EnemySpawner
    enemies.push_back(new EnemySpawner(400,400));

    //Init EnemySniper
    enemies.push_back(new EnemySniper(400,400));

    //Init EnemyCharger
    enemies.push_back(new EnemyCharger(400,400));

    //Init Wall Array
    std::vector<Wall> walls;
    walls.emplace_back(200,350);
    walls.emplace_back(200,400);
    walls.emplace_back(200,450);
    walls.emplace_back(200,500);

    //Time track and Framerate
    sf::Clock clock;
    window.setFramerateLimit(60);

    std::vector<int> listLevel;
    player.spawn();

    while(window.isOpen()){
        player.spawn();
        while(true){
            sf::Event event;
            while(window.pollEvent(event)){
                if(event.type == sf::Event::Closed){
                    window.close();
                }
            }

            window.clear(backgroundColor);

            updateLoop(window, clock, player, bulletsEnemy, bulletsAlly, walls, enemies);

            drawElements(window, player, bulletsEnemy, bulletsAlly, walls, enemies);

            window.display();
        }
    }

    return 0;
}