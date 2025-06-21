#include "enemies_internals.h"

#include <genesis.h>
#include <resources.h>

//**************************************************
// Defines
//**************************************************

#define BALL_PROJECTILE_LIMIT 10

//**************************************************
// Static Functions
//**************************************************

static void frame_change(Sprite *sprite);

//**************************************************
// Globals
//**************************************************

Enemy _ball_projectile[BALL_PROJECTILE_LIMIT];
u16 **_sprite_indexes;
u16 _alive_quantity;
u16 _spawn_countdown;

//**************************************************
// Functions
//**************************************************

/**
 *
 */
void enemy_ball_projectile_setup()
{
  u16 num_tiles;
  _sprite_indexes = SPR_loadAllFrames(&spr_projectile_01, _tile_index, &num_tiles);
  _tile_index += num_tiles;

  _alive_quantity = 0;
  _spawn_countdown = 0;

  for (int i = 0; i < BALL_PROJECTILE_LIMIT; i++)
  {
    _ball_projectile[i].dead = true;
  }
}

/**
 *
 */
void enemy_ball_projectile_clean()
{
  _alive_quantity = 0;

  for (u8 i = 0; i < BALL_PROJECTILE_LIMIT; i++)
  {
    if (_ball_projectile[i].dead)
    {
      continue;
    }

    _ball_projectile[i].dead = true;
    SPR_releaseSprite(_ball_projectile[i]._sprite);
  }

  SPR_defragVRAM();
}

/**
 *
 */
bool enemy_ball_projectile_spawn(fix16 x_velocity, fix16 y_velocity)
{
  return false;
}

/**
 *
 */
EnemiesEvents enemy_ball_projectile_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents return_value;

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