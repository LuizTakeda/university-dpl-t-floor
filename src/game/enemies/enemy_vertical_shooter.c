#include "enemies_internals.h"

#include <genesis.h>
#include <resources.h>

//**************************************************
//  Defines
//**************************************************

#define SHOTTERS_LIMIT 4
#define SHOTTERS_SPAWN_RATE 100
#define SHOTTERS_FIRE_COUNTDOWN 100

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

static Enemy _shotters[SHOTTERS_LIMIT];
static u16 **_sprite_indexes;
static u16 _alive_quantity;
static u16 _spawn_countdown;

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

  _shotters[i].data = 0;

  _shotters[i]._sprite = SPR_addSprite(
      &spr_enemy_03,
      fix16ToInt(_shotters[i].x), fix16ToInt(_shotters[i].y),
      TILE_ATTR(PAL3, 0, false, false));

  if (_shotters[i].state == ENEMY_STATE_SPAWNING_LEFT)
  {
    _shotters[i].hit_box_left_x = fix16ToInt(_shotters[i].x);
    _shotters[i].hit_box_right_x = fix16ToInt(_shotters[i].x) + _shotters[i]._sprite->definition->w - 1 - 3;
  }
  else
  {
    _shotters[i].hit_box_left_x = fix16ToInt(_shotters[i].x) + 3;
    _shotters[i].hit_box_right_x = fix16ToInt(_shotters[i].x) + _shotters[i]._sprite->definition->w - 1;
  }

  _shotters[i].hit_box_top_y = fix16ToInt(_shotters[i].y);
  _shotters[i].hit_box_bottom_y = fix16ToInt(_shotters[i].y) + _shotters[i]._sprite->definition->h - 1;

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
  return_value.enemies_dead = 0;
  return_value.player_hit = false;

  for (int i = 0; i < SHOTTERS_LIMIT; i++)
  {
    Enemy *enemy = &_shotters[i];

    if (enemy->dead)
    {
      continue;
    }

    switch (enemy->state)
    {
    case ENEMY_STATE_SPAWNING_LEFT:
      SPR_setAnim(enemy->_sprite, 0);

      if (did_player_hit_enemy(enemy, player_info))
      {
        enemy->state = ENEMY_STATE_DYING_LEFT;
        break;
      }

      if (enemy->_sprite->frameInd >= 4)
      {
        enemy->state = ENEMY_STATE_SHOOTING_RIGHT;
      }
      break;

    case ENEMY_STATE_SPAWNING_RIGHT:
      SPR_setAnim(enemy->_sprite, 4);

      if (did_player_hit_enemy(enemy, player_info))
      {
        enemy->state = ENEMY_STATE_DYING_RIGHT;
        break;
      }

      if (enemy->_sprite->frameInd >= 4)
      {
        enemy->state = ENEMY_STATE_SHOOTING_LEFT;
      }
      break;

    case ENEMY_STATE_SHOOTING_LEFT:
      SPR_setAnim(enemy->_sprite, 5);

      if (did_player_hit_enemy(enemy, player_info))
      {
        enemy->state = ENEMY_STATE_DYING_RIGHT;
        break;
      }

      return_value.player_hit |= did_enemy_hit_player(enemy, player_info);

      if (enemy->_sprite->frameInd == 5)
      {
        enemy_ball_projectile_spawn(fix16ToInt(enemy->x), fix16ToInt(enemy->y), FIX16(-1.5), FIX16(0));
        enemy->state = ENEMY_STATE_IDLE_RIGHT;
      }

      break;

    case ENEMY_STATE_SHOOTING_RIGHT:
      SPR_setAnim(enemy->_sprite, 1);

      if (did_player_hit_enemy(enemy, player_info))
      {
        enemy->state = ENEMY_STATE_DYING_LEFT;
        break;
      }

      return_value.player_hit |= did_enemy_hit_player(enemy, player_info);

      if (enemy->_sprite->frameInd == 5)
      {
        enemy_ball_projectile_spawn(fix16ToInt(enemy->x) + 8, fix16ToInt(enemy->y), FIX16(1.5), FIX16(0));
        enemy->state = ENEMY_STATE_IDLE_LEFT;
      }

      break;

    case ENEMY_STATE_IDLE_LEFT:
      if (did_player_hit_enemy(enemy, player_info))
      {
        enemy->state = ENEMY_STATE_DYING_LEFT;
        break;
      }

      return_value.player_hit |= did_enemy_hit_player(enemy, player_info);

      if (enemy->_sprite->frameInd >= 9)
      {
        enemy->data = SHOTTERS_FIRE_COUNTDOWN;
        SPR_setAnim(enemy->_sprite, 2);
      }

      if (enemy->_sprite->animInd != 2)
      {
        break;
      }

      if (enemy->data <= 0)
      {
        enemy->state = ENEMY_STATE_SHOOTING_RIGHT;
      }
      else
      {
        enemy->data--;
      }

      break;

    case ENEMY_STATE_IDLE_RIGHT:
      if (did_player_hit_enemy(enemy, player_info))
      {
        enemy->state = ENEMY_STATE_DYING_RIGHT;
        break;
      }

      return_value.player_hit |= did_enemy_hit_player(enemy, player_info);

      if (enemy->_sprite->frameInd >= 9)
      {
        enemy->data = SHOTTERS_FIRE_COUNTDOWN;
        SPR_setAnim(enemy->_sprite, 6);
      }

      if (enemy->_sprite->animInd != 6)
      {
        break;
      }

      if (enemy->data <= 0)
      {
        enemy->state = ENEMY_STATE_SHOOTING_LEFT;
      }
      else
      {
        enemy->data--;
      }
      break;

    case ENEMY_STATE_DYING_LEFT:
      SPR_setAnim(enemy->_sprite, 3);

      if (enemy->_sprite->frameInd >= 3)
      {
        SPR_releaseSprite(enemy->_sprite);
        enemy->dead = true;
        _alive_quantity--;
        return_value.enemies_dead++;
      }

      break;

    case ENEMY_STATE_DYING_RIGHT:
      SPR_setAnim(enemy->_sprite, 7);

      if (enemy->_sprite->frameInd >= 3)
      {
        SPR_releaseSprite(enemy->_sprite);
        enemy->dead = true;
        _alive_quantity--;
        return_value.enemies_dead++;
      }
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