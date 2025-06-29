#include <genesis.h>

#include "game.h"
#include "inputs/inputs.h"

// ### START Globals ###
u16 _tile_index = TILE_USER_INDEX;
// ### END Globals ###

static GameScreenName get_current_screen();

/**
 *
 */
void game_setup()
{
  VDP_setScreenWidth320();
  SPR_init();

  game_screen_set(GSN_START);

  SYS_doVBlankProcess();
}

/**
 *
 */
void game_logic()
{
  GameInputs inputs = game_inputs_read();

  switch (get_current_screen())
  {
  case GSN_START:
    game_screen_start(&inputs);
    break;

  case GSN_GAME:
    game_screen_game(&inputs);
    break;

  case GSN_PAUSE:
    game_screen_pause(&inputs);
    break;

  case GSN_GAME_OVER:
    game_screen_game_over(&inputs);
    break;

  case GSN_FINISH:
    game_screen_finish(&inputs);
    break;

  case GSN_CREDITS:
    game_screen_credits(&inputs);
    break;

  case GSN_DETAILS:
    game_screen_details(&inputs);
    break;

  default:
    break;
  }

  SPR_update();

  SYS_doVBlankProcess();
}

// Varivavel auxiliar para dizer qual tela atual
static GameScreenName _current_screen;

// Flag auxiliar para dizer se a primeira entrada naquela tela
static bool _first_entry;

/**
 * Retorna qual a tela atual
 */
static GameScreenName get_current_screen()
{
  return _current_screen;
}

/**
 * Atualiza qual a tela atual e ativa flag de primeira entrada
 */
void game_screen_set(const GameScreenName target_screen)
{
  _current_screen = target_screen;
  _first_entry = true;
}

/**
 * Atualiza qual a tela atual sme ativar a flag de primeira entrada
 */
void game_screen_come_back(const GameScreenName target_screen)
{
  _current_screen = target_screen;
}

/**
 * Verifica se a flag de primeira entrada está ativa
 * Reseta ela caso esteja e retorna true
 */
bool game_screen_is_first_entry()
{
  if (!_first_entry)
  {
    return false;
  }
  _first_entry = false;
  return true;
}