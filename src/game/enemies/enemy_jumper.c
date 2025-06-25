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

  _jumpers[i].velocity_x = random() % 2 ? FIX16(0.3) : FIX16(-0.3);

  if (random() % 2)
  {
    _jumpers[i].y = FIX16(72);
    _jumpers[i].velocity_y = FIX16(0.7);
    _jumpers[i].data_16 = 0;
  }
  else
  {
    _jumpers[i].y = FIX16(168);
    _jumpers[i].velocity_y = FIX16(-0.7);
    _jumpers[i].data_16 = 3;
  }

  _jumpers[i].data_16 = (fix16ToInt(_jumpers[i].y) - 72) / 32;

  _jumpers[i]
      ._sprite = SPR_addSprite(
      &spr_enemy_06,
      fix16ToInt(_jumpers[i].x), fix16ToInt(_jumpers[i].y),
      TILE_ATTR(PAL3, 0, false, false));

  SPR_setAutoTileUpload(_jumpers[i]._sprite, FALSE);
  SPR_setFrameChangeCallback(_jumpers[i]._sprite, &frame_change);
  SPR_setAlwaysOnTop(_jumpers[i]._sprite);

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

      if (enemy->_sprite->frameInd >= 3)
      {
        enemy->state = ENEMY_STATE_MOVING;
      }

      break;

    case ENEMY_STATE_MOVING:
    {
      if (enemy->_sprite->animInd != 2)
      {
        SPR_setAnim(enemy->_sprite, 2);
      }

      if (enemy->_sprite->frameInd <= 4)
      {
        break;
      }

      if (enemy->_sprite->frameInd >= 5)
      {
        SPR_setAnim(enemy->_sprite, -1);
        SPR_setAnimAndFrame(enemy->_sprite,  2, 5);
      }

      u16 y = fix16ToInt(enemy->y);

      if (enemy->data_16 != 0 && y == 72)
      {
        enemy->data_16 = 0;
        enemy->state = ENEMY_STATE_IDLE;
        enemy->data = 100;
        break;
      }
      else if (enemy->data_16 != 1 && y == 104)
      {
        enemy->data_16 = 1;
        enemy->state = ENEMY_STATE_IDLE;
        enemy->data = 100;
        break;
      }
      else if (enemy->data_16 != 2 && y == 136)
      {
        enemy->data_16 = 2;
        enemy->state = ENEMY_STATE_IDLE;
        enemy->data = 100;
        break;
      }
      else if (enemy->data_16 != 3 && y == 168)
      {
        enemy->data_16 = 3;
        enemy->state = ENEMY_STATE_IDLE;
        enemy->data = 100;
        break;
      }

      enemy->x += enemy->velocity_x;
      enemy->y += enemy->velocity_y;

      u16 x = fix16ToInt(enemy->x);
      y = fix16ToInt(enemy->y);

      if (x < 80)
      {
        x = 80;
        enemy->x = FIX16(80);
        enemy->velocity_x = FIX16(0.3);
      }
      else if (x > 216)
      {
        x = 216;
        enemy->x = FIX16(216);
        enemy->velocity_x = FIX16(-0.3);
      }

      if (y <= 72)
      {
        enemy->velocity_y = FIX16(0.7);
      }
      else if (y >= 168)
      {
        enemy->velocity_y = FIX16(-0.7);
      }

      enemy->hit_box_left_x = x;
      enemy->hit_box_right_x = x + enemy->_sprite->definition->w - 1;

      enemy->hit_box_top_y = y + 2;
      enemy->hit_box_bottom_y = y + enemy->_sprite->definition->h - 1 - 3;

      SPR_setPosition(enemy->_sprite, x, y);
    }
    break;

    case ENEMY_STATE_IDLE:
      if (enemy->_sprite->animInd == 2 && enemy->_sprite->frameInd >= 7)
      {
        SPR_setAnim(enemy->_sprite, 1);
        break;
      }

      if (enemy->_sprite->animInd != 1)
      {
        break;
      }

      if (enemy->data > 0)
      {
        enemy->data--;
        break;
      }

      enemy->state = ENEMY_STATE_MOVING;

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