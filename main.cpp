#include <iostream> 
#include "Game.h"

int main() {
	srand(time(0));
	Game game;
	while (!(game.isOver() || game.isVictory())) {
		game.run();
	}
	system("cls");
	std::cout << "YOU " << (game.isOver() ? "LOSE" : "WIN");
	std::cout << std::endl;
	return 0;
}