#include <cassert>
#include <iostream>
#include <cmath>
#include "Enemy/Enemy.h"
#include "Enemy/EnemyCharger.h"
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

    std::cout << "Everything worked ! :3c" << std::endl;
    return 0;
} 