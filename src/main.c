/**
 * Para compilar & rodar (com extensão Genesis Code):
 * ----------------------------------------------------
 *  OBS: instalar extensao Genesis Code e configurar "Gens Path"
 *
 *  Executar: $ cmd									<< pelo terminal do VSCode (extensão não funciona em PowerShell)
 *  F1 -> Genesis Code Compile Project				<< compilar
 *  F1 -> Genesis Code Compiler & Run Project		<< compilar & executar
 * -----------------------------------------------------
 * Solução temporária para o problema do VSCode não encontrar o path do Java:
 *   Editar o arquivo sgdk200/common.mk:
 *     JAVA := C:/Users/Aluno/Downloads/jdk-24/bin/java
 */
#include <genesis.h>
#include <resources.h>

#include "game/globals.h"
#include "game/game.h"

int main(bool resetType)
{
	if (!resetType)
	{
		SYS_hardReset();
	}

	SYS_showFrameLoad(true);

	XGM_setPCM(EFFECT_OPTION_ID, sfx_option_effect, EFFECT_OPTION_SIZE);
	XGM_setPCM(EFFECT_HIT_ID, sfx_hit_effect, EFFECT_HIT_SIZE);
	XGM_setPCM(EFFECT_STAIRS_ID, sfx_stairs_effect, EFFECT_STAIRS_SIZE);
	XGM_setPCM(EFFECT_SHOOT_ID, sfx_shoot_effect, EFFECT_SHOOT_SIZE);
	XGM_setPCM(EFFECT_TELEPORT_ID, sfx_teleport_effect, EFFECT_TELEPORT_SIZE);
	XGM_setPCM(EFFECT_JUMP_ID, sfx_jump_effect, EFFECT_JUMP_SIZE);

	game_setup();

	while (true)
	{
		game_logic();
	}

	return 0;
}
