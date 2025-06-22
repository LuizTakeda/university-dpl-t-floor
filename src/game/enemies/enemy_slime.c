#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "enemies_internals.h"

#define SLIME_LIMIT 16
#define SLIME_FLOORS 4
#define SLIME_LIMIT_FLOOR (SLIME_LIMIT / SLIME_FLOORS)

#define SLIME_HIT_BOX_OFFSET_LEFT_X 1
#define SLIME_HIT_BOX_OFFSET_RIGHT_X 1
#define SLIME_HIT_BOX_OFFSET_TOP_Y 1
#define SLIME_HIT_BOX_OFFSET_BOTTOM_Y 0

//*********************************************************************
//
//*********************************************************************

static Enemy _slime_list[SLIME_LIMIT];
static u8 _floor_quantity[SLIME_FLOORS] = {0};
static u16 **_slime_sprite_indexes;
static u16 _spawn_counter = 0;

//*********************************************************************
//
//*********************************************************************

static bool slime_create(u8 floor);
static void slime_frame_change(Sprite *sprite);

//*********************************************************************
//
//*********************************************************************

/**
 *
 */
void enemy_slime_setup()
{
  // Carregando todos os frames em memoria
  u16 num_tiles;
  _slime_sprite_indexes = SPR_loadAllFrames(&spr_enemy_01, _tile_index, &num_tiles);
  _tile_index += num_tiles;

  for (u8 i = 0; i < SLIME_FLOORS; i++)
  {
    _floor_quantity[i] = 0;
  }

  for (u8 i = 0; i < SLIME_LIMIT; i++)
  {
    _slime_list[i].dead = true;
  }
}

void enemy_slime_clean()
{
  for (u8 i = 0; i < SLIME_FLOORS; i++)
  {
    _floor_quantity[i] = 0;
  }

  for (u8 i = 0; i < SLIME_LIMIT; i++)
  {
    if (!_slime_list[i].dead)
    {
      SPR_releaseSprite(_slime_list[i]._sprite);
    }
    _slime_list[i].dead = true;
  }

  SPR_defragVRAM();
}

/**
 *
 */
bool enemy_slime_spawn(GameLevel game_level)
{
  if (_spawn_counter < 50)
  {
    _spawn_counter++;
    return false;
  }

  _spawn_counter = 0;

  u8 array[SLIME_FLOORS];

  // Preenche o array com valores de 0 até SLIME_FLOORS - 1
  for (u8 i = 0; i < SLIME_FLOORS; i++)
  {
    array[i] = i;
  }

  // Embaralha o array usando Fisher-Yates
  for (u8 i = SLIME_FLOORS - 1; i > 0; i--)
  {
    u8 j = random() % (i + 1);
    u8 temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }

  for (u8 i = 0; i < SLIME_FLOORS; i++)
  {
    if (slime_create(array[i]))
    {
      return true;
    }
  }

  return false;
}

/**
 *
 */
