#ifndef _STATE_H_
#define _STATE_H_
#include "Util.h"
#include <stack>
#include "SFML/Audio.hpp"
using namespace std;
class State
{
protected:
	RenderWindow* window;
	Font font;

	Event event;

	Vector2f mPos;
	 
	void updateMouse();

public:
	bool isEnd;

	stack<State*>* states;

	State() = delete;
	State(const State&) = delete;
	State(State&&) = delete;
	State(RenderWindow* window, Font& font, stack<State*>* states);

	void setFont(Font font);
	Font getFont() const;

	void setWindow(RenderWindow* window);
	RenderWindow* getWindow() const;

	void setEvent(Event event);
	Event getEvent();

	virtual void update(float dt) = 0;
	virtual void draw(RenderStates = RenderStates::Default) = 0;
};


#endif //_STATE_H_