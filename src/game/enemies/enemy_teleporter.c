#include "enemies_internals.h"

#include <genesis.h>
#include <resources.h>

//**************************************************
//  Defines
//**************************************************

#define LIMIT 2
#define SPAWN_RATE 1100
#define TELEPORT_COUNTDOWN 200

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

static Enemy _teleporters[LIMIT];
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
  u16 num_tiles;
  _sprite_indexes = SPR_loadAllFrames(&spr_enemy_05, _tile_index, &num_tiles);
  _tile_index += num_tiles;

  _alive_quantity = 0;
  _spawn_countdown = 0;

  for (int i = 0; i < LIMIT; i++)
  {
    _teleporters[i].dead = true;
  }
}

/**
 *
 */
void enemy_teleporter_clean()
{
  _alive_quantity = 0;
  _spawn_countdown = 0;

  for (u8 i = 0; i < LIMIT; i++)
  {
    if (_teleporters[i].dead)
    {
      continue;
    }

    _teleporters[i].dead = true;
    SPR_releaseSprite(_teleporters[i]._sprite);
  }

  SPR_defragVRAM();
}

/**
 *
 */
bool enemy_teleporter_spawn(GameLevel game_level, const GamePlayerInfo *player_info)
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
  for (i = 0; i < LIMIT && !_teleporters[i].dead; i++)
    ;

  _teleporters[i].dead = false;
  _teleporters[i].data = 0;
  _teleporters[i].state = ENEMY_STATE_SPAWNING;

  _teleporters[i].x = intToFix16(player_info->left_x);
  _teleporters[i].y = intToFix16(player_info->top_y);

  _teleporters[i]
      ._sprite = SPR_addSprite(
      &spr_enemy_05,
      fix16ToInt(_teleporters[i].x), fix16ToInt(_teleporters[i].y),
      TILE_ATTR(PAL3, 0, false, false));

  SPR_setAutoTileUpload(_teleporters[i]._sprite, FALSE);
  SPR_setFrameChangeCallback(_teleporters[i]._sprite, &frame_change);
  SPR_setAlwaysOnTop(_teleporters[i]._sprite);

  _alive_quantity++;

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

  for (int i = 0; i < LIMIT; i++)
  {
    Enemy *enemy = &_teleporters[i];

    if (enemy->dead)
    {
      continue;
    }

    switch (enemy->state)
    {
    case ENEMY_STATE_SPAWNING:
    {
      SPR_setAnim(enemy->_sprite, 0);

      EnemyPlayerHit hit = did_player_hit_enemy(enemy, player_info);
      if (hit == ENEMY_PLAYER_HIT_LEFT)
      {
        enemy->state = ENEMY_STATE_DYING_LEFT;
        break;
      }
      else if (hit == ENEMY_PLAYER_HIT_RIGHT)
      {
        enemy->state = ENEMY_STATE_DYING_RIGHT;
        break;
      }

      if (enemy->_sprite->frameInd >= 3)
      {
        enemy->data = TELEPORT_COUNTDOWN;
        enemy->state = ENEMY_STATE_IDLE;
      }
    }
    break;

    case ENEMY_STATE_IDLE:
      SPR_setAnim(enemy->_sprite, 1);

      return_value.player_hit |= did_enemy_hit_player(enemy, player_info);

      if (enemy->data <= 0)
      {
        enemy->state = ENEMY_STATE_MOVING;
        break;
      }

      enemy->data--;

      break;

    case ENEMY_STATE_MOVING:
    {
      EnemyPlayerHit hit = did_player_hit_enemy(enemy, player_info);
      if (hit == ENEMY_PLAYER_HIT_LEFT)
      {
        enemy->state = ENEMY_STATE_DYING_LEFT;
        break;
      }
      else if (hit == ENEMY_PLAYER_HIT_RIGHT)
      {
        enemy->state = ENEMY_STATE_DYING_RIGHT;
        break;
      }

      if (enemy->_sprite->animInd != 2 && enemy->_sprite->animInd != 3)
      {
        SPR_setAnim(enemy->_sprite, 2);
      }

      if (enemy->_sprite->animInd == 2 && enemy->_sprite->frameInd >= 2)
      {
        SPR_setAnim(enemy->_sprite, 3);
        enemy->x = intToFix16(player_info->left_x);
        enemy->y = intToFix16(player_info->top_y);
      }

      if (enemy->_sprite->animInd == 3 && enemy->_sprite->frameInd >= 3)
      {
        enemy->data = TELEPORT_COUNTDOWN;
        enemy->state = ENEMY_STATE_IDLE;
      }
    }
    break;

    case ENEMY_STATE_DYING_LEFT:
    {
      SPR_setAnim(enemy->_sprite, 4);

      enemy->y += FIX16(0.4);
      enemy->x += FIX16(0.8);

      u16 x = fix16ToInt(enemy->x);

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

      if (enemy->_sprite->frameInd >= 2)
      {
        return_value.enemies_dead++;
        enemy->state = ENEMY_STATE_CLEAN;
      }
    }
    break;

    case ENEMY_STATE_DYING_RIGHT:
    {
      SPR_setAnim(enemy->_sprite, 4);

      enemy->y += FIX16(0.4);
      enemy->x += FIX16(-0.8);

      u16 x = fix16ToInt(enemy->x);

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

      if (enemy->_sprite->frameInd >= 2)
      {
        return_value.enemies_dead++;
        enemy->state = ENEMY_STATE_CLEAN;
      }
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
static void frame_change(Sprite *sprite)
{
  u16 tileIndex = _sprite_indexes[sprite->animInd][sprite->frameInd];

  SPR_setVRAMTileIndex(sprite, tileIndex);
}