EnemiesEvents enemy_slime_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents return_value;
  return_value.player_hit = false;
  return_value.enemies_dead = 0;

  for (u8 i = 0; i < SLIME_LIMIT; i++)
  {
    if (_slime_list[i].dead)
    {
      continue;
    }

    switch (_slime_list[i].state)
    {
    case ENEMY_STATE_SPAWNING:
      SPR_setAnim(_slime_list[i]._sprite, 1);

      _slime_list[i].data = did_player_hit_enemy(&_slime_list[i], player_info);

      if (_slime_list[i].data)
      {
        _slime_list[i].last_state = _slime_list[i].state;
        _slime_list[i].state = ENEMY_STATE_DYING;
        break;
      }

      if (_slime_list[i]._sprite->frameInd == 4)
      {
        if (random() % 2)
        {
          _slime_list[i].state = ENEMY_STATE_RUNNING_RIGHT;
          break;
        }

        _slime_list[i].state = ENEMY_STATE_RUNNING_LEFT;
        break;
      }
      break;

    case ENEMY_STATE_RUNNING_RIGHT:
      SPR_setAnim(_slime_list[i]._sprite, 2);

      _slime_list[i].data = did_player_hit_enemy(&_slime_list[i], player_info);

      if (_slime_list[i].data)
      {
        _slime_list[i].last_state = _slime_list[i].state;
        _slime_list[i].state = ENEMY_STATE_DYING;
        break;
      }

      return_value.player_hit |= did_enemy_hit_player(&_slime_list[i], player_info);

      _slime_list[i].x += _slime_list[i].velocity_x;

      if (_slime_list[i].x > FIX16(224))
      {
        _slime_list[i].x = FIX16(224);
        _slime_list[i].state = ENEMY_STATE_RUNNING_LEFT;
        break;
      }
      break;

    case ENEMY_STATE_RUNNING_LEFT:
      SPR_setAnim(_slime_list[i]._sprite, 3);

      _slime_list[i].data = did_player_hit_enemy(&_slime_list[i], player_info);

      if (_slime_list[i].data)
      {
        _slime_list[i].last_state = _slime_list[i].state;
        _slime_list[i].state = ENEMY_STATE_DYING;
        break;
      }

      return_value.player_hit |= did_enemy_hit_player(&_slime_list[i], player_info);

      _slime_list[i].x -= _slime_list[i].velocity_x;

      if (_slime_list[i].x < FIX16(80))
      {
        _slime_list[i].x = FIX16(80);
        _slime_list[i].state = ENEMY_STATE_RUNNING_RIGHT;
        break;
      }
      break;

    case ENEMY_STATE_SHOOTING:
      break;

    case ENEMY_STATE_IDLE:
      break;

    case ENEMY_STATE_DYING:
      SPR_setAnim(_slime_list[i]._sprite, _slime_list[i].last_state == ENEMY_STATE_RUNNING_RIGHT ? 5 : 4);

      if (_slime_list[i]._sprite->frameInd > 0)
      {
        _slime_list[i].x += _slime_list[i].data == ENEMY_PLAYER_HIT_RIGHT  ? FIX16(-2)
                            : _slime_list[i].data == ENEMY_PLAYER_HIT_LEFT ? FIX16(2)
                                                                           : 0;

        _slime_list[i].y -= FIX16(.75);

        if (_slime_list[i].x > FIX16(224))
        {
          _slime_list[i].x = FIX16(224);
        }

        if (_slime_list[i].x < FIX16(80))
        {
          _slime_list[i].x = FIX16(80);
        }
      }

      if (_slime_list[i]._sprite->frameInd >= 3)
      {
        SPR_releaseSprite(_slime_list[i]._sprite);
        _floor_quantity[_slime_list[i]._sprite->data]--;
        _slime_list[i].dead = true;
        return_value.enemies_dead++;
        break;
      }

      break;

    default:
      break;
    }

    _slime_list[i].hit_box_left_x = fix16ToInt(_slime_list[i].x) + SLIME_HIT_BOX_OFFSET_LEFT_X;
    _slime_list[i].hit_box_right_x = fix16ToInt(_slime_list[i].x) + _slime_list[i]._sprite->definition->w - 1 - SLIME_HIT_BOX_OFFSET_RIGHT_X;

    _slime_list[i].hit_box_top_y = fix16ToInt(_slime_list[i].y) + SLIME_HIT_BOX_OFFSET_TOP_Y;
    _slime_list[i].hit_box_bottom_y = fix16ToInt(_slime_list[i].y) + _slime_list[i]._sprite->definition->h - 1 - SLIME_HIT_BOX_OFFSET_BOTTOM_Y;

    SPR_setPosition(_slime_list[i]._sprite, fix16ToInt(_slime_list[i].x), fix16ToInt(_slime_list[i].y));
  }

  return return_value;
}

//*********************************************************************
//
//*********************************************************************

/**
 *
 */
static bool slime_create(u8 floor)
{
  if (_floor_quantity[floor] >= SLIME_LIMIT_FLOOR)
  {
    return false;
  }

  for (u8 i = 0; i < SLIME_LIMIT; i++)
  {
    if (!_slime_list[i].dead)
    {
      continue;
    }

    _slime_list[i].x = FIX16(80 + (8 * (random() % 19)));
    _slime_list[i].y = FIX16(168 - (32 * floor));
    _slime_list[i].velocity_y = 0;
    _slime_list[i].velocity_x = FIX16(0.75);
    _slime_list[i].state = ENEMY_STATE_SPAWNING;
    _slime_list[i].dead = false;
    _slime_list[i].data = 0;
    _slime_list[i]._sprite =
        SPR_addSprite(
            &spr_enemy_01,
            fix16ToInt(_slime_list[i].x), fix16ToInt(_slime_list[i].y),
            TILE_ATTR_FULL(PAL3, 0, false, false, _tile_index));

    _slime_list[i]._sprite->data = floor;

    SPR_setAutoTileUpload(_slime_list[i]._sprite, FALSE);
    SPR_setFrameChangeCallback(_slime_list[i]._sprite, &slime_frame_change);
  SPR_setAlwaysOnTop(_slime_list[i]._sprite);

    _floor_quantity[floor]++;
    return true;
  }

  return false;
}

static void slime_frame_change(Sprite *sprite)
{
  u16 tileIndex = _slime_sprite_indexes[sprite->animInd][sprite->frameInd];

  SPR_setVRAMTileIndex(sprite, tileIndex);
}