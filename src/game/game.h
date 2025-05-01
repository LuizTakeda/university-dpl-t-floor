#ifndef GAME_H
#define GAME_H

#define SCREEN_W 320
#define SCREEN_H 224

typedef enum
{
  GIS_NOTHING = 0,
  GIS_CLICK,
  GIS_HOLD,
  GIS_PRESS,
  GIS_RELEASED,
} GameInputStates;

typedef struct
{
  GameInputStates left;
  GameInputStates right;
  GameInputStates up;
  GameInputStates down;
  GameInputStates start;
  GameInputStates ok;
} GameInputs;

typedef enum
{
  GSN_START = 0,
  GSN_GAME,
  GSN_PAUSE,
  GSN_GAME_OVER,
  GSN_FINISH,
  GSN_CREDITS,
} GameScreenName;

// ### START Functions ###

/**
 * Realiza a inicialização do jogo
 */
void game_setup();

/**
 * Contem toda a logica do jogo
 */
void game_logic();

/**
 * Retorna true caso seja a primeira entrada na tela
 */
bool game_screen_is_first_entry();

/**
 * Define a tela atual
 */
void game_screen_set(const GameScreenName target_screen);

/**
 * Volta para uma tela sem reinicar o estado dela
 */
void game_screen_come_back(const GameScreenName target_screen);

/**
 * Tela inicial
 */
void game_screen_start(const GameInputs *inputs);

/**
 * Tela em jogo
 */
void game_screen_game(const GameInputs *inputs);

/**
 * Tela em pause
 */
void game_screen_pause(const GameInputs *inputs);

/**
 * Tela fim de jogo
 */
void game_screen_game_over(const GameInputs *inputs);

/**
 * Tela jogo concluído
 */
void game_screen_finish(const GameInputs *inputs);

/**
 * Tela de creditos
 */
void game_screen_credits(const GameInputs *inputs);

// ### END Functions ###

#endif