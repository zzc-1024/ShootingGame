#ifndef _MAINMENUSTATE_H_
#define _MAINMENUSTATE_H_
#include "State.h"
#include "GameState.h"
class MainMenuState :
    public State
{
private:
    Text startText;
    RectangleShape startButton;

public:
    MainMenuState(RenderWindow* window, Font& font, stack<State*>* states);
    void update(float dt);
    void draw(RenderStates states = RenderStates::Default) const;
};

#endif //_MAINMENUSTATE_H_