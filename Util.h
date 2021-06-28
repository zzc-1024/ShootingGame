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

#endif //_UTIL_H_
