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
}

/**
 *
 */
void enemy_jumper_clean()
{
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