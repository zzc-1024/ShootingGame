#ifndef _UTIL_H_
#define _UTIL_H_
#include <SFML/Graphics.hpp>
#include <cmath>

using namespace sf;

const double pi = acos(-1);

class Util
{
public:
	static void setCenter(Text& text);
	static void setCenter(Sprite& sprite);
	static void setCenter(Shape& shape);
};

//From https://stackoverflow.com/questions/15500621/c-c-algorithm-to-produce-same-pseudo-random-number-sequences-from-same-seed-on
void srandInt(unsigned seek);
unsigned int randInt();

#endif //_UTIL_H_
