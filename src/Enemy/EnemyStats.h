#pragma once
#include <vector>
#include <string>

struct EnemyStats{
    float speedFactor = 1;
    float sizeFactor = 1;
    float speedBulletFactor = 1;
    float turnSpeedFactor = 1;
    std::vector<std::string> specialEffects;
};