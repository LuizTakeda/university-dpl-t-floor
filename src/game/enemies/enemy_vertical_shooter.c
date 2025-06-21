#include "enemies_internals.h"

#include <genesis.h>
#include <resources.h>

//**************************************************
//  Defines
//**************************************************

#define SHOTTERS_LIMIT 4
#define SHOTTERS_SPAWN_RATE 100

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

Enemy _shotters[SHOTTERS_LIMIT];
u16 **_sprite_indexes;
u16 _alive_quantity;
u16 _spawn_countdown;

//**************************************************
//  Functions
//**************************************************

/**
 *
 */
void enemy_vertical_shooter_setup()
{
  u16 num_tiles;
  _sprite_indexes = SPR_loadAllFrames(&spr_enemy_03, _tile_index, &num_tiles);
  _tile_index += num_tiles;

  _alive_quantity = 0;
  _spawn_countdown = 0;

  for (int i = 0; i < SHOTTERS_LIMIT; i++)
  {
    _shotters[i].dead = true;
  }
}

/**
 *
 */
void enemy_vertical_shooter_clean()
{
  _alive_quantity = 0;

  for (u8 i = 0; i < SHOTTERS_LIMIT; i++)
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
bool enemy_vertical_shooter_spawn(GameLevel game_level)
{
  if (_alive_quantity >= SHOTTERS_LIMIT)
  {
    return false;
  }

  if (_spawn_countdown > 0)
  {
    _spawn_countdown--;
    return false;
  }

  _spawn_countdown = SHOTTERS_SPAWN_RATE;

  // Find the dead enemy position
  int i;
  for (i = 0; i < SHOTTERS_LIMIT && !_shotters[i].dead; i++)
    ;

  _shotters[i].dead = false;

  // Left or right wall
  if ((random() % 2))
  {
    _shotters[i].state = ENEMY_STATE_SPAWNING_LEFT;
    _shotters[i].x = FIX16(80);
  }
  else
  {
    _shotters[i].state = ENEMY_STATE_SPAWNING_RIGHT;
    _shotters[i].x = FIX16(216);
  }

  _shotters[i].y = FIX16(64 + (i * 32));

  _shotters[i]._sprite = SPR_addSprite(
      &spr_enemy_03,
      fix16ToInt(_shotters[i].x), fix16ToInt(_shotters[i].y),
      TILE_ATTR(PAL3, 0, false, false));

  SPR_setAutoTileUpload(_shotters[i]._sprite, FALSE);
  SPR_setFrameChangeCallback(_shotters[i]._sprite, &frame_change);

  _alive_quantity++;

  return true;
}

/**
 *
 */
EnemiesEvents enemy_vertical_shooter_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents return_value;

  for (int i = 0; i < SHOTTERS_LIMIT; i++)
  {
    Enemy *enemy = &_shotters[i];

    switch (enemy->state)
    {
    case ENEMY_STATE_SPAWNING_LEFT:
      SPR_setAnim(enemy->_sprite, 0);

      if (enemy->_sprite->frameInd >= 4)
      {
        enemy->state = ENEMY_STATE_SHOOTING_RIGHT;
      }
      break;

    case ENEMY_STATE_SPAWNING_RIGHT:
      SPR_setAnim(enemy->_sprite, 3);

      if (enemy->_sprite->frameInd >= 4)
      {
        enemy->state = ENEMY_STATE_SHOOTING_LEFT;
      }
      break;

    case ENEMY_STATE_SHOOTING_LEFT:
      SPR_setAnim(enemy->_sprite, 4);
      break;

    case ENEMY_STATE_SHOOTING_RIGHT:
      SPR_setAnim(enemy->_sprite, 1);
      break;

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