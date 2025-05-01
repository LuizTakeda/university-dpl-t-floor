#include <genesis.h>
#include <string.h>

#include "inputs.h"

static GameInputStates read_button(u16 current_inputs, u16 button);

GameInputs game_inputs_read()
{
  GameInputs return_value;

  u16 current_inputs = JOY_readJoypad(JOY_1);

  return_value.left = read_button(current_inputs, BUTTON_LEFT);
  return_value.right = read_button(current_inputs, BUTTON_RIGHT);
  return_value.up = read_button(current_inputs, BUTTON_UP);
  return_value.down = read_button(current_inputs, BUTTON_DOWN);
  return_value.start = read_button(current_inputs, BUTTON_START);
  return_value.ok = read_button(current_inputs, BUTTON_A);

  return return_value;
}

static u16 __last_inputs_state = 0;

static GameInputStates read_button(u16 current_inputs, u16 button)
{
  if (current_inputs & button && !(__last_inputs_state & button))
  {
    __last_inputs_state |= button;
    return GIS_CLICK;
  }

  if (!(current_inputs & button) && __last_inputs_state & button)
  {
    __last_inputs_state &= ~button;
    return GIS_RELEASED;
  }

  if (current_inputs & button && __last_inputs_state & button)
  {
    return GIS_HOLD;
  }

  return GIS_NOTHING;
}