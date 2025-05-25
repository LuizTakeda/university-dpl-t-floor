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

uint16_t score = 0;
uint16_t player_life = 3;

void game_screen_game(const GameInputs *inputs)
{
  if (game_screen_is_first_entry())
  {
    VDP_clearPlane(BG_A, 1);
    VDP_clearPlane(BG_B, 1);
    VDP_clearPlane(WINDOW, 1);

    SPR_reset();

    _tile_index = 0;

    VDP_drawImageEx(BG_A, &img_bg, TILE_ATTR_FULL(PAL0, 0, 0, 0, _tile_index), 0, 0, true, DMA);
    _tile_index += img_bg.tileset->numTile;

    player_setup();

    enemies_setup(GAME_LEVEL_ONE);

    score = 0;
    player_life = 3;

    VDP_drawText("50/0", 1, 2);
    VDP_drawText("3", 1, 4);
  }

  GamePlayerInfo player_info = player_logic(inputs);

  EnemiesEvents enemies_events = enemies_logic(&player_info);

  if (enemies_events.enemies_dead > 0)
  {
    score += enemies_events.enemies_dead;
    char str[6];
    sprintf(str, "50/%d", score);
    VDP_drawText(str, 1, 2);
  }

  if (enemies_events.player_hit)
  {
    player_life--;
    char str[6];
    sprintf(str, "%d", player_life);
    VDP_drawText(str, 1, 4);
    player_hit();
  }

  if (player_life == 0)
  {
    game_screen_set(GSN_GAME_OVER);
  }
}