#include "Game.h"

Game::Game()
{
	start = std::clock();
	//Console Settings
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions
	MoveWindow(console, r.left, r.top, 680, 800, TRUE);

	//Create empty game map (only borders)
	for (int i = 0; i < mapHeight; ++i) {
		for (int j = 0; j < mapWidth; ++j) {
			if (i == 0 || j == 0 || i == mapHeight - 1 || j == mapWidth - 1) {
				gameMap[i][j] = 'W';
			}
			else {
				gameMap[i][j] = ' ';
			}
		}
	}
	//Create walls
	int wallsCnt = 20;
	int wallPosX;
	int wallPosY;
	int wallLength;
	bool isHorizontal; //vertical = 0, horizontal = 1

	for (int currWallsCnt = 0; currWallsCnt < wallsCnt; ++currWallsCnt) {
		wallPosX = getRandomNumber(5, mapWidth - 10);
		wallPosY = getRandomNumber(5, mapHeight - 10);
		wallLength = getRandomNumber(5, 10);
		isHorizontal = getRandomNumber(0, 1);
		if (isHorizontal) {
			for (int i = 0; i < wallLength; ++i) {
				if (wallPosX + i < mapWidth)
					gameMap[wallPosY][wallPosX + i] = 'W';
			}
		}
		else {
			for (int i = 0; i < wallLength; ++i) {
				if (wallPosY + i < mapHeight - 2)
					gameMap[wallPosY + i][wallPosX] = 'W';
			}
		}
	}
	//Save initial map
	for (int i = 0; i < mapHeight; ++i) {
		for (int j = 0; j < mapWidth; ++j) {
			initialMap[i][j] = gameMap[i][j];
		}
	}
	//Create player
	player = Tank(mapWidth / 2, mapHeight - 2, 1, direction::STOP);
	lastMoveDir = direction::UP;
	healthPoints = 3;
	scorePoints = 0;
	gameMap[player.getPosY()][player.getPosX()] = 'X';

	//Create enemies
	enemiesCnt = 5;
	for (int i = 0; i < enemiesCnt; ++i) {
		int enemyPosX;
		int enemyPosY;
		int step = mapWidth / enemiesCnt;
		int minDiffBtwEnemies = 3;
		bool posFound = false;
		while (!posFound) {
			enemyPosX = getRandomNumber(step*i, step*(i + 1) - minDiffBtwEnemies);
			enemyPosY = getRandomNumber(0, mapHeight - 10);
			//check collision
			if (gameMap[enemyPosY][enemyPosX] == ' ') {
				posFound = true;
			}
		}
		enemies.push_back(Tank(enemyPosX, enemyPosY, 1, direction::STOP));
		gameMap[enemies[i].getPosY()][enemies[i].getPosX()] = '0';
	}
	//create vectors for enemies' bullets
	enemiesBullets.resize(enemiesCnt);
}

Game::~Game()
{
}

void Game::run()
{
	system("cls");
	cleanMap();
	handleInput();
	update();
	draw();
	//Sleep(50);
}

bool Game::isOver()
{
	return healthPoints == 0;
}

bool Game::isVictory()
{
	return enemies.size() == 0;
}

void Game::cleanMap()
{
	for (int i = 0; i < mapHeight; ++i) {
		for (int j = 0; j < mapWidth; ++j) {
			gameMap[i][j] = initialMap[i][j];
		}
	}
}

void Game::handleInput()
{
	if (GetAsyncKeyState(VK_UP))
	{
		player.setDir(direction::UP);
		lastMoveDir = direction::UP;
	}
	else if (GetAsyncKeyState(VK_RIGHT))
	{
		player.setDir(direction::RIGHT);
		lastMoveDir = direction::RIGHT;
	}
	else if (GetAsyncKeyState(VK_DOWN))
	{
		player.setDir(direction::DOWN);
		lastMoveDir = direction::DOWN;
	}
	else if (GetAsyncKeyState(VK_LEFT))
	{
		player.setDir(direction::LEFT);
		lastMoveDir = direction::LEFT;
	}
	else if (GetAsyncKeyState(VK_SPACE)) {

		switch (player.getDir()) {
		case direction::STOP:
			playerBullets.push_back(Bullet(player.getPosX(), player.getPosY(), 1, true, lastMoveDir));
			break;
		default:
			playerBullets.push_back(Bullet(player.getPosX(), player.getPosY(), 1, true, player.getDir()));
			break;
		}
	}
	else player.setDir(direction::STOP);
} 

void Game::update() {
	updatePlayer();
	updatePlayerBullets();
	updateEnemies();
	updateEnemiesBullets();
}

void Game::updatePlayer()
{
	int initialPlayerPosX = player.getPosX();
	int initialPlayerPosY = player.getPosY();

	player.update();

	//check collision with wall
	if (gameMap[player.getPosY()][player.getPosX()] == 'W') {
		player.setPosX(initialPlayerPosX);
		player.setPosY(initialPlayerPosY);
	}
	gameMap[player.getPosY()][player.getPosX()] = 'X';
}

