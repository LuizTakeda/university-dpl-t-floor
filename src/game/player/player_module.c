#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "player.h"
#include "../inputs/inputs.h"

typedef enum
{
  GPA_RUNNING_LEFT = 4,
  GPA_STOPED_LEFT = 3,
  GPA_TURING_LEFT = 2,
  GPA_RUNNING_RIGHT = 1,
  GPA_STOPED_RIGHT = 0,
  GPA_TURING_RIGHT = 5,
  GPA_CLIMBING = 6,
} GamePlayerAnimations;

// ### START Variaveis globais ###

static Sprite *_sprite;
static fix16 _x = FIX16(128);
static fix16 _y = FIX16(176 - 16);
static u16 _left_x, _right_x, _bottom_y, _top_y;
static GamePlayerStates _current_state;
static bool _turn_complete = false;
static bool _first_entry = true;
static u8 _frame_counter = 0;
// ### END Variaveis globais ###

// ### START Funções internas ###

static void set_state(GamePlayerStates state);
static void convert_positions_fix_to_ints();
static bool can_climb();

static void running_right_logic(const GameInputs *inputs);
static void stoped_right_logic(const GameInputs *inputs);
static void turing_right_logic(const GameInputs *inputs);

static void running_left_logic(const GameInputs *inputs);
static void stoped_left_logic(const GameInputs *inputs);
static void turing_left_logic(const GameInputs *inputs);

static void climbing_up_logic(const GameInputs *inputs);
static void climbing_down_logic(const GameInputs *inputs);
// ### END Funções internas ###

/**
 * Carrega as informações iniciais do player
 */
void player_setup()
{
  PAL_setPalette(PAL1, spr_player.palette->data, DMA);
  _sprite = SPR_addSprite(&spr_player, 128, 176 - 16, TILE_ATTR_FULL(PAL1, false, false, false, 1));
  set_state(GPS_RUNNING_RIGHT);
}

/**
 * Logica principal do player
 */
GamePlayerInfo player_logic(const GameInputs *inputs)
{
  GamePlayerInfo player_info;

  do
  {
    player_info.state = _current_state;

    switch (player_info.state)
    {
    case GPS_RUNNING_RIGHT:
      running_right_logic(inputs);
      break;

    case GPS_STOPED_RIGHT:
      stoped_right_logic(inputs);
      break;

    case GPS_TURING_RIGHT:
      turing_right_logic(inputs);
      break;

    case GPS_RUNNING_LEFT:
      running_left_logic(inputs);
      break;

    case GPS_TURING_LEFT:
      turing_left_logic(inputs);
      break;

    case GPS_STOPED_LEFT:
      stoped_left_logic(inputs);
      break;

    case GPS_CLIMBING_UP:
      climbing_up(inputs);
      break;

    case GPS_CLIMBING_DOWN:
      climbing_down(inputs);
      break;

    default:
      break;
    }
  } while (player_info.state != _current_state);

  convert_positions_fix_to_ints();

  player_info.left_x = _left_x;
  player_info.right_x = _right_x;

  player_info.bottom_y = _bottom_y;
  player_info.top_y = _top_y;

  return player_info;
}

static void on_turn_tick(Sprite *sprite)
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

/**
 * Atualiza o estado atual do player
 * Ativa flag de primeira entrada
 */
static void set_state(GamePlayerStates state)
{
  _first_entry = true;
  _current_state = state;
}

/**
 * Retorna true caso a flag esteja ativa e desativa ela
 * Retorna falso quando a flag não está mais ativa
 */
static bool is_first_entry()
{
  if (!_first_entry)
  {
    return false;
  }
  _first_entry = false;
  return true;
}

/**
 * Calcula as posições do player
 */
static void convert_positions_fix_to_ints()
{
  _left_x = fix16ToInt(_x);
  _right_x = fix16ToInt(_x) + 8;

  _bottom_y = fix16ToInt(_y);
  _top_y = fix16ToInt(_y) + 16;
}

/**
 * Verifica se o player pode subir alguma escada
 */
