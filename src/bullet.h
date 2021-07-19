#ifndef _BULLET_H_
#define _BULLET_H_
#include <vector>
#include "utils/Util.h"

using namespace sf;
using namespace std;

class Bullet
{
public:
	float speed;
	float life;
	float angle;
	CircleShape shape;
	vector<Sprite> states;
	float stateCounter;

	Bullet();
	void loadTexture(Texture& t, int x, int y, int w, int h, int n);
	Sprite& updateState(float& dt);
};

#endif