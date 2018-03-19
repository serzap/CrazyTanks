#pragma once

#include "Constants.h"
class Tank
{
public:
	Tank();
	Tank(int posX, int posY, int speed, direction dir);
	virtual ~Tank();

	const int& getPosX() const;
	void setPosX(int posX);

	const int& getPosY() const;
	void setPosY(int posY);

	const int& getSpeed() const;
	void setSpeed(int speed);

	const direction& getDir() const;
	void setDir(direction dir);

	const bool& isAlive() const;
	void setAlive(bool alive);

	void update();

private:
	int posX;
	int posY;
	int speed;
	direction dir;
	bool alive;
};

