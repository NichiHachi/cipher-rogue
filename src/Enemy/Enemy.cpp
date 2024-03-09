#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>
#include <queue>
#include <functional>
#include <vector>
#include <iostream>
#include <set>

#include "../Position.h"
#include "../Projectile/Bullet.h"
#include "../Projectile/Bombshell.h"
#include "Enemy.h"
#include "../Player/Player.h"

Enemy::Enemy(Position position, float speed, float angle, float shootTimer, float speedBullet, int hp, int size, bool movable) : position(position), speed(speed), angle(angle), shootTimer(shootTimer), speedBullet(speedBullet), hp(hp), size(size), movable(movable){}

void Enemy::receiveDamageIfShot(std::shared_ptr<std::vector<std::unique_ptr<Bullet>>> bullets, std::shared_ptr<std::vector<std::unique_ptr<Bombshell>>> bombshells){
    Position diffPos;
    float hitBoxBoth;
    auto bullet = bullets->begin();

    while (bullet != bullets->end()) {
        diffPos = position - bullet->get()->getPosition();
        hitBoxBoth = bullet->get()->getSize() + size;
        // sqrt(x² + y²) < n <=> x² + y² < n² :
        if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth * hitBoxBoth) {
            receiveDamage(1);
            bullet = bullets->erase(bullet);
        }
        else {
            bullet++;
        }
    }

    for(unsigned int i = 0; i < bombshells->size(); i++){
        if(!bombshells->at(i)->hasExploded() || !bombshells->at(i)->isAlly()) continue;
        
        if (std::find(bombshells->at(i)->hitEnemies.begin(), bombshells->at(i)->hitEnemies.end(), this) != bombshells->at(i)->hitEnemies.end()) {
            continue;
        }

        diffPos = position - bombshells->at(i)->getPosition();
        hitBoxBoth = size + bombshells->at(i)->getSize();
        if(diffPos.x * diffPos.x + diffPos.y * diffPos.y < hitBoxBoth * hitBoxBoth){
            receiveDamage(2);
            bombshells->at(i)->hitEnemies.push_back(this);
        }
    }
}

