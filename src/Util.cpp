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

// our initial starting seed is 5323
static unsigned int nSeed = 5323;
void srandInt(unsigned seek)
{
    nSeed = seek;
}
unsigned int randInt()
{
    // Take the current seed and generate a new value from it
    // Due to our use of large constants and overflow, it would be
    // very hard for someone to predict what the next number is
    // going to be from the previous one.
    nSeed = (8253729 * nSeed + 2396403);

    // Take the seed and return a value between 0 and 32767
    return nSeed % 32767;
}
