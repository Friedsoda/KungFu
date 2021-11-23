#pragma once

#include "glheader.h"

class Player
{
private:
    glm::vec3 position;
    int HP;
    bool dir;  // Player direction, True = right and False = left
    bool ifJump; // If player is jumping
    bool ifAttack; // If player is attacking
    bool ifFalling; // If player is falling (game over)

public:
    Player();
    ~Player();
    glm::vec3 getPlayerPosition();
    bool getPlayerDirection();
    bool ifPlayerJumping();
    bool ifPlayerAttacking();
    bool ifPlayerFalling();
    void updatePlayerPosition(float stepX, float stepY);
    void updatePlayerDirection(bool dir);
    void updatePlayerJump(bool ifJump);
    void updatePlayerAttack(bool ifAttack);
    void gameOver();
};