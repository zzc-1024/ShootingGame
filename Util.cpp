#include "Util.h"

void Util::setCenter(Text& text)
{
	FloatRect lb = text.getLocalBounds();
	text.setOrigin(lb.left + lb.width / 2, lb.top + lb.height / 2);
}

void Util::setCenter(Sprite& sprite)
{
	FloatRect lb = sprite.getLocalBounds();
	sprite.setOrigin(lb.left + lb.width / 2, lb.top + lb.height / 2);
}

void Util::setCenter(Shape& shape)
{
	FloatRect lb = shape.getLocalBounds();
	shape.setOrigin(lb.left + lb.width / 2, lb.top + lb.height / 2);
}

