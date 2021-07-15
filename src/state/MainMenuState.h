#pragma once
#include "State.h"
class MainMenuState :
    public State
{
private:
    Text startText;
    RectangleShape startButton;

public:
    MainMenuState(RenderWindow* window, Font& font);
    void update(float dt);
    void draw(RenderStates states = RenderStates::Default) const;
};

