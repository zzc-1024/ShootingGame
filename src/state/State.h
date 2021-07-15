#ifndef _STATE_H_
#define _STATE_H_
#include "Util.h"
class State
{
protected:
	RenderWindow* window;
	Font font;

	Vector2f mPos;

	void updateMouse();

public:
	State() = delete;
	State(const State&) = delete;
	State(State&&) = delete;
	State(RenderWindow* window, Font& font);

	void setFont(Font font);
	Font getFont() const;

	void setWindow(RenderWindow* window);
	RenderWindow* getWindow() const;

	virtual void update(float dt) = 0;
	virtual void draw(RenderStates = RenderStates::Default) const = 0;
};


#endif //_STATE_H_