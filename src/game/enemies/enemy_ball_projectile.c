#include "enemies_internals.h"

#include <genesis.h>
#include <resources.h>

#include "game/globals.h"

//**************************************************
// Defines
//**************************************************

#define BALL_PROJECTILE_LIMIT 10

//**************************************************
// Static Functions
//**************************************************

static void ball_projectile_frame_change(Sprite *sprite);

//**************************************************
// Globals
//**************************************************

static Enemy _ball_projectile[BALL_PROJECTILE_LIMIT];
static u16 **_sprite_indexes;
static u16 _alive_quantity;

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
bool enemy_ball_projectile_spawn(u16 x, u16 y, fix16 x_velocity, fix16 y_velocity)
{
  if (_alive_quantity >= BALL_PROJECTILE_LIMIT)
  {
    return false;
  }

  // Find the dead enemy position
  int i;
  for (i = 0; i < BALL_PROJECTILE_LIMIT && !_ball_projectile[i].dead; i++)
    ;

  _ball_projectile[i].state = ENEMY_STATE_MOVING;

  _ball_projectile[i].dead = false;

  _ball_projectile[i].x = FIX16(x);
  _ball_projectile[i].y = FIX16(y);

  _ball_projectile[i].velocity_x = x_velocity;
  _ball_projectile[i].velocity_y = y_velocity;

  _ball_projectile[i].data = 0;

  _ball_projectile[i]._sprite = SPR_addSprite(
      &spr_projectile_01,
      fix16ToInt(_ball_projectile[i].x), fix16ToInt(_ball_projectile[i].y),
      TILE_ATTR(PAL3, 0, false, false));

  SPR_setAutoTileUpload(_ball_projectile[i]._sprite, FALSE);
  SPR_setFrameChangeCallback(_ball_projectile[i]._sprite, &ball_projectile_frame_change);
  SPR_setAlwaysOnTop(_ball_projectile[i]._sprite);

  _alive_quantity++;

  XGM2_playPCMEx(EFFECT_SHOOT);

  return true;
}

/**
 *
 */
EnemiesEvents enemy_ball_projectile_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents return_value;
  return_value.enemies_dead = 0;
  return_value.player_hit = false;

  for (int i = 0; i < BALL_PROJECTILE_LIMIT; i++)
  {
    Enemy *enemy = &_ball_projectile[i];

    if (enemy->dead)
    {
      continue;
    }

    switch (enemy->state)
    {
    case ENEMY_STATE_MOVING:
      SPR_setAnim(enemy->_sprite, 0);

      if (did_enemy_hit_player(enemy, player_info))
      {
        enemy->state = ENEMY_STATE_CLEAN;
        return_value.player_hit = true;
      }

      EnemyPlayerHit hit = did_player_hit_enemy(enemy, player_info);

      if (hit == ENEMY_PLAYER_HIT_LEFT)
      {
        enemy->state = ENEMY_STATE_DYING_LEFT;
        break;
      }

      if (hit == ENEMY_PLAYER_HIT_RIGHT)
      {
        enemy->state = ENEMY_STATE_DYING_RIGHT;
        break;
      }

      enemy->x += enemy->velocity_x;
      enemy->y += enemy->velocity_y;

      if (fix16ToInt(enemy->x) <= 80 || fix16ToInt(enemy->x) >= 224)
      {
        enemy->state = ENEMY_STATE_CLEAN;
      }

      if (fix16ToInt(enemy->y) <= 53 || fix16ToInt(enemy->y) >= 168)
      {
        enemy->state = ENEMY_STATE_CLEAN;
      }

      break;

    case ENEMY_STATE_DYING_LEFT:
      SPR_setAnim(enemy->_sprite, 1);

      if (enemy->_sprite->frameInd >= 2)
      {
        enemy->state = ENEMY_STATE_CLEAN;
      }

      break;

    case ENEMY_STATE_DYING_RIGHT:
      SPR_setAnim(enemy->_sprite, 2);

      if (enemy->_sprite->frameInd >= 2)
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

    enemy->hit_box_left_x = fix16ToInt(enemy->x) + 1;
    enemy->hit_box_right_x = fix16ToInt(enemy->x) + enemy->_sprite->definition->w - 2;

    enemy->hit_box_top_y = fix16ToInt(enemy->y) + 1;
    enemy->hit_box_bottom_y = fix16ToInt(enemy->y) + enemy->_sprite->definition->h - 2;

    SPR_setPosition(enemy->_sprite, fix16ToInt(enemy->x), fix16ToInt(enemy->y));
  }

  return return_value;
}

//**************************************************
//  Static Functions
//**************************************************

/**
 *
 */
static void ball_projectile_frame_change(Sprite *sprite)
{
  u16 tileIndex = _sprite_indexes[sprite->animInd][sprite->frameInd];

  SPR_setVRAMTileIndex(sprite, tileIndex);
}