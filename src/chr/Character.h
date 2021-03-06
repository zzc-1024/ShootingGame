#ifndef _CHARACTER_H_
#define _CHARACTER_H_
#include <iostream>
//#include <vector>
#include "utils/Util.h"
#include "utils/SeqList.h"

using namespace std;

class Character :
	public Drawable
{
public:
	int hp;
	int attack;
	float speed;
	CircleShape shape;//??ײ??
	SeqList<Sprite> st;
	int stp;//state pointer
	float stateCounter;
	bool isReverse;

	Character();
	~Character();
	void resetColor();
	void loadTexture(Texture& t, int x, int y, int w, int h, int n);
	void update(float& dt);
	void updateState(float& dt);
	void draw(RenderTarget& target, RenderStates states) const;
};
#endif //_CHARACTER_H_

