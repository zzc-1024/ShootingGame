#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include "state/State.h"
#include "chr/AI.h"
#include "chr/Player.h"
#include "Bullet.h"
#include "map/TileMap.h"
#include <list>

class GameState :
    public State
{
private:
    Player player;
    list<AI> aiList;
    list<Bullet> bulletList;

    TileMap tMap;

public:
    GameState(RenderWindow* window, Font font, stack<State*>* states);

    void update(float dt);
    void draw(RenderStates = RenderStates::Default) const;
};

#endif //_GAMESTATE_H_