bool Enemy::adjustPositionBasedOnWalls(std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls){
    bool positionAdjusted = false;
    Position wallPos;
    float angleEnemyWall, wallSize;
    for (unsigned int i = 0; i < walls->size(); i++) {
        wallPos = walls->at(i)->getPosition();
        angleEnemyWall = atan2(wallPos.y - position.y, position.x - wallPos.x);
        //If the enemy nearest point from the middle of the wall is in the wall
        if (walls->at(i)->isInWall(position + Position(-cos(angleEnemyWall),sin(angleEnemyWall))*size)){
            wallSize = walls->at(i)->getSize();
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

bool Enemy::adjustPositionBasedOnEnemies(std::shared_ptr<std::vector<std::unique_ptr<Enemy>>> enemies){
    bool positionAdjusted = false;
    float angleEnemyEnemy, distance, moveDistance;
    Position enemyPos, diffPos;
    int enemySize;
    for (unsigned int i = 0; i < enemies->size(); i++) {
        if (enemies->at(i).get() == this) {
            //Don't check collision with itself
            continue;
        }

        enemyPos = enemies->at(i)->getPosition();
        enemySize = enemies->at(i)->getSize();
        diffPos = position - enemyPos;
        
        //If the object is inside of the enemy
        if (diffPos.x * diffPos.x + diffPos.y * diffPos.y < (enemySize+size) * (enemySize+size)){
            angleEnemyEnemy = atan2(enemyPos.y - position.y, position.x - enemyPos.x);
            moveDistance = enemySize + size - sqrt(diffPos.x * diffPos.x + diffPos.y * diffPos.y);
            // Move the enemy gradually towards the outside

            //TO DO : Scale the movement with the deltaTime
            if(enemies->at(i)->isMovable()){
                enemies->at(i)->setPosition(enemyPos + Position(cos(M_PI + angleEnemyEnemy) * moveDistance / 4,
                                                                - sin(M_PI + angleEnemyEnemy) * moveDistance / 4));
                                                      
                position += Position(cos(angleEnemyEnemy) * moveDistance / 4,
                                     - sin(angleEnemyEnemy) * moveDistance / 4);
            }
            else{
                position += Position(cos(angleEnemyEnemy) * moveDistance / 2,
                                     - sin(angleEnemyEnemy) * moveDistance / 2);
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

float Enemy::getAngleToTarget(Position target){
    return atan2(position.y - target.y, target.x - position.x);
}

float Enemy::getAngleToFuturPlayerPosition(Player player){
    Position diffPos = player.getPosition() - position;
    float timeBulletTravel = sqrt(diffPos.x*diffPos.x + diffPos.y*diffPos.y)/speedBullet;

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
    return sqrt((start.x - target.x)*(start.x - target.x) + (start.y - target.y)*(start.y - target.y));
}

float dist_between(Node* nodeA, Node* nodeB) {
    float dx = nodeB->position.x - nodeA->position.x;
    float dy = nodeB->position.y - nodeA->position.y;
    return sqrt(dx * dx + dy * dy);
}

bool isIndexInWall(Position pos, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls){
    for(int w=0; w<walls->size(); w++){
        if(walls->at(w)->isIndexInWall(pos)){
            return true;
        }
    }
    return false;
}

std::vector<Node*> getNeighbors(Node* node, Position target ,std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls){
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
                        neighbors.push_back(new Node(neighborPos, node->gScore + 1.5, heuristicCostEstimate(neighborPos, target), node));
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

std::vector<Position> Enemy::aStar(Position target, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls){
    std::priority_queue<Node*, std::vector<Node*>, NodeComparator> openSet;
    std::vector<Node*> closedSet;
    std::vector<Position> openSetLookup;
    std::vector<Position> path;
    Position positionGrid = Position((int)position.x/50, (int)position.y/50);
    Position targetGrid = Position((int)target.x/50, (int)target.y/50);

    Node* startNode = new Node(positionGrid, 0, heuristicCostEstimate(positionGrid, targetGrid), nullptr);
    openSet.push(startNode);

    float weight = 5;

    while(!openSet.empty()){
        Node* current = openSet.top();
        openSet.pop();

        if(current->position == targetGrid){
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

float Enemy::pathFinding(Position target, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls, float deltaTime){
    if(hasLineOfSight(target, size, walls)){
        float angle = getAngleToTarget(target);
        position += Position(cos(angle),-sin(angle)) * speed * deltaTime * 60;
        return angle;
    } else {
        std::vector<Position> path = aStar(target, walls);   
        Position vector;

        std::vector<Position> pathReduced = {path[0]};
        for(int i=1; i<path.size(); i++){
            if(vector == path[i]-pathReduced[pathReduced.size()-1]){
                pathReduced[pathReduced.size()-1] = path[i];
            } else {
                vector = path[i]-pathReduced[pathReduced.size()-1];
                pathReduced.push_back(path[i]);
            }
        }

        //REDO THIS PART because when the enemy goes to an other tile, it reset the good trajectory 
        std::vector<Position> pathScaled = {position};
        for(int i=1; i<pathReduced.size()-1; i++){
            if(pathReduced[i].x == pathReduced[i].y || pathReduced[i].x == -pathReduced[i].y){
                pathScaled.push_back(pathReduced[i]*50 + Position(25,25));
            } else {
                vector = pathReduced[i]-pathReduced[i-1];
                //Left
                if(vector.x > 0){
                    vector = pathReduced[i+1]-pathReduced[i];
                    if(vector.y > 0){
                        pathScaled.push_back(pathReduced[i]*50+Position((size+5),50-(size+5)));
                    } else {
                        pathScaled.push_back(pathReduced[i]*50+Position((size+5),(size+5)));
                    }
                }
                //Right
                else if(vector.x < 0){
                    vector = pathReduced[i+1]-pathReduced[i];
                    if(vector.y > 0){
                        pathScaled.push_back(pathReduced[i]*50+Position(50-(size+5),50-(size+5)));
                    } else {
                        pathScaled.push_back(pathReduced[i]*50+Position(50-(size+5),(size+5)));
                    }
                } 
                //Down
                else if(vector.y > 0){
                    vector = pathReduced[i+1]-pathReduced[i];
                    if(vector.x > 0){
                        pathScaled.push_back(pathReduced[i]*50+Position(50-(size+5),50-(size+5)));
                    } else {
                        pathScaled.push_back(pathReduced[i]*50+Position((size+5),50-(size+5)));
                    }
                }
                //Up
                else{
                    vector = pathReduced[i+1]-pathReduced[i];
                    if(vector.x > 0){
                        pathScaled.push_back(pathReduced[i]*50+Position(50-(size+5),(size+5)));
                    } else {
                        pathScaled.push_back(pathReduced[i]*50+Position((size+5),(size+5)));
                    }
                }
            }
        }
        pathScaled.push_back(target);
        
        float distanceToMove = speed * deltaTime * 60;
        float distanceCheckpoint;
        Position chechpoint;
        while(distanceToMove > 0 && pathScaled.size() > 1){
            chechpoint = pathScaled[1]-position;
            distanceCheckpoint = chechpoint.x*chechpoint.x + chechpoint.y*chechpoint.y;
            if(distanceToMove*distanceToMove >= distanceCheckpoint){
                distanceToMove -= sqrt(distanceCheckpoint);
                position = pathScaled[1];
                pathScaled.erase(pathScaled.begin());
            } else {
                Position vector = pathScaled[1]-position;
                vector /= (float)sqrt(distanceCheckpoint);
                position += vector*distanceToMove;
                distanceToMove = 0;
            }
        }

        return getAngleToTarget(pathScaled[1]);
    }
}

bool Enemy::hasLineOfSight(Position target, int size, std::shared_ptr<std::vector<std::unique_ptr<Wall>>> walls){
    Position vectorLine = target - position;
    Position discretisationPos;
    int discretisation = 100;
    
    for(int i=0; i<discretisation; i++){
        discretisationPos = position + vectorLine*i/discretisation;
        for(int w=0; w<walls->size(); w++){
            if(walls->at(w)->isEntityInWall(discretisationPos, size)){
                return false;
            }
        }
    }
    
    return true;
}