void Game::updatePlayerBullets()
{
	for (size_t i = 0; i < playerBullets.size(); ++i) {
		playerBullets[i].update();
	}
	//check collision with wall, delete collided bullets
	playerBullets.erase(
		std::remove_if(playerBullets.begin(), playerBullets.end(), [&](const Bullet& b) {
		return (gameMap[b.getPosY()][b.getPosX()] == 'W'); }),
		playerBullets.end());
	//check collision with enemies
	for (size_t i = 0; i < playerBullets.size(); ++i) {
		for (size_t j = 0; j < enemies.size(); ++j) {
			if ((playerBullets[i].getPosX() == enemies[j].getPosX()) && (playerBullets[i].getPosY() == enemies[j].getPosY())) {
				playerBullets[i].setInFlight(false);
				enemies[j].setAlive(false);
				scorePoints++;
			}
		}
	}
	playerBullets.erase(
		std::remove_if(playerBullets.begin(), playerBullets.end(), [&](const Bullet& b) {return !b.getInFlight(); }),
		playerBullets.end());
	enemies.erase(
		std::remove_if(enemies.begin(), enemies.end(), [&](const Tank& t) {return !t.isAlive(); }),
		enemies.end());

	for (size_t i = 0; i < playerBullets.size(); ++i) {
		gameMap[playerBullets[i].getPosY()][playerBullets[i].getPosX()] = '*';
	}
}

void Game::updateEnemies()
{
	for (size_t i = 0; i < enemies.size(); ++i) {
		int updateChance = 40;
		if (getRandomNumber(1, 100) <= updateChance) {

			int initialEnemyPosX = enemies[i].getPosX();
			int initialEnemyPosY = enemies[i].getPosY();
			int step = mapWidth / enemiesCnt;
			int part;
			int minDiffBtwEnemies = 3;
			bool posFound = false;
			while (!posFound) {
				enemies[i].setDir(static_cast<direction>(getRandomNumber(0, 4)));
				enemies[i].update();
				//on which part of the map is enemy
				for (int j = 0; j < enemiesCnt; ++j) {
					if (enemies[i].getPosX() >= j * step && enemies[i].getPosX() < step*(j + 1)) {
						part = j;
						break;
					}
				}
				//check collision
				if ((gameMap[enemies[i].getPosY()][enemies[i].getPosX()] == ' ') &&
					(enemies[i].getPosX() >= int(step * part)) && (enemies[i].getPosX() < int(step * (part + 1)) - minDiffBtwEnemies)) {
					posFound = true;
				}
				else {
					enemies[i].setPosX(initialEnemyPosX);
					enemies[i].setPosY(initialEnemyPosY);
				}
			}
		}
		gameMap[enemies[i].getPosY()][enemies[i].getPosX()] = '0';	
	}
}

void Game::updateEnemiesBullets()
{
	//randomly create bullets
	for (size_t i = 0; i < enemies.size(); ++i) {
		int chanceToShoot = 10;
		if (getRandomNumber(1, 100) <= chanceToShoot) {
			enemiesBullets[i].push_back(Bullet(enemies[i].getPosX(), enemies[i].getPosY(), 1, true, static_cast<direction>(getRandomNumber(1, 4))));
		}
	}
	for (size_t i = 0; i < enemiesBullets.size(); ++i) {
		for (size_t j = 0; j < enemiesBullets[i].size(); ++j) {
			enemiesBullets[i][j].update();
		}
	}
	
	//check collision with wall, delete collided bullets
	for (size_t i = 0; i < enemiesBullets.size(); ++i) {
		enemiesBullets[i].erase(
			std::remove_if(enemiesBullets[i].begin(), enemiesBullets[i].end(), [&](const Bullet& b) {
			return (gameMap[b.getPosY()][b.getPosX()] == 'W'); }),
			enemiesBullets[i].end());
	}
	
	//check collision with player
	for (size_t i = 0; i < enemiesBullets.size(); ++i) {
		for (size_t j = 0; j < enemiesBullets[i].size(); ++j) {
			if ((enemiesBullets[i][j].getPosX() == player.getPosX()) && (enemiesBullets[i][j].getPosY() == player.getPosY())) {
				enemiesBullets[i][j].setInFlight(false);
				healthPoints--;
			}
		}
	}
	for (size_t i = 0; i < enemiesBullets.size(); ++i) {
		enemiesBullets[i].erase(
			std::remove_if(enemiesBullets[i].begin(), enemiesBullets[i].end(), [&](const Bullet& b) {return !b.getInFlight(); }),
			enemiesBullets[i].end());
	}
	
	for (size_t i = 0; i < enemiesBullets.size(); ++i) {
		for (size_t j = 0; j < enemiesBullets[i].size(); ++j) {
			gameMap[enemiesBullets[i][j].getPosY()][enemiesBullets[i][j].getPosX()] = '+';
		}
	}
	
}

void Game::draw()
{
	for (int i = 0; i < mapHeight; ++i) {
		for (int j = 0; j < mapWidth; ++j) {
			std::cout << gameMap[i][j] << std::flush;
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << " HEALTH POINTS - " << healthPoints << std::endl;
	std::cout << " PLAYER SCORE - " << scorePoints << std::endl;
	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	std::cout << " GAME DURATION - " << duration << "sec" << std::endl;
}

int Game::getRandomNumber(int lower, int upper)
{
	return lower + (std::rand() % (upper - lower + 1));
}

