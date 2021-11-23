#include "Npc.h"

Npc::Npc()
{
    this->HP = 100;
    this->position = glm::vec3(0.0f,  0.09f,  0.0f);
    this->dir = true;
    this->ifAttack = false;
}

Npc::~Npc()
{
    ;
}

glm::vec3 Npc::getNpcPosition()
{
    return this->position;
}

bool Npc::getNpcDirection()
{
    return this->dir;
}


bool Npc::ifNpcAttacking()
{
    return this->ifAttack;
}

void Npc::updateNpcPosition(float stepX, float stepY)
{
    this->position.x = stepX;
    this->position.y = stepY;
}

void Npc::updateNpcDirection(bool dir)
{
    this->dir = dir;
}

void Npc::updateNpcAttack(bool ifAttack)
{
    this->ifAttack = ifAttack;
}