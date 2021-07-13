#ifndef _STATE_H_
#define _STATE_H_
#include "Util.h"
class State
{
public:
	virtual void update(float dt) = 0;
	virtual void draw() = 0;
};


#endif //_STATE_H_