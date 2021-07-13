#ifndef _CHARACTER_H_
#define _CHARACTER_H_
#include <iostream>
#include <vector>
#include "Util.h"

using namespace std;

class Character
{
public:
	int hp;
	int attack;
	float speed;
	CircleShape shape;
	Sprite st[4];
	float stateCounter;
	bool isReverse;
	//Sprite sprite;

	Character();
	void resetColor();
	void loadTexture(Texture& t, int x, int y, int w, int h, int n);
	Sprite& updateState(float& dt);
};
#endif //_CHARACTER_H_

