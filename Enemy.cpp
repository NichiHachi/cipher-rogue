#include "Enemy.h"
#include "Bullet.h"
#include <cmath>

Enemy::Enemy(float x, float y, float angle, float speed, int hp, float shootTimer) : x(x), y(y), angle(angle), speed(speed), hp(hp), shootTimer(shootTimer) {
    this->x = x;
    this->y = y;
    this->angle = angle;
    this->speed = speed;
    this->hp = hp;
    this->shootTimer = shootTimer;
}

bool Enemy::getShot(std::vector<Bullet*>& bullets) {
    int diffX, diffY;
    for(auto bullet = bullets.begin(); bullet != bullets.end();){
        diffX = x - (*bullet)->getX();
        diffY = y - (*bullet)->getY();
        // If the border of the bullet touches the enemy
        // sqrt(x² + y²) < 34 :
        if (diffX * diffX + diffY * diffY < 34*34) {
            hp -= 1;
            delete *bullet;
            bullet = bullets.erase(bullet);
        }
        else {
            bullet++;
        }
    }
    return hp <= 0;
}