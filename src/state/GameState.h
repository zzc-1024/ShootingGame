#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include "state/State.h"
#include "chr/AI.h"

class GameState :
    public State
{
public:
    GameState(RenderWindow* window, Font font, stack<State*>* states);

    void update(float dt);
    void draw(RenderStates = RenderStates::Default) const;
};

#endif //_GAMESTATE_H_
