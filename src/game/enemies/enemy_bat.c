#include "enemies_internals.h"

#include <genesis.h>
#include <resources.h>

//**************************************************
//
//**************************************************

#define BAT_LIMIT 10

//**************************************************
//
//**************************************************

Enemy _bat_list[BAT_LIMIT];
u16 **_bat_sprite_indexes;

//**************************************************
//
//**************************************************

/**
 *
 */
void enemy_bat_setup()
{
  u16 num_tiles;
  _bat_sprite_indexes = SPR_loadAllFrames(&spr_enemy_02, _tile_index, &num_tiles);
  _tile_index += num_tiles;

  for (u8 i = 0; i < BAT_LIMIT; i++)
  {
    _bat_list[i].dead = true;
  }
}

/**
 *
 */
void enemy_bat_clean()
{
}

/**
 *
 */
bool enemy_bat_spawn(GameLevel game_level)
{
  

  return false;
}

/**
 *
 */
EnemiesEvents enemy_bat_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents return_value;
  return_value.enemies_dead = 0;
  return_value.player_hit = false;

  for (u8 i = 0; i < BAT_LIMIT; i++)
  {
    switch (_bat_list[i].state)
    {
      ENEMY_STATE_SPAWNING = 0,
      ENEMY_RUNNING_RIGHT,
      ENEMY_RUNNING_LEFT,
      ENEMY_SHOOTING,
      ENEMY_IDLE,
      ENEMY_DYING,

      default : break;
    }
  }

  return return_value;
}

//**************************************************
//
//**************************************************

static bool bat_create()
{
}