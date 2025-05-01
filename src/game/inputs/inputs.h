#ifndef INPUTS_H
#define INPUTS_H

#include <genesis.h>

#include "../game.h"

/**
 * Realiza a leitura dos inputs e controle do estado de cada um deles
 */
GameInputs game_inputs_read();

inline bool game_inputs_click(const GameInputStates state)
{
  return state == GIS_CLICK;
}

inline bool game_inputs_hold(const GameInputStates state)
{
  return state == GIS_HOLD;
}

inline bool game_inputs_released(const GameInputStates state)
{
  return state == GIS_RELEASED;
}

#endif