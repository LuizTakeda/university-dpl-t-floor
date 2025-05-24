#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "../game.h"
#include "../player/player.h"
#include "../enemies/enemies.h"

typedef enum
{
  GAME_STATE_SETUP = 0,
  GAME_STATE_IN_LEVEL,
  GAME_STATE_CHANING_LEVEL,
  GAME_STATE_BOSS,
} GameState;

void game_screen_game(const GameInputs *inputs)
{
  if (game_screen_is_first_entry())
  {
    VDP_clearPlane(BG_A, 1);
    VDP_clearPlane(BG_B, 1);
    VDP_clearPlane(WINDOW, 1);
    VDP_drawImageEx(BG_A, &img_bg, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, 0, true, DMA);

    player_setup();

    enemies_setup();
  }

  GamePlayerInfo player_info = player_logic(inputs);

  EnemiesEvents enemies_events = enemies_logic(&player_info);
}