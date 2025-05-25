#ifndef ENEMIES_H
#define ENEMIES_H

#include "../game.h"

typedef struct{
  bool player_hit;
  u8 enemies_dead;
}EnemiesEvents;

void enemies_setup(GameLevel level);

void enemies_clean();

EnemiesEvents enemies_logic(const GamePlayerInfo *player_info);

#endif