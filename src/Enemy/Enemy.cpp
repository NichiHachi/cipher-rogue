#include <SFML/Window.hpp>
#include <cmath>
#include <queue>
#include <functional>
#include <vector>
#include <set>
#include <algorithm>

#include "../Position.h"
#include "../Projectile/Bullet.h"
#include "../Projectile/Bombshell.h"
#include "Enemy.h"

Enemy::Enemy(Position position, float speed, float angle, float shootTimer, float speedBullet, int hp, int size, bool movable) : position(position), speed(speed), angle(angle), shootTimer(shootTimer), speedBullet(speedBullet), hp(hp), size(size), movable(movable){}

void Enemy::receiveDamageIfShot(const std::shared_ptr<std::vector<std::unique_ptr<Bullet>>>& bullets, const std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>>& bombshells){
    Position diffPos;
    float hitBoxBoth;
    auto bullet = bullets->begin();

    while (bullet != bullets->end()) {
        diffPos = position - bullet->get()->getPosition();
        hitBoxBoth = bullet->get()->getSize() + static_cast<float>(size);
        // sqrt(x² + y²) < n <=> x² + y² < n² :
        if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth * hitBoxBoth) {
            receiveDamage(1);
            bullet = bullets->erase(bullet);
        }
        else {
            bullet++;
        }
    }

    for(const auto & bombshell : *bombshells){
        if(!bombshell->hasExploded() || !bombshell->isAlly()) continue;
        
        if (std::find(bombshell->hitEnemies.begin(), bombshell->hitEnemies.end(), this) != bombshell->hitEnemies.end()) {
            continue;
        }

        diffPos = position - bombshell->getPosition();
        hitBoxBoth = static_cast<float>(size) + bombshell->getSize();
        if(diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth * hitBoxBoth){
            receiveDamage(2);
            bombshell->hitEnemies.push_back(this);
        }
    }
}

bool Enemy::adjustPositionBasedOnWalls(const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls){
    bool positionAdjusted = false;
    Position wallPos;
    float angleEnemyWall;
    int wallSize;
    for (const auto & wall : *walls) {
        wallPos = wall->getPosition();
        angleEnemyWall = std::atan2(wallPos.y - position.y, position.x - wallPos.x);
        //If the enemy nearest point from the middle of the wall is in the wall
        if (wall->isInWall(position + Position(-std::cos(angleEnemyWall),std::sin(angleEnemyWall))*size)){
            wallSize = wall->getSize();
            if(-M_PI/4 <= angleEnemyWall && angleEnemyWall <= M_PI/4){
                position.x += size - (position.x - (wallPos.x + wallSize));
            }
            else if(angleEnemyWall >= M_PI*3/4 || angleEnemyWall <= -M_PI*3/4){
                position.x -= size - ((wallPos.x - wallSize) - position.x);
            } 
            else if(M_PI/4 <= angleEnemyWall && angleEnemyWall <= M_PI*3/4){
                position.y -= size - ((wallPos.y - wallSize) - position.y);
            } 
            //angleEnemyWall< -M_PI/4 && angleEnemyWall > -M_PI*3/4
            else{
                position.y += size - (position.y - (wallPos.y + wallSize));
            }
            positionAdjusted = true;
        }
    }
    return positionAdjusted;
}

bool Enemy::adjustPositionBasedOnEnemies(const std::shared_ptr<std::vector<std::unique_ptr<Enemy>>>& enemies){
    bool positionAdjusted = false;
    float angleEnemyEnemy, hitBoxBoth, moveDistance;
    Position enemyPos, diffPos;
    for (const auto & enemy : *enemies) {
        if (enemy.get() == this) {
            //Don't check collision with itself
            continue;
        }

        enemyPos = enemy->getPosition();
        hitBoxBoth = static_cast<float>(enemy->getSize() + size);
        diffPos = position - enemyPos;
        
        //If the object is inside of the enemy
        if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth * hitBoxBoth){
            angleEnemyEnemy = std::atan2(enemyPos.y - position.y, position.x - enemyPos.x);
            moveDistance = hitBoxBoth - std::sqrt(diffPos.x * diffPos.x + diffPos.y * diffPos.y);
            // Move the enemy gradually towards the outside

            //TO DO : Scale the movement with the deltaTime
            if(enemy->isMovable()){
                enemy->setPosition(enemyPos + Position(std::cos(M_PI + angleEnemyEnemy) * moveDistance / 4,
                                                                - std::sin(M_PI + angleEnemyEnemy) * moveDistance / 4));
                                                      
                position += Position(std::cos(angleEnemyEnemy) * moveDistance / 4,
                                     - std::sin(angleEnemyEnemy) * moveDistance / 4);
            }
            else{
                position += Position(std::cos(angleEnemyEnemy) * moveDistance / 2,
                                     - std::sin(angleEnemyEnemy) * moveDistance / 2);
            }
            positionAdjusted = true;
        }
    }
    return positionAdjusted;
}

