#ifndef _PLAYER_H
#define _PLAYER_H

#include <genesis.h>
#include <sprite_eng.h>

#include "gameobject.h"

#define PLAYER_SPEED FIX16(3)

extern GameObject player;

u16 Player_init();
void Player_get_input();
void Player_update();

#endif