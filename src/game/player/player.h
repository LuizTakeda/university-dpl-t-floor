#ifndef PLAYER_H
#define PLAYER_H

#include "../game.h"

void player_setup();

void player_clean();

GamePlayerInfo player_logic(const GameInputs *inputs);

void player_hit();

#endif