bool Enemy::adjustPositionBasedOnOOB(){
    bool positionAdjusted = false;

    if (position.x < size) {
        position.x = size;
        positionAdjusted = true;
    }
    else if (position.x > 1000 - size) {
        position.x = 1000 - size;
        positionAdjusted = true;
    }

    if (position.y < size) {
        position.y = size;
        positionAdjusted = true;
    }
    else if (position.y > 1000 - size) {
        position.y = 1000 - size;
        positionAdjusted = true;
    }

    return positionAdjusted;
}

float Enemy::getAngleToTarget(Position target) const {
    return std::atan2(position.y - target.y, target.x - position.x);
}

float Enemy::getAngleToFuturPlayerPosition(Player player){
    Position diffPos = player.getPosition() - position;
    float timeBulletTravel = std::sqrt(diffPos.x*diffPos.x + diffPos.y*diffPos.y)/speedBullet;

    Position playerNewPos = player.getPosition();

    int xAxisMove = 0;
    int yAxisMove = 0;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) yAxisMove++;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) xAxisMove--;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) yAxisMove--;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) xAxisMove++;
    
    if (yAxisMove != 0 && xAxisMove != 0) {
        playerNewPos += Position(xAxisMove*sqrt(2)/2,-yAxisMove*sqrt(2)/2)*player.getSpeed()*timeBulletTravel;
    }
    else{
        playerNewPos += Position(xAxisMove,-yAxisMove)*player.getSpeed()*timeBulletTravel;
    }

    //Return the angle between the enemy and the new player position
    return getAngleToTarget(playerNewPos);
}

void Enemy::smoothTurn(float targetAngle, float turnSpeedFactor, float deltaTime){
    float angleDiff = targetAngle - angle;
    
    // angleDiff stays between PI and -PI
    if (angleDiff > M_PI) {
        angleDiff -= 2 * M_PI;
    } else if (angleDiff < -M_PI) {
        angleDiff += 2 * M_PI;
    }

    angle += angleDiff * turnSpeedFactor * deltaTime * 60;

    // angle stays between PI and -PI
    if (angle > M_PI) {
        angle -= 2 * M_PI;
    } else if (angle < -M_PI) {
        angle += 2 * M_PI;
    }
}

// START A STAR ALGORITHM

struct Node {
    Position position;
    float gScore;
    float fScore;
    Node* parent;

    Node(Position position, float gScore, float fScore, Node* parent) 
        : position(position), gScore(gScore), fScore(fScore), parent(parent) {}
};

struct NodeComparator {
    bool operator()(const Node* a, const Node* b) const {
        return a->fScore > b->fScore;
    }
};

float heuristicCostEstimate(Position start, Position target){
    return std::sqrt((start.x - target.x)*(start.x - target.x) + (start.y - target.y)*(start.y - target.y));
}

