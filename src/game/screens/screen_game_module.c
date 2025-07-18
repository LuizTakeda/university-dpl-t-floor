#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "../game.h"
#include "../player/player.h"
#include "../enemies/enemies.h"
#include "../items/items.h"
#include "../inputs/inputs.h"

//**************************************************
//  Defines
//**************************************************

#define START_PLAYER_LIFE 3
#define START_TARGET_SCORE 15
#define PLUS_TARGET_SCORE 3

//**************************************************
//  Enums
//**************************************************
typedef enum
{
  GAME_STATE_SETUP = 0,
  GAME_STATE_IN_LEVEL,
  GAME_STATE_CHANING_LEVEL,
  GAME_STATE_DEAD,
  GAME_STATE_BOSS,
} GameState;

//**************************************************
//  Static functions
//**************************************************

static void state_setup(const GameInputs *inputs);
static void state_in_level(const GameInputs *inputs);
static void state_changing_level(const GameInputs *inputs);
static void state_dead(const GameInputs *inputs);
static void state_boss(const GameInputs *inputs);

static void set_player_life(u16 new_value);
static void set_score(u16 current, u16 target);
static void set_level(u16 new_value);

//**************************************************
//  Globals
//**************************************************

GameState _game_state = GAME_STATE_SETUP;
GameLevel _current_level = GAME_LEVEL_1;

uint16_t _score, _target_score;
uint16_t _player_life;

//**************************************************
//  Functions
//**************************************************

/**
 * Game logic
 */
void game_screen_game(const GameInputs *inputs)
{
  if (game_screen_is_first_entry())
  {
    _game_state = GAME_STATE_SETUP;

    XGM2_play(sfx_in_game_music);
    XGM2_setFMVolume(50);
  }

  if (!XGM2_isPlaying())
  {
    XGM2_stop();
    XGM2_play(sfx_in_game_music);
    XGM2_setFMVolume(50);
  }

  switch (_game_state)
  {
  case GAME_STATE_SETUP:
    state_setup(inputs);
    break;

  case GAME_STATE_IN_LEVEL:
    state_in_level(inputs);
    break;

  case GAME_STATE_CHANING_LEVEL:
    state_changing_level(inputs);
    break;

  case GAME_STATE_DEAD:
    state_dead(inputs);
    break;

  case GAME_STATE_BOSS:
    state_boss(inputs);
    break;

  default:
    break;
  }
}

//**************************************************
//  Static functions
//**************************************************

/**
 *
 */
static void state_setup(const GameInputs *inputs)
{
  // Resets everthing
  VDP_clearPlane(BG_A, 1);
  VDP_clearPlane(BG_B, 1);
  VDP_clearPlane(WINDOW, 1);
  SPR_reset();
  _tile_index = 0;

  // Draws level background
  VDP_drawImageEx(BG_A, &img_bg, TILE_ATTR_FULL(PAL0, 0, 0, 0, _tile_index), 0, 0, true, DMA);
  _tile_index += img_bg.tileset->numTile;

  // Initial values
  set_player_life(START_PLAYER_LIFE);
  set_score(0, START_TARGET_SCORE);
  set_level(GAME_LEVEL_1);

  // Setup
  player_setup();
  enemies_setup(_current_level);
  items_setup();

  // Change game state to
  _game_state = GAME_STATE_IN_LEVEL;
}

/**
 *
 */
static void state_in_level(const GameInputs *inputs)
{
  // XGM2_resume();

  GamePlayerInfo player_info = player_logic(inputs);

  EnemiesEvents enemies_events = enemies_logic(&player_info);

  ItemsEvent items_event = items_logic(&player_info);

  // Checks if any enemy has been killed
  if (enemies_events.enemies_dead > 0)
  {
    set_score(_score + enemies_events.enemies_dead, _target_score);
  }

  // Passes the level
  if (_score >= _target_score)
  {
    _game_state = GAME_STATE_CHANING_LEVEL;
    return;
  }

  if (items_event == ITEM_EVENT_LIFE)
  {
    set_player_life(_player_life + 1);
  }

  // Checks if the player was hit
  if (enemies_events.player_hit)
  {
    set_player_life(_player_life - 1);
    player_hit();

    if (_player_life == 0)
    {
      _game_state = GAME_STATE_DEAD;
    }
  }
}

/**
 *
 */

static void state_changing_level(const GameInputs *inputs)
{
  // XGM2_pause();

  if (_current_level + 1 > 9)
  {
    player_clean();
    enemies_clean();
    items_clean();
    game_screen_set(GSN_FINISH);
    return;
  }

  set_level(_current_level + 1);
  set_score(0, _target_score + PLUS_TARGET_SCORE);
  set_player_life(_player_life + 1);

  enemies_clean();
  items_clean();
  enemies_next_level(_current_level);

  _game_state = GAME_STATE_IN_LEVEL;
}

/**
 *
 */
static void state_dead(const GameInputs *inputs)
{
  // XGM2_pause();
  player_clean();
  enemies_clean();
  items_clean();
  game_screen_set(GSN_GAME_OVER);
}

/**
 *
 */
static void state_boss(const GameInputs *inputs)
{
}

/**
 *
 */
static void set_player_life(u16 new_value)
{
  _player_life = new_value;

  char str[10];
  sprintf(str, "LIFE %d", _player_life);
  VDP_drawText(str, 31, 6);
}

/**
 *
 */
static void set_score(u16 current, u16 target)
{
  _score = current;
  _target_score = target;

  char str[10];
  VDP_drawText("KILLS ", 1, 6);
  sprintf(str, "%3d/%3d", _score, _target_score);
  VDP_drawText(str, 1, 7);
}

/**
 *
 */
static void set_level(u16 new_value)
{
  _current_level = new_value;

  char str[10];
  sprintf(str, "LEVEL %d", _current_level);
  VDP_drawText(str, 31, 21);
}
