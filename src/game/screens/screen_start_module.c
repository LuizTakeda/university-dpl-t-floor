#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "../game.h"
#include "../inputs/inputs.h"

typedef enum
{
  SO_START_GAME = 0,
  SO_CREDITS,
} ScreenOptions;

static ScreenOptions _current_option = SO_START_GAME;

void game_screen_start(const GameInputs *inputs)
{
  if (game_screen_is_first_entry())
  {
    VDP_clearPlane(BG_A, 1);
    VDP_clearPlane(BG_B, 1);
    VDP_clearPlane(WINDOW, 1);

    VDP_drawImageEx(BG_A, &img_menu, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, 0, true, DMA);

    _current_option = SO_START_GAME;
  }

  switch (_current_option)
  {
  case SO_START_GAME:
    VDP_drawTextBG(BG_B, " >Start< ", 15, 14);
    VDP_drawTextBG(BG_B, " Credits ", 15, 16);

    if (game_inputs_click(inputs->down))
    {
      _current_option = SO_CREDITS;
      return;
    }

    if (game_inputs_click(inputs->ok))
    {
      game_screen_set(GSN_GAME);
      return;
    }
    break;

  case SO_CREDITS:
  VDP_drawTextBG(BG_B, "  Start  ", 15, 14);
  VDP_drawTextBG(BG_B, ">Credits<", 15, 16);

    if (game_inputs_click(inputs->up))
    {
      _current_option = SO_START_GAME;
      return;
    }

    if (game_inputs_click(inputs->ok))
    {
      game_screen_set(GSN_CREDITS);
      return;
    }
    break;
  }
}