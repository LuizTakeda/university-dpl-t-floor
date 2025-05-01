#ifndef PLAYER_H
#define PLAYER_H

#include "../game.h"

void player_setup();

GamePlayerInfo player_logic(const GameInputs *inputs);

#endif