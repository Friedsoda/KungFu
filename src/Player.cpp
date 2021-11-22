#include "Player.h"

Player::Player()
{
    this->HP = 800;
    this->position = glm::vec3(0.0f,  0.09f,  0.0f);
    this->dir = true;
    this->ifJump = false;
    this->ifAttack = false;
}

Player::~Player()
{
    ;
}

glm::vec3 Player::getPlayerPosition()
{
    return this->position;
}

bool Player::getPlayerDirection()
{
    return this->dir;
}

bool Player::ifPlayerJumping()
{
    return this->ifJump;
}

bool Player::ifPlayerAttacking()
{
    return this->ifAttack;
}

void Player::updatePlayerPosition(float stepX, float stepY)
{
    this->position.x = stepX;
    this->position.y = stepY;
}

void Player::updatePlayerDirection(bool dir)
{
    this->dir = dir;
}

void Player::updatePlayerJump(bool ifJump)
{
    this->ifJump = ifJump;
}

void Player::updatePlayerAttack(bool ifAttack)
{
    this->ifAttack = ifAttack;
}