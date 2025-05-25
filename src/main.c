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

#include "game/game.h"

int main(bool resetType)
{
	if (!resetType)
	{
		SYS_hardReset();
	}

	SYS_showFrameLoad(true);

	game_setup();

	while (true)
	{
		game_logic();
	}

	return 0;
}
