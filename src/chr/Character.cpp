#include "Character.h"

Character::Character()
{
	this->hp = 10;
	this->attack = 1;
	this->speed = 100.f;
	this->stp = 0;
	this->stateCounter = 0.f;
	this->isReverse = false;

	shape.setRadius(20.f);
	shape.setOrigin(Vector2f(20.f, 20.f));
	shape.setFillColor(Color(rand() % 256, rand() % 256, rand() % 256, 255));
	shape.setOutlineColor(Color::Black);
	shape.setOutlineThickness(1);
}

Character::~Character()
{//³¢ÊÔ½â¾öÄÚ´æĞ¹Â©
	vector<Sprite>().swap(st);
}

void Character::resetColor()
{
	shape.setFillColor(Color(rand() % 256, rand() % 256, rand() % 256));
}

void Character::loadTexture(Texture& t, int x, int y, int w, int h, int n)
{
	st.clear();
	Sprite sprite;
	for (int i = 0; i < n; i++)
	{
		sprite.setTexture(t);
		sprite.setTextureRect(IntRect(x + i * w, y, w, h));
		Util::setCenter(sprite);
		st.push_back(sprite);
	}
}

void Character::update(float& dt)
{
	updateState(dt);
}

void Character::updateState(float& dt)
{
	float T = 0.1f * st.size();
	stateCounter += dt;
	while (stateCounter >= T)
		stateCounter -= T;
	stp = static_cast<int>(stateCounter * 10);
	st[stp].setPosition(shape.getPosition());
	if (isReverse)
		st[stp].setScale(-3.f, 3.f);
	else
		st[stp].setScale(3.f, 3.f);
	//return st[static_cast<int>(stateCounter * 10)];
}

void Character::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(st[stp], states);
}
