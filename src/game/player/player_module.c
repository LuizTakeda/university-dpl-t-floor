#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "player.h"
#include "../inputs/inputs.h"

typedef enum
{
  GPS_RUNNING_LEFT = 0,
  GPS_STOP_LEFT,
  GPS_TURING_LEFT,
  GPS_RUNNING_RIGHT,
  GPS_STOP_RIGHT,
  GPS_TURING_RIGHT,
  GPS_CLIMBING,
} GamePlayerStates;

static Sprite *sprite;

inline static GamePlayerStates player_get_state();
inline static bool player_done_turn();

static void player_set_state(GamePlayerStates state);

fix16 player_x, player_y;

void player_setup()
{
  PAL_setPalette(PAL1, spr_player.palette->data, DMA);
  sprite = SPR_addSprite(&spr_player, 128, 176 - 16, TILE_ATTR_FULL(PAL1, false, false, false, 1));
  player_x = FIX16(128);
  player_y = FIX16(176 - 16);
  player_set_state(GPS_RUNNING_RIGHT);
}

void player_logic(const GameInputs *inputs)
{
  GamePlayerStates player_state;
  do
  {
    player_state = player_get_state();

    switch (player_state)
    {
    case GPS_RUNNING_LEFT:
      if (game_inputs_click(inputs->right))
      {
        player_set_state(GPS_TURING_RIGHT);
        break;
      }

      if (fix16ToInt(player_x) > 80)
      {
        player_x -= FIX16(0.9);
      }
      else
      {
        player_set_state(GPS_STOP_LEFT);
        break;
      }

      SPR_setPosition(sprite, fix16ToInt(player_x), fix16ToInt(player_y));
      break;

    case GPS_TURING_RIGHT:
      if (player_done_turn())
      {
        player_set_state(GPS_RUNNING_RIGHT);
        break;
      }

      break;

    case GPS_RUNNING_RIGHT:
      if (game_inputs_click(inputs->left))
      {
        player_set_state(GPS_TURING_LEFT);
        break;
      }

      if (fix16ToInt(player_x) + 8 < 232)
      {
        player_x += FIX16(0.9);
      }
      else
      {
        player_set_state(GPS_STOP_RIGHT);
        break;
      }

      SPR_setPosition(sprite, fix16ToInt(player_x), fix16ToInt(player_y));
      break;

    case GPS_TURING_LEFT:
      if (player_done_turn())
      {
        player_set_state(GPS_RUNNING_LEFT);
        break;
      }

      break;

    case GPS_CLIMBING:
      if (game_inputs_click(inputs->right))
      {
        player_set_state(GPS_TURING_RIGHT);
        break;
      }
      break;

    case GPS_STOP_LEFT:
    if (game_inputs_click(inputs->right))
      {
        player_set_state(GPS_TURING_RIGHT);
        break;
      }
      break;

    case GPS_STOP_RIGHT:
      if (game_inputs_click(inputs->left))
      {
        player_set_state(GPS_TURING_LEFT);
        break;
      }
      break;

    default:
      break;
    }
  } while (player_state != player_get_state());
}

static GamePlayerStates _current_state;
static bool _turn_complete = false;

static u8 _frame_counter = 0;

static void onTurnTick(Sprite *sprite)
{
  if (_frame_counter <= 2)
  {
    _frame_counter++;
    return;
  }

  _frame_counter = 0;
  _turn_complete = true;
  SPR_setFrameChangeCallback(sprite, NULL);
}

inline static GamePlayerStates player_get_state()
{
  return _current_state;
}

static void player_set_state(GamePlayerStates state)
{
  _current_state = state;

  switch (_current_state)
  {
  case GPS_RUNNING_RIGHT:
    SPR_setAnim(sprite, 1);
    break;

  case GPS_STOP_RIGHT:
    SPR_setAnim(sprite, 0);
    break;

  case GPS_TURING_RIGHT:
    _turn_complete = false;
    SPR_setAnim(sprite, 5);
    SPR_setFrameChangeCallback(sprite, onTurnTick);
    break;

  case GPS_RUNNING_LEFT:
    SPR_setAnim(sprite, 4);
    break;

  case GPS_STOP_LEFT:
    SPR_setAnim(sprite, 3);
    break;

  case GPS_TURING_LEFT:
    _turn_complete = false;
    SPR_setAnim(sprite, 2);
    SPR_setFrameChangeCallback(sprite, onTurnTick);
    break;

  case GPS_CLIMBING:
    SPR_setAnim(sprite, 6);
    break;

  default:
    break;
  }
}

inline static bool player_done_turn()
{
  return _turn_complete;
}
