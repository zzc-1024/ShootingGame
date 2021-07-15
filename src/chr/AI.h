#ifndef _AI_H_
#define _AI_H_
#include "Character.h"
class AI :
    public Character
{
public:
    static int globalHP;
    static int globalAttack;
    static float globalSpeed;

    AI();
};
#endif //_AI_H_

