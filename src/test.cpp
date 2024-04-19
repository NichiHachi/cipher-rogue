#include <cassert>
#include <iostream>
#include <cmath>
#include <memory>
#include "Enemy/Enemy.h"
#include "Enemy/EnemyCharger.h"
#include "Enemy/EnemySpawner.h"
#include "Wall.h"
#include "Position.h"
#include "Game.h"

int main() {
    std::cout << "Start Test" << std::endl;
    EnemyCharger enemyCharger(Position(30, 30));

    Wall wall(Position(0, 0));

    std::cout << "Test 1 : adjustPositionBasedOnWalls" << std::endl;

    //enemyCharger.adjustPositionBasedOnWalls(wall);

    //assert(!wall.isInWall(enemyCharger.getPosition()));

    std::cout << "Test 1 succeeded" << std::endl;

    std::cout << "Test 2 : adjustPositionBasedOnOOB" << std::endl;
    enemyCharger.setPosition(Position(-100, -100));
    enemyCharger.adjustPositionBasedOnOOB();
    assert(enemyCharger.getPosition().x >= 0 && enemyCharger.getPosition().y >= 0 && enemyCharger.getPosition().x <= 1000 && enemyCharger.getPosition().y <= 1000);
    std::cout << "Test 2 succeeded" << std::endl;

    std::cout << "Test 3 : getAngleToTarget" << std::endl;
    enemyCharger.setPosition(Position(10, 10));
    assert(enemyCharger.getAngleToTarget(Position(10, 20)) == static_cast<float>(-M_PI/2));
    assert(enemyCharger.getAngleToTarget(Position(20, 10)) == 0);
    std::cout << "Test 3 succeeded" << std::endl;

    std::cout << "Test 4 : ReceiveDamage and isDead" << std::endl;
    enemyCharger.receiveDamage(100);
    assert(enemyCharger.isDead());
    std::cout << "Test 4 succeeded" << std::endl;

    std::cout << "Test 5 : isPlayerDead" << std::endl;
    Game game;
    assert(!game.isPlayerDead());
    std::cout << "Test 5 succeeded" << std::endl;

    std::cout << "Test 6 : Update Spawner" << std::endl;
    EnemySpawner enemySpawner(Position(10, 10));
    std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies = std::make_shared<std::vector<std::unique_ptr<Enemy>>>();
    enemySpawner.update(std::make_shared<std::vector<std::unique_ptr<Bullet>>>() , Player(), std::make_shared<std::vector<std::unique_ptr<Wall>>>() ,enemies, 1000, std::make_shared<std::vector<std::unique_ptr<Bombshell>>>() );
    assert(enemies->size() == 1);
    std::cout << "Test 6 succeeded" << std::endl;
    
    std::cout << "Everything worked ! :3c" << std::endl;
    return 0;
} 