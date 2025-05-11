#include <genesis.h>
#include <resources.h>
#include <sprite_eng.h>

#include "enemies.h"

#define ENEMIES_MAX 80

// _sprites[_enemies_counter]->frameInd <- Indica qual frame atual da animação

typedef enum
{
  ENEMY_TYPE_SLIME = 0
} EnemyType;

typedef struct
{
  Sprite *_sprite;
  EnemyType type;
  fix16 x, y;
  fix16 velocity_x, velocity_y;
  bool dead;
} Enemy;

static Enemy _enemies[ENEMIES_MAX] = {{0}};
static fix16 _enemies_counter = 0;

static void create_enemy(EnemyType type);
static void enemy_logic_slime(Enemy *enemy);

void enemies_setup()
{
  PAL_setPalette(PAL3, spr_enemy_01.palette->data, DMA);
  for (int i = 0; i < ENEMIES_MAX; i++)
    create_enemy(ENEMY_TYPE_SLIME);
}

void enemies_logic(const GamePlayerInfo *player_info)
{
  for (int i = 0; i < ENEMIES_MAX; i++)
  {
    if (_enemies[i].dead)
      continue;

    switch (_enemies[i].type)
    {
    case ENEMY_TYPE_SLIME:
      enemy_logic_slime(&_enemies[i]);
      break;

    default:
      break;
    }
  }
}

static void create_enemy(EnemyType type)
{
  switch (type)
  {
  case ENEMY_TYPE_SLIME:
    _enemies[_enemies_counter].type = ENEMY_TYPE_SLIME;
    _enemies[_enemies_counter].x = FIX16(80 + (8 * (random() % 20)));
    _enemies[_enemies_counter].y = FIX16(168 - (32 * (random() % 4)));
    _enemies[_enemies_counter].velocity_y = 0;
    _enemies[_enemies_counter].velocity_x = FIX16((random() % 2) ? 0.5 : -0.5);
    _enemies[_enemies_counter].dead = false;
    _enemies[_enemies_counter]._sprite =
        SPR_addSprite(
            &spr_enemy_01,
            fix16ToInt(_enemies[_enemies_counter].x), fix16ToInt(_enemies[_enemies_counter].y),
            TILE_ATTR_FULL(PAL3, false, false, false, 1));

    _enemies_counter++;
    break;

  default:
    break;
  }
}

static void enemy_logic_slime(Enemy *slime)
{
  if (slime->x < FIX16(80))
  {
    slime->velocity_x = FIX16(0.5);
    slime->x = FIX16(80);
  }

  if (slime->x > FIX16(224))
  {
    slime->velocity_x = FIX16(-0.5);
    slime->x = FIX16(224);
  }

  slime->x += slime->velocity_x;
  slime->y += slime->velocity_y;

  SPR_setPosition(slime->_sprite, fix16ToInt(slime->x), fix16ToInt(slime->y));
}