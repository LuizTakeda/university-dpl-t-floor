#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "game/globals.h"
#include "enemies.h"
#include "enemies_internals.h"

//**************************************************
// Statis Function Prototype
//**************************************************

static EnemiesEvents level_1(const GamePlayerInfo *player_info);
static EnemiesEvents level_2(const GamePlayerInfo *player_info);
static EnemiesEvents level_3(const GamePlayerInfo *player_info);
static EnemiesEvents level_4(const GamePlayerInfo *player_info);
static EnemiesEvents level_5(const GamePlayerInfo *player_info);
static EnemiesEvents level_6(const GamePlayerInfo *player_info);
static EnemiesEvents level_7(const GamePlayerInfo *player_info);
static EnemiesEvents level_8(const GamePlayerInfo *player_info);
static EnemiesEvents level_9(const GamePlayerInfo *player_info);

//**************************************************
// Globals
//**************************************************

GameLevel _game_level;

//**************************************************
// Functions
//**************************************************

/**
 *
 */
void enemies_setup(GameLevel level)
{
  _game_level = level;

  PAL_setPalette(PAL3, spr_enemy_01.palette->data, DMA);

  enemy_slime_setup();

  enemy_bat_setup();

  enemy_ball_projectile_setup();

  enemy_vertical_shooter_setup();

  enemy_horizontal_shooter_setup();

  enemy_jumper_setup();

  enemy_teleporter_setup();
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

  enemy_ball_projectile_clean();

  enemy_vertical_shooter_clean();

  enemy_horizontal_shooter_clean();

  enemy_jumper_clean();

  enemy_teleporter_clean();
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

  case GAME_LEVEL_1:
    return level_1(player_info);

  case GAME_LEVEL_2:
    return level_2(player_info);

  case GAME_LEVEL_3:
    return level_3(player_info);

  case GAME_LEVEL_4:
    return level_4(player_info);

  case GAME_LEVEL_5:
    return level_5(player_info);

  case GAME_LEVEL_6:
    return level_6(player_info);

  case GAME_LEVEL_7:
    return level_7(player_info);

  case GAME_LEVEL_8:
    return level_8(player_info);

  case GAME_LEVEL_9:
    return level_9(player_info);

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
    if (player_info->left_x >= enemy->hit_box_left_x && player_info->left_x <= enemy->hit_box_right_x)
    {
      if ((enemy->hit_box_top_y >= player_info->top_y && enemy->hit_box_top_y <= player_info->bottom_y) ||
          (enemy->hit_box_bottom_y >= player_info->top_y && enemy->hit_box_bottom_y <= player_info->bottom_y))
      {
        XGM2_playPCMEx(EFFECT_HIT);
        return ENEMY_PLAYER_HIT_RIGHT;
      }
    }
  }

  if (player_info->state == PLAYER_STATE_RUNNING_RIGHT)
  {
    if (player_info->right_x >= enemy->hit_box_left_x && player_info->right_x <= enemy->hit_box_right_x)
    {
      if ((enemy->hit_box_top_y >= player_info->top_y && enemy->hit_box_top_y <= player_info->bottom_y) ||
          (enemy->hit_box_bottom_y >= player_info->top_y && enemy->hit_box_bottom_y <= player_info->bottom_y))
      {
        XGM2_playPCMEx(EFFECT_HIT);
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

  if (player_info->state == PLAYER_STATE_RUNNING_LEFT && (enemy->hit_box_left_x <= player_info->middle_x || enemy->hit_box_right_x <= player_info->middle_x || enemy->velocity_x > FIX16(0)))
  {
    return false;
  }

  if (player_info->state == PLAYER_STATE_RUNNING_RIGHT && (enemy->hit_box_left_x > player_info->middle_x || enemy->hit_box_right_x > player_info->middle_x || enemy->velocity_x < FIX16(0)))
  {
    return false;
  }

  if ((enemy->hit_box_top_y >= player_info->top_y && enemy->hit_box_top_y <= player_info->bottom_y) || (enemy->hit_box_bottom_y >= player_info->top_y && enemy->hit_box_bottom_y <= player_info->bottom_y))
  {
    if ((enemy->hit_box_left_x >= player_info->left_x && enemy->hit_box_left_x <= player_info->right_x) || (enemy->hit_box_right_x >= player_info->left_x && enemy->hit_box_right_x <= player_info->right_x))
    {
      XGM2_playPCMEx(EFFECT_HIT);
      return true;
    }
  }

  if ((player_info->top_y >= enemy->hit_box_top_y && player_info->top_y <= enemy->hit_box_bottom_y) || (player_info->bottom_y >= enemy->hit_box_top_y && player_info->bottom_y <= enemy->hit_box_bottom_y))
  {
    if ((player_info->left_x >= enemy->hit_box_left_x && player_info->left_x <= enemy->hit_box_right_x) || (player_info->right_x >= enemy->hit_box_left_x && player_info->right_x <= enemy->hit_box_right_x))
    {
      XGM2_playPCMEx(EFFECT_HIT);
      return true;
    }
  }

  return false;
}

//**************************************************
// Static functions
//**************************************************

static EnemiesEvents level_1(const GamePlayerInfo *player_info)
{
  EnemiesEvents enemies_event;
  enemies_event.enemies_dead = 0;
  enemies_event.player_hit = false;

  enemy_slime_spawn(_game_level);
  EnemiesEvents slime_event = enemy_slime_logic(player_info);
  enemies_event.enemies_dead += slime_event.enemies_dead;
  enemies_event.player_hit |= slime_event.player_hit;

  return enemies_event;
}

static EnemiesEvents level_2(const GamePlayerInfo *player_info)
{
  EnemiesEvents enemies_event;
  enemies_event.enemies_dead = 0;
  enemies_event.player_hit = false;

  enemy_slime_spawn(_game_level);
  EnemiesEvents slime_event = enemy_slime_logic(player_info);
  enemies_event.enemies_dead += slime_event.enemies_dead;
  enemies_event.player_hit |= slime_event.player_hit;

  enemy_teleporter_spawn(_game_level, player_info);
  EnemiesEvents teleporter_event = enemy_teleporter_logic(player_info);
  enemies_event.enemies_dead += teleporter_event.enemies_dead;
  enemies_event.player_hit |= teleporter_event.player_hit;

  return enemies_event;
}

static EnemiesEvents level_3(const GamePlayerInfo *player_info)
{
  EnemiesEvents enemies_event;
  enemies_event.enemies_dead = 0;
  enemies_event.player_hit = false;

  enemy_slime_spawn(_game_level);
  EnemiesEvents slime_event = enemy_slime_logic(player_info);
  enemies_event.enemies_dead += slime_event.enemies_dead;
  enemies_event.player_hit |= slime_event.player_hit;

  enemy_teleporter_spawn(_game_level, player_info);
  EnemiesEvents teleporter_event = enemy_teleporter_logic(player_info);
  enemies_event.enemies_dead += teleporter_event.enemies_dead;
  enemies_event.player_hit |= teleporter_event.player_hit;

  enemy_bat_spawn(_game_level);
  EnemiesEvents bat_event = enemy_bat_logic(player_info);
  enemies_event.enemies_dead += bat_event.enemies_dead;
  enemies_event.player_hit |= bat_event.player_hit;

  return enemies_event;
}

static EnemiesEvents level_4(const GamePlayerInfo *player_info)
{
  EnemiesEvents enemies_event;
  enemies_event.enemies_dead = 0;
  enemies_event.player_hit = false;

  enemy_slime_spawn(_game_level);
  EnemiesEvents slime_event = enemy_slime_logic(player_info);
  enemies_event.enemies_dead += slime_event.enemies_dead;
  enemies_event.player_hit |= slime_event.player_hit;

  enemy_bat_spawn(_game_level);
  EnemiesEvents bat_event = enemy_bat_logic(player_info);
  enemies_event.enemies_dead += bat_event.enemies_dead;
  enemies_event.player_hit |= bat_event.player_hit;

  enemy_jumper_spawn(_game_level);
  EnemiesEvents jumper_event = enemy_jumper_logic(player_info);
  enemies_event.enemies_dead += jumper_event.enemies_dead;
  enemies_event.player_hit |= jumper_event.player_hit;

  return enemies_event;
}

static EnemiesEvents level_5(const GamePlayerInfo *player_info)
{
  EnemiesEvents enemies_event;
  enemies_event.enemies_dead = 0;
  enemies_event.player_hit = false;

  enemy_slime_spawn(_game_level);
  EnemiesEvents slime_event = enemy_slime_logic(player_info);
  enemies_event.enemies_dead += slime_event.enemies_dead;
  enemies_event.player_hit |= slime_event.player_hit;

  enemy_teleporter_spawn(_game_level, player_info);
  EnemiesEvents teleporter_event = enemy_teleporter_logic(player_info);
  enemies_event.enemies_dead += teleporter_event.enemies_dead;
  enemies_event.player_hit |= teleporter_event.player_hit;

  enemy_bat_spawn(_game_level);
  EnemiesEvents bat_event = enemy_bat_logic(player_info);
  enemies_event.enemies_dead += bat_event.enemies_dead;
  enemies_event.player_hit |= bat_event.player_hit;

  enemy_jumper_spawn(_game_level);
  EnemiesEvents jumper_event = enemy_jumper_logic(player_info);
  enemies_event.enemies_dead += jumper_event.enemies_dead;
  enemies_event.player_hit |= jumper_event.player_hit;

  return enemies_event;
}

static EnemiesEvents level_6(const GamePlayerInfo *player_info)
{
  EnemiesEvents enemies_event;
  enemies_event.enemies_dead = 0;
  enemies_event.player_hit = false;

  enemy_slime_spawn(_game_level);
  EnemiesEvents slime_event = enemy_slime_logic(player_info);
  enemies_event.enemies_dead += slime_event.enemies_dead;
  enemies_event.player_hit |= slime_event.player_hit;

  enemy_jumper_spawn(_game_level);
  EnemiesEvents jumper_event = enemy_jumper_logic(player_info);
  enemies_event.enemies_dead += jumper_event.enemies_dead;
  enemies_event.player_hit |= jumper_event.player_hit;

  enemy_horizontal_shooter_spawn(_game_level);
  EnemiesEvents vertical_shooter_event = enemy_horizontal_shooter_logic(player_info);
  enemies_event.enemies_dead += vertical_shooter_event.enemies_dead;
  enemies_event.player_hit |= vertical_shooter_event.player_hit;

  EnemiesEvents ball_projectile_event = enemy_ball_projectile_logic(player_info);
  enemies_event.player_hit |= ball_projectile_event.player_hit;

  return enemies_event;
}

static EnemiesEvents level_7(const GamePlayerInfo *player_info)
{
  EnemiesEvents enemies_event;
  enemies_event.enemies_dead = 0;
  enemies_event.player_hit = false;

  enemy_slime_spawn(_game_level);
  EnemiesEvents slime_event = enemy_slime_logic(player_info);
  enemies_event.enemies_dead += slime_event.enemies_dead;
  enemies_event.player_hit |= slime_event.player_hit;

  enemy_bat_spawn(_game_level);
  EnemiesEvents bat_event = enemy_bat_logic(player_info);
  enemies_event.enemies_dead += bat_event.enemies_dead;
  enemies_event.player_hit |= bat_event.player_hit;

  enemy_horizontal_shooter_spawn(_game_level);
  EnemiesEvents vertical_shooter_event = enemy_horizontal_shooter_logic(player_info);
  enemies_event.enemies_dead += vertical_shooter_event.enemies_dead;
  enemies_event.player_hit |= vertical_shooter_event.player_hit;

  enemy_vertical_shooter_spawn(_game_level);
  EnemiesEvents horizontal_shooter_event = enemy_vertical_shooter_logic(player_info);
  enemies_event.enemies_dead += horizontal_shooter_event.enemies_dead;
  enemies_event.player_hit |= horizontal_shooter_event.player_hit;

  EnemiesEvents ball_projectile_event = enemy_ball_projectile_logic(player_info);
  enemies_event.player_hit |= ball_projectile_event.player_hit;

  return enemies_event;
}

static EnemiesEvents level_8(const GamePlayerInfo *player_info)
{
  EnemiesEvents enemies_event;
  enemies_event.enemies_dead = 0;
  enemies_event.player_hit = false;

  enemy_teleporter_spawn(_game_level, player_info);
  EnemiesEvents teleporter_event = enemy_teleporter_logic(player_info);
  enemies_event.enemies_dead += teleporter_event.enemies_dead;
  enemies_event.player_hit |= teleporter_event.player_hit;

  enemy_jumper_spawn(_game_level);
  EnemiesEvents jumper_event = enemy_jumper_logic(player_info);
  enemies_event.enemies_dead += jumper_event.enemies_dead;
  enemies_event.player_hit |= jumper_event.player_hit;

  enemy_vertical_shooter_spawn(_game_level);
  EnemiesEvents horizontal_shooter_event = enemy_vertical_shooter_logic(player_info);
  enemies_event.enemies_dead += horizontal_shooter_event.enemies_dead;
  enemies_event.player_hit |= horizontal_shooter_event.player_hit;

  EnemiesEvents ball_projectile_event = enemy_ball_projectile_logic(player_info);
  enemies_event.player_hit |= ball_projectile_event.player_hit;

  return enemies_event;
}

static EnemiesEvents level_9(const GamePlayerInfo *player_info)
{
  EnemiesEvents enemies_event;
  enemies_event.enemies_dead = 0;
  enemies_event.player_hit = false;

  enemy_slime_spawn(_game_level);
  EnemiesEvents slime_event = enemy_slime_logic(player_info);
  enemies_event.enemies_dead += slime_event.enemies_dead;
  enemies_event.player_hit |= slime_event.player_hit;

  enemy_teleporter_spawn(_game_level, player_info);
  EnemiesEvents teleporter_event = enemy_teleporter_logic(player_info);
  enemies_event.enemies_dead += teleporter_event.enemies_dead;
  enemies_event.player_hit |= teleporter_event.player_hit;

  enemy_bat_spawn(_game_level);
  EnemiesEvents bat_event = enemy_bat_logic(player_info);
  enemies_event.enemies_dead += bat_event.enemies_dead;
  enemies_event.player_hit |= bat_event.player_hit;

  enemy_jumper_spawn(_game_level);
  EnemiesEvents jumper_event = enemy_jumper_logic(player_info);
  enemies_event.enemies_dead += jumper_event.enemies_dead;
  enemies_event.player_hit |= jumper_event.player_hit;

  enemy_vertical_shooter_spawn(_game_level);
  EnemiesEvents horizontal_shooter_event = enemy_vertical_shooter_logic(player_info);
  enemies_event.enemies_dead += horizontal_shooter_event.enemies_dead;
  enemies_event.player_hit |= horizontal_shooter_event.player_hit;

  EnemiesEvents ball_projectile_event = enemy_ball_projectile_logic(player_info);
  enemies_event.player_hit |= ball_projectile_event.player_hit;

  return enemies_event;
}