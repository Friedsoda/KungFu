#pragma once

#include "glheader.h"

class Npc
{
private:
    glm::vec3 position;
    int HP;
    bool dir;  // Player direction, True = right and False = left
    bool ifAttack; // If player is attacking

public:
    Npc();
    ~Npc();
    glm::vec3 getNpcPosition();
    bool getNpcDirection();
    bool ifNpcAttacking();
    void updateNpcPosition(float stepX, float stepY);
    void updateNpcDirection(bool dir);
    void updateNpcAttack(bool ifAttack);
};