float dist_between(Node* nodeA, Node* nodeB) {
    float dx = nodeB->position.x - nodeA->position.x;
    float dy = nodeB->position.y - nodeA->position.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool isIndexInWall(Position pos, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls){
    for(const auto & wall : *walls){
        if(wall->isInWall(pos*50+Position(25,25))){
            return true;
        }
    }
    return false;
}

std::vector<Node*> getNeighbors(Node* node, Position target ,const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls){
    std::vector<Node*> neighbors;
    Position neighborPos;
    for(int x=-1; x<=1; x++){
        for(int y=-1; y<=1; y++){
            if(x == 0 && y == 0) continue;
            
            neighborPos = node->position + Position(x, y);
            if(!isIndexInWall(neighborPos, walls) && neighborPos.x >= 0 && neighborPos.x < 20 && neighborPos.y >= 0 && neighborPos.y < 20){
                //Diagonal movement
                if(x == y || x == -y){
                    if(!isIndexInWall(node->position + Position(x, 0), walls) && !isIndexInWall(node->position + Position(0, y), walls)){
                        neighbors.push_back(new Node(neighborPos, node->gScore + 1.5f, heuristicCostEstimate(neighborPos, target), node));
                    }
                }
                //Straight movement
                else{
                    neighbors.push_back(new Node(neighborPos, node->gScore + 1, heuristicCostEstimate(neighborPos, target), node));
                }
            }
        }
    }

    return neighbors;
}

std::vector<Position> Enemy::aStar(Position target, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls){
    std::priority_queue<Node*, std::vector<Node*>, NodeComparator> openSet;
    std::vector<Node*> closedSet;
    std::vector<Position> openSetLookup;
    std::vector<Position> path;
    Position positionGrid = Position((int)position.x/50, (int)position.y/50);
    Position targetGrid = Position((int)target.x/50, (int)target.y/50);

    Node* startNode = new Node(positionGrid, 0, heuristicCostEstimate(positionGrid, targetGrid), nullptr);
    openSet.push(startNode);

    float weight = 50;

    while(!openSet.empty()){
        Node* current = openSet.top();
        openSet.pop();

        if(current->position == targetGrid || hasLineOfSight(target, walls)){
            Node* temp = current;

            while(temp != nullptr){
                path.push_back(temp->position);
                temp = temp->parent;
            }

            std::reverse(path.begin(), path.end());
            break;
        }

        closedSet.push_back(current);

        for(Node* neighbor: getNeighbors(current, targetGrid, walls)){
            if(std::find(closedSet.begin(), closedSet.end(), neighbor) != closedSet.end()){
                continue;
            }

            float tentativeGScore = current->gScore + dist_between(current, neighbor);
            if(tentativeGScore <= neighbor->gScore){
                neighbor->parent = current;
                neighbor->gScore = tentativeGScore;
                neighbor->fScore = neighbor->gScore + weight * heuristicCostEstimate(neighbor->position, target);
                
                if (std::find(openSetLookup.begin(), openSetLookup.end(), neighbor->position) == openSetLookup.end()) {
                    openSet.push(neighbor);
                    openSetLookup.push_back(neighbor->position);
                }
            }
        }
    }

    for (Node* node : closedSet) {
        delete node;
    }
    while (!openSet.empty()) {
        delete openSet.top();
        openSet.pop();
    }

    return path;
}

// END A STAR ALGORITHM

float Enemy::pathFinding(Position target, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls, float deltaTime){
    if(hasLineOfSight(target, walls)){
        float angleTarget = getAngleToTarget(target);
        position += Position(std::cos(angleTarget),-std::sin(angleTarget)) * speed * deltaTime * 60;
        return angleTarget;
    } else {
        std::vector<Position> path = aStar(target, walls);

        if (path.empty()) {
            return angle;
        }

        for(auto & pos : path){
            pos = pos*50 + Position(25, 25);
        }
        
        float distanceToMove = speed * deltaTime * 60;
        float distanceCheckpoint;
        Position checkpoint;
        while(distanceToMove > 0 && path.size() > 1){
            checkpoint = path[1] - position;
            distanceCheckpoint = checkpoint.x * checkpoint.x + checkpoint.y * checkpoint.y;
            if(distanceToMove*distanceToMove >= distanceCheckpoint){
                distanceToMove -= std::sqrt(distanceCheckpoint);
                position = path[1];
                path.erase(path.begin());
            } else {
                Position vectorScaled = path[1]-position;
                vectorScaled /= std::sqrt(distanceCheckpoint);
                position += vectorScaled*distanceToMove;
                distanceToMove = 0;
            }
        }

        return getAngleToTarget(path[1]);
    }
}

bool lineIntersectsWall(Position enemy, Position target, const Wall& wall) {
    float a1 = target.y - enemy.y;
    float b1 = enemy.x - target.x;
    float c1 = a1 * enemy.x + b1 * enemy.y;

    for (int i = 0; i < 4; ++i) {
        Position wallPoint1 = wall.getPoint(i);
        Position wallPoint2 = wall.getPoint((i + 1) % 4);

        float a2 = wallPoint2.y - wallPoint1.y;
        float b2 = wallPoint1.x - wallPoint2.x;
        float c2 = a2 * wallPoint1.x + b2 * wallPoint1.y;

        float determinant = a1 * b2 - a2 * b1;

        if (determinant != 0) {
            float x = (b2 * c1 - b1 * c2) / determinant;
            float y = (a1 * c2 - a2 * c1) / determinant;

            if (std::min(wallPoint1.x, wallPoint2.x) <= x && x <= std::max(wallPoint1.x, wallPoint2.x) &&
                std::min(wallPoint1.y, wallPoint2.y) <= y && y <= std::max(wallPoint1.y, wallPoint2.y) &&
                std::min(enemy.x, target.x) <= x && x <= std::max(enemy.x, target.x) &&
                std::min(enemy.y, target.y) <= y && y <= std::max(enemy.y, target.y)) {
                return true;
            }
        }
    }

    return false;
}

bool Enemy::hasLineOfSight(Position target, const std::shared_ptr<std::vector<std::unique_ptr<Wall>>>& walls) {
    return std::all_of(walls->begin(), walls->end(), [this, target](const std::unique_ptr<Wall>& wall) {
        return !lineIntersectsWall(position, target, *wall);});
}

void Enemy::setPosition(Position newPosition) { this->position = newPosition; };
void Enemy::receiveDamage(int damage) { hp -= damage; };

Position Enemy::getPosition() const { return position; };
int Enemy::getSize() const { return size; };
bool Enemy::isMovable() const { return movable; };
bool Enemy::isDead() const { return hp <= 0; };
float Enemy::getSpeedBullet() const { return speedBullet; };  
int Enemy::getHp() const { return hp; };