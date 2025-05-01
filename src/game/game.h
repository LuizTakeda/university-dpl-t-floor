#ifndef GAME_H
#define GAME_H

typedef enum
{
  GIS_NOTHING = 0,
  GIS_CLICK,
  GIS_HOLD,
  GIS_PRESS,
  GIS_RELEASED,
} GameInputStates;

typedef struct
{
  GameInputStates left;
  GameInputStates right;
  GameInputStates up;
  GameInputStates down;
  GameInputStates start;
  GameInputStates ok;
} GameInputs;

#endif