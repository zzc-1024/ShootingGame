#ifndef _CHARACTER_H_
#define _CHARACTER_H_
#include <iostream>
#include <vector>
#include "Util.h"

using namespace std;

class Character :
	public Drawable
{
public:
	int hp;
	int attack;
	float speed;
	CircleShape shape;//Åö×²Ïä
	vector<Sprite> st;
	int stp;//state pointer
	float stateCounter;
	bool isReverse;

	Character();
	void resetColor();
	void loadTexture(Texture& t, int x, int y, int w, int h, int n);
	void update(float& dt);
	void updateState(float& dt);
	void draw(RenderTarget& target, RenderStates states) const;
};
#endif //_CHARACTER_H_

