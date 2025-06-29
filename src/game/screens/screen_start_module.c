#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "game/globals.h"
#include "../game.h"
#include "../inputs/inputs.h"

typedef enum
{
  OPTION_START_GAME = 0,
  OPTION_DETAILS,
  OPTION_CREDITS,
} Options;

static Options _current_option = OPTION_START_GAME;

void game_screen_start(const GameInputs *inputs)
{
  if (game_screen_is_first_entry())
  {
    VDP_clearPlane(BG_A, 1);
    VDP_clearPlane(BG_B, 1);
    VDP_clearPlane(WINDOW, 1);

    VDP_drawImageEx(BG_A, &img_menu, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, 0, true, DMA);

    _current_option = OPTION_START_GAME;
  }

  if (!XGM2_isPlaying())
  {
    XGM2_play(sfx_menu_music);
    XGM2_setFMVolume(40);
  }

  switch (_current_option)
  {
  case OPTION_START_GAME:
    VDP_drawTextBG(BG_B, " >Start< ", 15, 12);
    VDP_drawTextBG(BG_B, " Details ", 15, 14);
    VDP_drawTextBG(BG_B, " Credits ", 15, 16);

    if (game_inputs_click(inputs->down))
    {
      XGM2_playPCMEx(EFFECT_OPTION);
      _current_option++;
      return;
    }

    if (game_inputs_click(inputs->ok))
    {
      XGM2_playPCMEx(EFFECT_OPTION);
      game_screen_set(GSN_GAME);
      return;
    }
    break;

  case OPTION_DETAILS:
    VDP_drawTextBG(BG_B, "  Start  ", 15, 12);
    VDP_drawTextBG(BG_B, ">Details<", 15, 14);
    VDP_drawTextBG(BG_B, " Credits ", 15, 16);

    if (game_inputs_click(inputs->down))
    {
      XGM2_playPCMEx(EFFECT_OPTION);
      _current_option++;
      return;
    }

    if (game_inputs_click(inputs->up))
    {
      XGM2_playPCMEx(EFFECT_OPTION);
      _current_option--;
      return;
    }

    if (game_inputs_click(inputs->ok))
    {
      XGM2_playPCMEx(EFFECT_OPTION);
      return;
    }
    break;

  case OPTION_CREDITS:
    VDP_drawTextBG(BG_B, "  Start  ", 15, 12);
    VDP_drawTextBG(BG_B, " Details ", 15, 14);
    VDP_drawTextBG(BG_B, ">Credits<", 15, 16);

    if (game_inputs_click(inputs->up))
    {
      XGM2_playPCMEx(EFFECT_OPTION);
      _current_option--;
      return;
    }

    if (game_inputs_click(inputs->ok))
    {
      XGM2_playPCMEx(EFFECT_OPTION);
      game_screen_set(GSN_CREDITS);
      return;
    }
    break;
  }
}