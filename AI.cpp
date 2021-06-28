#include "AI.h"

int AI::globalHP = 3;
int AI::globalAttack = 1;
float AI::globalSpeed = 100.f;

AI::AI()
{
	this->hp = AI::globalHP;
	this->attack = AI::globalAttack;
	this->speed = AI::globalSpeed;
}