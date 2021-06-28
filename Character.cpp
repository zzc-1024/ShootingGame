#include "Character.h"


Character::Character()
{
	this->hp = 10;
	this->attack = 1;
	this->speed = 100.f;
	this->stateCounter = 0.f;
	this->isReverse = false;
	shape.setRadius(20.f);
	shape.setOrigin(Vector2f(20.f, 20.f));
	shape.setFillColor(Color(rand() % 256, rand() % 256, rand() % 256, 255));
	shape.setOutlineColor(Color::Black);
	shape.setOutlineThickness(1);
}

void Character::resetColor()
{
	shape.setFillColor(Color(rand() % 256, rand() % 256, rand() % 256));
}

void Character::loadTexture(Texture& t, int x, int y, int w, int h, int n)
{
	for (int i = 0; i < n; i++)
	{
		st[i].setTexture(t);
		st[i].setTextureRect(IntRect(x + i * w, y, w, h));
		Util::setCenter(st[i]);
	}
}

Sprite& Character::updateState(float& dt)
{
	float T = 0.1f * 4;
	stateCounter += dt;
	while (stateCounter >= T)
		stateCounter -= T;
	int tmp = static_cast<int>(stateCounter * 10);
	st[tmp].setPosition(shape.getPosition());
	if (isReverse)
		st[tmp].setScale(-3.f, 3.f);
	else
		st[tmp].setScale(3.f, 3.f);
	//sprite = st[static_cast<int>(stateCounter * 10)];
	return st[static_cast<int>(stateCounter * 10)];
}
