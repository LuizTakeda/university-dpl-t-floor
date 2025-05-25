#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "enemies.h"
#include "enemies_internals.h"

GameLevel _game_level;

/**
 *
 */
void enemies_setup(GameLevel level)
{
  _game_level = level;

  PAL_setPalette(PAL3, spr_enemy_01.palette->data, DMA);

  enemy_slime_setup();
}

/**
 *
 */
void enemies_clean()
{
}

/**
 *
 */
EnemiesEvents enemies_logic(const GamePlayerInfo *player_info)
{
  EnemiesEvents enemies_event;
  enemies_event.enemies_dead = 0;
  enemies_event.player_hit = false;

  switch (_game_level)
  {
  case GAME_LEVEL_ONE:
  {
    enemy_slime_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;
  }
  break;
  case GAME_LEVEL_TWO:
    break;
  case GAME_LEVEL_THREE:
    break;
  case GAME_LEVEL_FOUR:
    break;
  case GAME_LEVEL_FIVE:
    break;
  case GAME_LEVEL_SIX:
    break;
  case GAME_LEVEL_SEVEN:
    break;
  case GAME_LEVEL_EIGHT:
    break;

  default:
    break;
  }

  return enemies_event;
}

/**
 *
 */
EnemyPlayerHit did_player_hit_enemy(const Enemy *enemy, const GamePlayerInfo *player_info)
{
  if (player_info->state == PLAYER_STATE_RUNNING_LEFT)
  {
    if (fix16ToInt(enemy->y) >= player_info->top_y && fix16ToInt(enemy->y) <= player_info->bottom_y)
    {
      if (player_info->left_x >= fix16ToInt(enemy->x) && player_info->left_x <= fix16ToInt(enemy->x) + 7)
      {
        return ENEMY_PLAYER_HIT_RIGHT;
      }
    }
  }

  if (player_info->state == PLAYER_STATE_RUNNING_RIGHT)
  {
    if (fix16ToInt(enemy->y) >= player_info->top_y && fix16ToInt(enemy->y) <= player_info->bottom_y)
    {
      if (player_info->right_x >= fix16ToInt(enemy->x) && player_info->right_x <= fix16ToInt(enemy->x) + 7)
      {
        return ENEMY_PLAYER_HIT_LEFT;
      }
    }
  }

  return ENEMY_PLAYER_HIT_NO;
}

/**
 *
 */
bool did_enemy_hit_player(const Enemy *enemy, const GamePlayerInfo *player_info)
{
  if (player_info->intangible)
  {
    return false;
  }

  if (player_info->state == PLAYER_STATE_RUNNING_LEFT)
  {
    if(fix16ToInt(enemy->x) + enemy->_sprite->definition->w - 1<= player_info->right_x){
      return false;
    }
  }

  if (player_info->state == PLAYER_STATE_RUNNING_RIGHT)
  {
    if(fix16ToInt(enemy->x) >= player_info->left_x){
      return false;
    }
  }

  if ((fix16ToInt(enemy->x) >= player_info->left_x && fix16ToInt(enemy->x) <= player_info->right_x) ||
      (fix16ToInt(enemy->x) + enemy->_sprite->definition->w - 1 >= player_info->left_x && fix16ToInt(enemy->x) + enemy->_sprite->definition->w - 1 <= player_info->right_x))
  {
    if ((fix16ToInt(enemy->y) >= player_info->top_y && fix16ToInt(enemy->y) <= player_info->bottom_y) ||
        (fix16ToInt(enemy->y) + enemy->_sprite->definition->h - 1 >= player_info->top_y && fix16ToInt(enemy->y) + enemy->_sprite->definition->h - 1 <= player_info->bottom_y))
    {
      return true;
    }
  }

  return false;
}