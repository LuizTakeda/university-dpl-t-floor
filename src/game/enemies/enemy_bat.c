#include "enemies_internals.h"

#include <genesis.h>
#include <resources.h>

//**************************************************
//
//**************************************************

#define BAT_LIMIT 5
#define BAT_SPAWN_RATE 150

#define BAT_HIT_BOX_OFFSET_LEFT_X 1
#define BAT_HIT_BOX_OFFSET_RIGHT_X 1
#define BAT_HIT_BOX_OFFSET_TOP_X 1
#define BAT_HIT_BOX_OFFSET_BOTTOM_X 0

//**************************************************
//
//**************************************************

static Enemy _bat_list[BAT_LIMIT];
static u16 _bat_alive_quantity = 0;
static u16 **_bat_sprite_indexes;
static u16 _spawn_countdown = 0;

//**************************************************
//
//**************************************************

static bool bat_create();
static void bat_frame_change(Sprite *sprite);

//**************************************************
//
//**************************************************

/**
 *
 */
void enemy_bat_setup()
{
  u16 num_tiles;
  _bat_sprite_indexes = SPR_loadAllFrames(&spr_enemy_02, _tile_index, &num_tiles);
  _tile_index += num_tiles;

  _bat_alive_quantity = 0;
  _spawn_countdown = 0;

  for (u8 i = 0; i < BAT_LIMIT; i++)
  {
    _bat_list[i].dead = true;
  }
}

/**
 *
 */
void enemy_bat_clean()
{
  _bat_alive_quantity = 0;

  for (u8 i = 0; i < BAT_LIMIT; i++)
  {
    if (!_bat_list[i].dead)
    {
      SPR_releaseSprite(_bat_list[i]._sprite);
    }
    _bat_list[i].dead = true;
  }

  SPR_defragVRAM();
}

/**
 *
 */
bool enemy_bat_spawn(GameLevel game_level)
{
  if (_bat_alive_quantity >= BAT_LIMIT)
  {
    _spawn_countdown = BAT_SPAWN_RATE;
    return false;
  }

  if (_spawn_countdown > 0)
  {
    _spawn_countdown--;
    return false;
  }

  _spawn_countdown = BAT_SPAWN_RATE;

  return bat_create();
}

/**
 *
 */
EnemiesEvents enemy_bat_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents return_value;
  return_value.enemies_dead = 0;
  return_value.player_hit = false;

  for (u8 i = 0; i < BAT_LIMIT; i++)
  {
    if (_bat_list[i].dead)
    {
      continue;
    }

    switch (_bat_list[i].state)
    {
    case ENEMY_STATE_SPAWNING:
      SPR_setAnim(_bat_list[i]._sprite, 0);

      if (_bat_list[i]._sprite->frameInd == 4)
      {
        _bat_list[i].state = ENEMY_STATE_MOVING;
        break;
      }

      break;

    case ENEMY_STATE_MOVING:
      SPR_setAnim(_bat_list[i]._sprite, 1);

      _bat_list[i]._sprite->data = did_player_hit_enemy(&_bat_list[i], player_info);

      if (_bat_list[i]._sprite->data)
      {
        _bat_list[i].state = ENEMY_STATE_DYING;
        break;
      }

      return_value.player_hit |= did_enemy_hit_player(&_bat_list[i], player_info);

      _bat_list[i].x += _bat_list[i].velocity_x;
      _bat_list[i].y += _bat_list[i].velocity_y;

      if (_bat_list[i].y > FIX16(168))
      {
        _bat_list[i].velocity_y *= -1;
        _bat_list[i].y = FIX16(167);
      }

      if (_bat_list[i].y < FIX16(56))
      {
        _bat_list[i].velocity_y *= -1;
        _bat_list[i].y = FIX16(57);
      }

      if (_bat_list[i].x > FIX16(224))
      {
        _bat_list[i].velocity_x *= -1;
        _bat_list[i].x = FIX16(224);
      }

      if (_bat_list[i].x < FIX16(80))
      {
        _bat_list[i].velocity_x *= -1;
        _bat_list[i].x = FIX16(80);
      }

      break;

    case ENEMY_STATE_DYING:
      SPR_setAnim(_bat_list[i]._sprite, _bat_list[i]._sprite->data == ENEMY_PLAYER_HIT_LEFT ? 2 : 3);

      if (_bat_list[i]._sprite->frameInd > 0)
      {
        _bat_list[i].x += _bat_list[i].data == ENEMY_PLAYER_HIT_RIGHT  ? FIX16(-2)
                          : _bat_list[i].data == ENEMY_PLAYER_HIT_LEFT ? FIX16(2)
                                                                       : 0;

        _bat_list[i].y -= FIX16(.75);

        if (_bat_list[i].x > FIX16(224))
        {
          _bat_list[i].x = FIX16(224);
        }

        if (_bat_list[i].x < FIX16(80))
        {
          _bat_list[i].x = FIX16(80);
        }
      }

      if (_bat_list[i]._sprite->frameInd >= 2)
      {
        SPR_releaseSprite(_bat_list[i]._sprite);
        _bat_alive_quantity--;
        _bat_list[i].dead = true;
        return_value.enemies_dead++;
        break;
      }
      break;

    default:
      break;
    }

    _bat_list[i].hit_box_left_x = fix16ToInt(_bat_list[i].x) + BAT_HIT_BOX_OFFSET_LEFT_X;
    _bat_list[i].hit_box_right_x = fix16ToInt(_bat_list[i].x) + _bat_list[i]._sprite->definition->w - 1 - BAT_HIT_BOX_OFFSET_RIGHT_X;

    _bat_list[i].hit_box_top_y = fix16ToInt(_bat_list[i].y) + BAT_HIT_BOX_OFFSET_TOP_X;
    _bat_list[i].hit_box_bottom_y = fix16ToInt(_bat_list[i].y) + _bat_list[i]._sprite->definition->h - 1 - BAT_HIT_BOX_OFFSET_BOTTOM_X;

    SPR_setPosition(_bat_list[i]._sprite, fix16ToInt(_bat_list[i].x), fix16ToInt(_bat_list[i].y));
  }

  return return_value;
}

//**************************************************
//
//**************************************************

static bool bat_create()
{
  for (u16 i; i < BAT_LIMIT; i++)
  {
    if (!_bat_list[i].dead)
    {
      continue;
    }

    _bat_list[i].x = FIX16(80 + (random() % (224 - 80)));
    _bat_list[i].y = FIX16(56 + (random() % (168 - 56)));
    _bat_list[i].velocity_x = FIX16(.7);
    _bat_list[i].velocity_y = FIX16(.2);
    _bat_list[i].dead = false;
    _bat_list[i].state = ENEMY_STATE_SPAWNING;
    _bat_list[i]._sprite = SPR_addSprite(
        &spr_enemy_02,
        fix16ToInt(_bat_list[i].x), fix16ToInt(_bat_list[i].y),
        TILE_ATTR(PAL3, 0, false, false));

    SPR_setAutoTileUpload(_bat_list[i]._sprite, FALSE);
    SPR_setFrameChangeCallback(_bat_list[i]._sprite, &bat_frame_change);
    SPR_setAlwaysOnTop(_bat_list[i]._sprite);

    _bat_alive_quantity++;
    return true;
  }

  return false;
}

static void bat_frame_change(Sprite *sprite)
{
  u16 tileIndex = _bat_sprite_indexes[sprite->animInd][sprite->frameInd];

  SPR_setVRAMTileIndex(sprite, tileIndex);
}