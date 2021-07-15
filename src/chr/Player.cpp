#include "Player.h"

int Player::globalHP = 10;
int Player::globalAttack = 1;
float Player::globalSpeed = 100.f;
float Player::globalCD = 0.1f;

Player::Player()
{
	this->hp = Player::globalHP;
	this->attack = Player::globalAttack;
	this->speed = Player::globalSpeed;
	this->cd = Player::globalCD;
	this->cdRemain = 0.f;
}

