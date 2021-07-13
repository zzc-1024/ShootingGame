#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "Character.h"
class Player :
    public Character
{
public:
    static int globalHP;
    static int globalAttack;
    static float globalSpeed;
    static float globalCD;
    float cd;
    float cdRemain;
    
    Player();
};

#endif //_PLAYER_H_
