#include "bullet.h"

bullet::bullet()
{
	life = 2.f;
	speed = 300;
	stateCounter = 0.f;
	shape.setFillColor(Color(rand() % 256, rand() % 256, rand() % 256, 255));
	shape.setRadius(5.f);
	shape.setOrigin(5.f, 5.f);
	angle = 0.f;
}

void bullet::loadTexture(Texture& t, int x, int y, int w, int h, int n)
{
	Sprite sprite;
	sprite.setTexture(t);
	sprite.rotate(angle * 180 / pi);
	for (int i = 0; i < n; i++)
	{
		sprite.setTextureRect(IntRect(x + i * w, y, w, h));
		Util::setCenter(sprite);
		states.push_back(sprite);
	}
}

Sprite& bullet::updateState(float& dt)
{
	float T = 0.1f * states.size();
	stateCounter += dt;
	while (stateCounter >= T)
		stateCounter -= T;
	int tmp = static_cast<int>(stateCounter * 10);
	states[tmp].setPosition(shape.getPosition());
	return states[static_cast<int>(stateCounter * 10)];
}
