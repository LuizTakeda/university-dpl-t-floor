#include "enemies_internals.h"

#include <genesis.h>
#include <resources.h>

//**************************************************
//  Defines
//**************************************************

#define LIMIT 4
#define SPAWN_RATE 100
#define FLOORS_NUM 4

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

static Enemy _jumpers[LIMIT];
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
    _jumpers[i].dead = true;
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
    if (_jumpers[i].dead)
    {
      continue;
    }

    _jumpers[i].dead = true;
    SPR_releaseSprite(_jumpers[i]._sprite);
  }

  SPR_defragVRAM();
}

/**
 *
 */
bool enemy_jumper_spawn(GameLevel game_level)
{
  if (_alive_quantity >= LIMIT)
  {
    return false;
  }

  if (_spawn_countdown > 0)
  {
    _spawn_countdown--;
    return false;
  }

  _spawn_countdown = SPAWN_RATE;

  // Find the dead enemy position
  int i;
  for (i = 0; i < LIMIT && !_jumpers[i].dead; i++)
    ;

  _jumpers[i].dead = false;
  _jumpers[i].data = 0;
  _jumpers[i].state = ENEMY_STATE_SPAWNING;

  _jumpers[i].x = FIX16(80 + (16 * (random() % 9)));
  _jumpers[i].y = FIX16(72 + (32 * (random() % 4)));

  _jumpers[i].velocity_x = FIX16(0.3);
  _jumpers[i].velocity_y = FIX16(0.7);

  _jumpers[i]
      ._sprite = SPR_addSprite(
      &spr_enemy_06,
      fix16ToInt(_jumpers[i].x), fix16ToInt(_jumpers[i].y),
      TILE_ATTR(PAL3, 0, false, false));

  _jumpers[i].hit_box_left_x = fix16ToInt(_jumpers[i].x);
  _jumpers[i].hit_box_right_x = fix16ToInt(_jumpers[i].x) + _jumpers[i]._sprite->definition->w - 1;

  _jumpers[i].hit_box_top_y = fix16ToInt(_jumpers[i].y) + 2;
  _jumpers[i].hit_box_bottom_y = fix16ToInt(_jumpers[i].y) + _jumpers[i]._sprite->definition->h - 1 - 3;

  SPR_setAutoTileUpload(_jumpers[i]._sprite, FALSE);
  SPR_setFrameChangeCallback(_jumpers[i]._sprite, &frame_change);

  _alive_quantity++;

  return true;
}

/**
 *
 */
EnemiesEvents enemy_jumper_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents return_value = {
      .enemies_dead = 0,
      .player_hit = false};

  for (int i = 0; i < LIMIT; i++)
  {
    Enemy *enemy = &_jumpers[i];

    if (enemy->dead)
    {
      continue;
    }

    switch (enemy->state)
    {
    case ENEMY_STATE_SPAWNING:
      SPR_setAnim(enemy->_sprite, 0);
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