static bool can_climb()
{
  convert_positions_fix_to_ints();

  if (_right_x >= 104 && _right_x <= 111)
  {
    return true;
  }

  if (_left_x >= 104 && _left_x <= 111)
  {
    return true;
  }

  if (_right_x >= 200 && _right_x <= 207)
  {
    return true;
  }

  if (_left_x >= 200 && _left_x <= 207)
  {
    return true;
  }

  return false;
}

/**
 * Estado do player correndo para esquerda
 */
static void running_right_logic(const GameInputs *inputs)
{
  if (is_first_entry())
  {
    SPR_setAnim(_sprite, GPA_RUNNING_RIGHT);
  }

  if (game_inputs_click(inputs->left))
  {
    set_state(GPS_TURING_LEFT);
    return;
  }

  if (game_inputs_click(inputs->up) && can_climb())
  {
    set_state(GPS_CLIMBING_UP);
    return;
  }

  if (fix16ToInt(_x) + 8 < 232)
  {
    _x += FIX16(0.9);
    SPR_setPosition(_sprite, fix16ToInt(_x), fix16ToInt(_y));
    return;
  }

  set_state(GPS_STOPED_RIGHT);
  return;
}

/**
 * Estado do player quando está parado olhando para esquerda
 */
static void stoped_right_logic(const GameInputs *inputs)
{
  if (is_first_entry())
  {
    SPR_setAnim(_sprite, GPA_STOPED_RIGHT);
  }

  if (game_inputs_click(inputs->left))
  {
    set_state(GPS_TURING_LEFT);
    return;
  }
}

/**
 * Estado do player que ele está virando para esquerda
 */
static void turing_right_logic(const GameInputs *inputs)
{
  if (is_first_entry())
  {
    _turn_complete = false;
    SPR_setAnim(_sprite, GPA_TURING_RIGHT);
    SPR_setFrameChangeCallback(_sprite, on_turn_tick);
    return;
  }

  if (_turn_complete)
  {
    set_state(GPS_RUNNING_RIGHT);
    return;
  }
}

/**
 * Estado do player correndo para esquerda
 */
static void running_left_logic(const GameInputs *inputs)
{
  if (is_first_entry())
  {
    SPR_setAnim(_sprite, GPA_RUNNING_LEFT);
  }

  if (game_inputs_click(inputs->right))
  {
    set_state(GPS_TURING_RIGHT);
    return;
  }

  if (fix16ToInt(_x) > 80)
  {
    _x -= FIX16(0.9);
    SPR_setPosition(_sprite, fix16ToInt(_x), fix16ToInt(_y));
    return;
  }

  set_state(GPS_STOPED_LEFT);
  return;
}

/**
 * Estado do player quando está parado olhando para esquerda
 */
static void stoped_left_logic(const GameInputs *inputs)
{
  if (is_first_entry())
  {
    SPR_setAnim(_sprite, GPA_STOPED_LEFT);
  }

  if (game_inputs_click(inputs->right))
  {
    set_state(GPS_TURING_RIGHT);
    return;
  }
}

/**
 * Estado do player que ele está virando para esquerda
 */
static void turing_left_logic(const GameInputs *inputs)
{
  if (is_first_entry())
  {
    _turn_complete = false;
    SPR_setAnim(_sprite, GPA_TURING_LEFT);
    SPR_setFrameChangeCallback(_sprite, on_turn_tick);
    return;
  }

  if (_turn_complete)
  {
    set_state(GPS_RUNNING_LEFT);
    return;
  }
}

/**
 *  Estado para quando o player está subindo
 */
static void climbing_up_logic(const GameInputs *inputs)
{
  if (is_first_entry())
  {
    SPR_setAnim(_sprite, GPA_CLIMBING);
  }

  if (fix16ToInt(_y) > 128)
  {
    _y -= FIX16(1);
    SPR_setPosition(_sprite, fix16ToInt(_x), fix16ToInt(_y));
    return;
  }

  set_state(GPS_RUNNING_LEFT);
  return;
}

/**
 *  Estado para quando o player está descendo
 */
static void climbing_down_logic(const GameInputs *inputs)
{
  if (is_first_entry())
  {
    SPR_setAnim(_sprite, GPA_CLIMBING);
  }
}