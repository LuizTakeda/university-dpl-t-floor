#include "enemies_internals.h"

#include <genesis.h>
#include <resources.h>

//**************************************************
//  Defines
//**************************************************

#define LIMIT 1
#define SPAWN_RATE 300
#define TELEPORT_COUNTDOWN 100

//**************************************************
//  Enums
//**************************************************

//**************************************************
//  Static Functions
//**************************************************

static void frame_change(Sprite *sprite);

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
void enemy_teleporter_setup()
{
}

/**
 *
 */
void enemy_teleporter_clean()
{
}

/**
 *
 */
bool enemy_teleporter_spawn(GameLevel game_level)
{
  return false;
}

/**
 *
 */
EnemiesEvents enemy_teleporter_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents return_value = {
      .enemies_dead = 0,
      .player_hit = false,
  };

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