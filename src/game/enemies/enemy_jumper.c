#include "enemies_internals.h"

#include <genesis.h>
#include <resources.h>

//**************************************************
//  Defines
//**************************************************

#define LIMIT 4

//**************************************************
//  Enums
//**************************************************

//**************************************************
//  Static Functions
//**************************************************

static void jumper_frame_change(Sprite *sprite);

//**************************************************
//  Globals
//**************************************************

static Enemy _shotters[LIMIT];
static u16 **_sprite_indexes;
static u16 _alive_quantity;
static u16 _spawn_countdown;

//**************************************************
//  Functions
//**************************************************

/**
 *
 */
void enemy_jumper_setup()
{
  u16 num_tiles;
  _sprite_indexes = SPR_loadAllFrames(&spr_enemy_06, _tile_index, &num_tiles);
  _tile_index += num_tiles;

  _alive_quantity = 0;
  _spawn_countdown = 0;

  for (int i = 0; i < LIMIT; i++)
  {
    _shotters[i].dead = true;
  }
}

/**
 *
 */
void enemy_jumper_clean()
{
  _alive_quantity = 0;

  for (u8 i = 0; i < LIMIT; i++)
  {
    if (_shotters[i].dead)
    {
      continue;
    }

    _shotters[i].dead = true;
    SPR_releaseSprite(_shotters[i]._sprite);
  }

  SPR_defragVRAM();
}

/**
 *
 */
bool enemy_jumper_spawn(GameLevel game_level)
{
  return false;
}

/**
 *
 */
EnemiesEvents enemy_jumper_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents return_value = {
      .enemies_dead = 0,
      .player_hit = false};

  return return_value;
}

//**************************************************
//  Static Functions
//**************************************************

/**
 *
 */
static void jumper_frame_change(Sprite *sprite)
{
  u16 tileIndex = _sprite_indexes[sprite->animInd][sprite->frameInd];

  SPR_setVRAMTileIndex(sprite, tileIndex);
}