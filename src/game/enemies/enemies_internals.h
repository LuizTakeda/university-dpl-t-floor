#ifndef ENEMIES_INTERNALS_H
#define ENEMIES_INTERNALS_H

#include <genesis.h>

#include "enemies.h"

typedef enum
{
  ENEMY_STATE_SPAWNING = 0,
  ENEMY_STATE_SPAWNING_LEFT,
  ENEMY_STATE_SPAWNING_RIGHT,
  ENEMY_STATE_RUNNING_RIGHT,
  ENEMY_STATE_RUNNING_LEFT,
  ENEMY_STATE_MOVING,
  ENEMY_STATE_SHOOTING_LEFT,
  ENEMY_STATE_SHOOTING_RIGHT,
  ENEMY_STATE_SHOOTING,
  ENEMY_STATE_IDLE_LEFT,
  ENEMY_STATE_IDLE_RIGHT,
  ENEMY_STATE_IDLE,
  ENEMY_STATE_DYING,
  ENEMY_STATE_DYING_LEFT,
  ENEMY_STATE_DYING_RIGHT,
  ENEMY_STATE_CLEAN,
} EnemyState;

typedef enum
{
  ENEMY_PLAYER_HIT_NO = 0,
  ENEMY_PLAYER_HIT_LEFT,
  ENEMY_PLAYER_HIT_RIGHT,
} EnemyPlayerHit;

typedef struct
{
  Sprite *_sprite;
  fix16 x, y;
  fix16 velocity_x, velocity_y;
  u16 hit_box_left_x, hit_box_right_x, hit_box_top_y, hit_box_bottom_y;
  bool dead;
  EnemyState last_state;
  EnemyState state;
  u32 data;
  u16 data_16;
} Enemy;

EnemyPlayerHit did_player_hit_enemy(const Enemy *enemy, const GamePlayerInfo *player_info);

bool did_enemy_hit_player(const Enemy *enemy, const GamePlayerInfo *player_info);

//**************************************************
//  SLIME
//**************************************************

void enemy_slime_setup();

void enemy_slime_clean();

bool enemy_slime_spawn(GameLevel game_level);

EnemiesEvents enemy_slime_logic(const GamePlayerInfo *player_info);

//**************************************************
//  BAT
//**************************************************

void enemy_bat_setup();

void enemy_bat_clean();

bool enemy_bat_spawn(GameLevel game_level);

EnemiesEvents enemy_bat_logic(const GamePlayerInfo *player_info);

//**************************************************
//  BALL PROJECTILE
//**************************************************

void enemy_ball_projectile_setup();

void enemy_ball_projectile_clean();

bool enemy_ball_projectile_spawn(u16 x, u16 y, fix16 x_velocity, fix16 y_velocity);

EnemiesEvents enemy_ball_projectile_logic(const GamePlayerInfo *player_info);

//**************************************************
//  VERTICAL SHOOTER
//**************************************************

void enemy_vertical_shooter_setup();

void enemy_vertical_shooter_clean();

bool enemy_vertical_shooter_spawn(GameLevel game_level);

EnemiesEvents enemy_vertical_shooter_logic(const GamePlayerInfo *player_info);

//**************************************************
//  HORIZONTAL SHOOTER
//**************************************************

void enemy_horizontal_shooter_setup();

void enemy_horizontal_shooter_clean();

bool enemy_horizontal_shooter_spawn(GameLevel game_level);

EnemiesEvents enemy_horizontal_shooter_logic(const GamePlayerInfo *player_info);

//**************************************************
//  JUMPER
//**************************************************

void enemy_jumper_setup();

void enemy_jumper_clean();

bool enemy_jumper_spawn(GameLevel game_level);

EnemiesEvents enemy_jumper_logic(const GamePlayerInfo *player_info);

#endif