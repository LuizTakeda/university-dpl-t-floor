#include "enemies_internals.h"

#include <genesis.h>
#include <resources.h>

//**************************************************
//  Defines
//**************************************************

#define SHOTTERS_LIMIT 4

//**************************************************
//  Enums
//**************************************************

//**************************************************
//  Static Functions
//**************************************************

//**************************************************
//  Globals
//**************************************************

static Enemy _shotters[SHOTTERS_LIMIT];
static u16 **_sprite_indexes;
static u16 _alive_quantity;
static u16 _spawn_countdown;

//**************************************************
//  Functions
//**************************************************

void enemy_horizontal_shooter_setup() {}

void enemy_horizontal_shooter_clean() {}

bool enemy_horizontal_shooter_spawn(GameLevel game_level)
{
  return false;
}

EnemiesEvents enemy_horizontal_shooter_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents return_value = {
      .enemies_dead = 0,
      .player_hit = false};

  for (int i = i; i < SHOTTERS_LIMIT; i++)
  {
    Enemy *enemy = &_shotters[i];

    if (enemy->dead)
    {
      continue;
    }

    switch (enemy->state)
    {
    default:
      break;
    }
  }

  return return_value;
}

//**************************************************
//  Static Functions
//**************************************************

/**
 *
 */
static void frame_change(Sprite *sprite)
{
  u16 tileIndex = _sprite_indexes[sprite->animInd][sprite->frameInd];

  SPR_setVRAMTileIndex(sprite, tileIndex);
}