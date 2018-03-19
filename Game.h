#pragma 

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <ctime>
#include "Bullet.h"
#include "Tank.h"

// Wall - W
// Player - X
// Player bullet - *
// Enemy - 0
// Enemy bullet - +

class Game
{
public:
	Game();
	virtual ~Game();

	void run();
	bool isOver();
	bool isVictory();
	
private:
	//Map
	const static int mapHeight = 40;
	const static int mapWidth = 80;
	char gameMap[mapHeight][mapWidth];
	char initialMap[mapHeight][mapWidth];
	void cleanMap();

	//Player
	Tank player;
	direction lastMoveDir;

	//Statistics
	int healthPoints;
	int scorePoints;
	std::clock_t start;
	double duration;


	//Player bullets
	std::vector<Bullet> playerBullets;

	//Enemies
	std::vector<Tank> enemies;
	int enemiesCnt;

	//Enenmies' bullets
	std::vector<std::vector<Bullet>> enemiesBullets;

	void handleInput();

	void update();
	void updatePlayer();
	void updatePlayerBullets();
	void updateEnemies();
	void updateEnemiesBullets();

	void draw();

	int getRandomNumber(int lower, int upper);
};

