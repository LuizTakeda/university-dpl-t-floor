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
uint16_t score_target = 50;

uint16_t player_life = 3;

GameLevel _current_level = GAME_LEVEL_ONE;

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

    enemies_setup(_current_level);

    score = 0;
    player_life = 3;

    VDP_drawText("KILLS 50/0", 1, 2);
    VDP_drawText("LIFE 3", 1, 4);
    VDP_drawText("LEVEL 0", 1, 6);
  }

  GamePlayerInfo player_info = player_logic(inputs);

  EnemiesEvents enemies_events = enemies_logic(&player_info);

  if (enemies_events.enemies_dead > 0)
  {
    score += enemies_events.enemies_dead;
    char str[12];
    sprintf(str, "KILLS %d/%d", score_target, score);
    VDP_drawText(str, 1, 2);
  }

  if (score >= 50)
  {
    _current_level++;
    score = 0;
    score_target += 25;
    char str[10];
    sprintf(str, "LEVEL %d", _current_level + 1);
    VDP_drawText(str, 1, 4);
    enemies_clean();
    enemies_next_level(_current_level);
  }

  if (enemies_events.player_hit)
  {
    player_life--;
    char str[10];
    sprintf(str, "LIFE %d", player_life);
    VDP_drawText(str, 1, 4);
    player_hit();
  }

  if (player_life == 0)
  {
    player_clean();
    game_screen_set(GSN_GAME_OVER);
  }
}