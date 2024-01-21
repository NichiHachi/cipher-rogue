#include "Position.h"
#include "Bullet.h"

Bullet::Bullet(Position position, float angle, Position targetPosition, float speed, float size, bool ally, bool destructible) : position(position), angle(angle), targetPosition(targetPosition), speed(speed), size(size), ally(ally), destructible(destructible){}
