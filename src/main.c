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
#include <sprite_eng.h>

#include "resources.h"

#include "gameobject.h"
#include "player.h"

void game_init()
{
	VDP_setScreenWidth320();
	SPR_init();

	// Desenhando background
	VDP_drawImageEx(BG_A, &img_bg, TILE_ATTR_FULL(PAL0, 0, 0, 0, 1), 0, 0, true, DMA);

	// Iniciando player
	u16 num_tiles = Player_init();
}

void game_update()
{
	Player_update();
}

int main(bool resetType)
{
	// Soft reset doesn't clear RAM. Can lead to bugs.
	if (!resetType)
	{
		SYS_hardReset();
	}

	game_init();
	
	SYS_doVBlankProcess();

	while (true)
	{
		game_update();

		SPR_update();

		SYS_doVBlankProcess();
	}

	return 0;
}
