#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "items.h"
#include "game/globals.h"

#define LIFE_RATE 800
#define LIFE_TIMEOUT 300
#define LIMIT_PER_LEVEL 5

static Sprite *_sprite;
static u16 _life_spawn_counter = 0;
static bool _life_alive = false;
static u16 _life_x, _life_y;
static u16 _life_limit_counter = 0;

/**
 *
 */
void items_setup()
{
  _life_spawn_counter = 0;
  _life_alive = false;
  _life_limit_counter = 0;
}

/**
 *
 */
void items_clean()
{
  if (_life_alive)
  {
    SPR_releaseSprite(_sprite);
  }

  _life_spawn_counter = 0;
  _life_alive = false;
  _life_limit_counter = 0;
}

/**
 *
 */
ItemsEvent items_logic(GamePlayerInfo *player_info)
{

  if (_life_limit_counter < LIMIT_PER_LEVEL && !_life_alive && _life_spawn_counter >= LIFE_RATE)
  {
    _life_spawn_counter = 0;
    _life_alive = true;

    _life_x = 80 + (8 * (random() % 19));
    _life_y = 168 - (32 * (random() % 4));

    _sprite = SPR_addSprite(
        &spr_item_01,
        _life_x, _life_y,
        TILE_ATTR(PAL3, 0, false, false));

    SPR_setAnim(_sprite, 0);
  }
  else if (_life_alive && _life_spawn_counter >= LIFE_TIMEOUT)
  {
    _life_spawn_counter = 0;
    _life_alive = false;

    SPR_releaseSprite(_sprite);
  }
  else
  {
    _life_spawn_counter++;
  }

  if (_life_alive)
  {
    if (_sprite->animInd == 0 && _sprite->frameInd >= 4)
    {
      SPR_setAnim(_sprite, 1);
    }

    if (_life_y >= player_info->top_y && _life_y <= player_info->bottom_y || _life_y + 7 >= player_info->top_y && _life_y + 7 <= player_info->bottom_y)
    {
      if (_life_x >= player_info->left_x && _life_x <= player_info->right_x || _life_x + 7 >= player_info->left_x && _life_x + 7 <= player_info->right_x)
      {
        _life_spawn_counter = 0;
        _life_alive = false;
        _life_limit_counter++;
        SPR_releaseSprite(_sprite);

        XGM2_playPCMEx(EFFECT_LIFE);

        return ITEM_EVENT_LIFE;
      }
    }
  }

  return ITEM_EVENT_NONE;
}