#include "enemies_internals.h"

#include <genesis.h>
#include <resources.h>

//**************************************************
//  Defines
//**************************************************

#define LIMIT 4
#define SPAWN_RATE 100
#define SHOT_COUNTDOWN 100

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

void enemy_vertical_shooter_setup()
{
  u16 num_tiles;
  _sprite_indexes = SPR_loadAllFrames(&spr_enemy_04, _tile_index, &num_tiles);
  _tile_index += num_tiles;

  _alive_quantity = 0;
  _spawn_countdown = 0;

  for (int i = 0; i < LIMIT; i++)
  {
    _shotters[i].dead = true;
  }
}

void enemy_vertical_shooter_clean()
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

bool enemy_vertical_shooter_spawn(GameLevel game_level)
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
  for (i = 0; i < LIMIT && !_shotters[i].dead; i++)
    ;

  _shotters[i].dead = false;

  _shotters[i].x = FIX16(96 + (16 * (random() % 7)));
  _shotters[i].y = FIX16(55);

  _shotters[i].data = 0;
  _shotters[i].state = ENEMY_STATE_SPAWNING;

  _shotters[i]
      ._sprite = SPR_addSprite(
      &spr_enemy_04,
      fix16ToInt(_shotters[i].x), fix16ToInt(_shotters[i].y),
      TILE_ATTR(PAL3, 0, false, false));

  _shotters[i].hit_box_left_x = fix16ToInt(_shotters[i].x);
  _shotters[i].hit_box_right_x = fix16ToInt(_shotters[i].x) + _shotters[i]._sprite->definition->w - 1;

  _shotters[i].hit_box_top_y = fix16ToInt(_shotters[i].y);
  _shotters[i].hit_box_bottom_y = fix16ToInt(_shotters[i].y) + _shotters[i]._sprite->definition->h - 1 - 3;

  SPR_setAutoTileUpload(_shotters[i]._sprite, FALSE);
  SPR_setFrameChangeCallback(_shotters[i]._sprite, &frame_change);

  _alive_quantity++;

  return true;

  return false;
}

EnemiesEvents enemy_vertical_shooter_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents return_value = {
      .enemies_dead = 0,
      .player_hit = false};

  for (int i = 0; i < LIMIT; i++)
  {
    Enemy *enemy = &_shotters[i];

    if (enemy->dead)
    {
      continue;
    }

    switch (enemy->state)
    {
    case ENEMY_STATE_SPAWNING:
      SPR_setAnim(enemy->_sprite, 0);

      if (did_player_hit_enemy(enemy, player_info))
      {
        return_value.enemies_dead++;
        enemy->state = ENEMY_STATE_DYING;
      }

      if (enemy->_sprite->frameInd >= 4)
      {
        enemy->state = ENEMY_STATE_SHOOTING;
      }

      break;

    case ENEMY_STATE_SHOOTING:
      SPR_setAnim(enemy->_sprite, 1);

      if (did_player_hit_enemy(enemy, player_info))
      {
        return_value.enemies_dead++;
        enemy->state = ENEMY_STATE_DYING;
      }

      if (enemy->_sprite->frameInd >= 5)
      {
        enemy_ball_projectile_spawn(enemy->hit_box_left_x, enemy->hit_box_top_y + 8, FIX16(0), FIX16(+1.5));
        enemy->state = ENEMY_STATE_IDLE;
      }

      break;

    case ENEMY_STATE_IDLE:
      if (did_player_hit_enemy(enemy, player_info))
      {
        return_value.enemies_dead++;
        enemy->state = ENEMY_STATE_DYING;
      }

      if (enemy->_sprite->frameInd >= 9)
      {
        SPR_setAnim(enemy->_sprite, 2);
        enemy->data = SHOT_COUNTDOWN;
      }

      if (enemy->_sprite->animInd != 2)
      {
        break;
      }

      if (enemy->data <= 0)
      {
        enemy->state = ENEMY_STATE_SHOOTING;
      }
      else
      {
        enemy->data--;
      }
      break;

    case ENEMY_STATE_DYING:
      SPR_setAnim(enemy->_sprite, 3);

      if (enemy->_sprite->frameInd >= 3)
      {
        enemy->state = ENEMY_STATE_CLEAN;
      }

      break;

    case ENEMY_STATE_CLEAN:
      SPR_releaseSprite(enemy->_sprite);
      enemy->dead = true;
      _alive_quantity--;
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