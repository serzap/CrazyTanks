#pragma once
#include "Constants.h"
class Bullet
{
public:
	Bullet();
	Bullet(int posX, int posY, int speed, bool inFlight, direction dir);
	virtual ~Bullet();

	const int& getPosX() const;
	void setPosX(int posX);

	const int& getPosY() const;
	void setPoxY(int posY);

	const int& getSpeed() const;
	void setSpeed(int speed);

	const direction& getDir() const;
	void setDir(direction dir);

	const bool& getInFlight() const;
	void setInFlight(bool inFlight);

	void update();

private:
	int posX;
	int posY;
	int speed;
	bool inFlight;
	direction dir;
};

