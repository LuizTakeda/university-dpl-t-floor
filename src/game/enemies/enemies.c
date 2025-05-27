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

  enemy_bat_setup();
}

void enemies_next_level(GameLevel level)
{
  _game_level = level;
}

/**
 *
 */
void enemies_clean()
{
  enemy_slime_clean();

  enemy_bat_clean();
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
   { enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;}
    break;

  case GAME_LEVEL_THREE:
    {enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;}
    break;

  case GAME_LEVEL_FOUR:
   { enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;}
    break;

  case GAME_LEVEL_FIVE:
    {enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;}
    break;

  case GAME_LEVEL_SIX:
    {enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;}
    break;

  case GAME_LEVEL_SEVEN:
    {enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;}
    break;

  case GAME_LEVEL_EIGHT:
  {
    enemy_slime_spawn(_game_level);

    enemy_bat_spawn(_game_level);

    EnemiesEvents slime_event = enemy_slime_logic(player_info);

    enemies_event.enemies_dead += slime_event.enemies_dead;
    enemies_event.player_hit |= slime_event.player_hit;

    EnemiesEvents bat_event = enemy_bat_logic(player_info);

    enemies_event.enemies_dead += bat_event.enemies_dead;
    enemies_event.player_hit |= bat_event.player_hit;
  }
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
    if (fix16ToInt(enemy->x) + enemy->_sprite->definition->w - 1 <= player_info->right_x)
    {
      return false;
    }
  }

  if (player_info->state == PLAYER_STATE_RUNNING_RIGHT)
  {
    if (fix16ToInt(enemy->x) >= player_info->left_x)
    {
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