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
    int width, height;

    View view;

    Player player;
    list<AI> aiList;
    AI ai;
    float flushAI;
    
    list<Bullet> bulletList;

    TileMap tMap;

    Texture texture, fireBullet;
    
    SoundBuffer shootBuffer;
    Sound shoot;

    int score;

    float dt;

public:
    GameState(RenderWindow* window, Font font, stack<State*>* states);
    ~GameState();

    void update(float dt);
    void draw(RenderStates = RenderStates::Default);
};

#endif //_GAMESTATE_H_
