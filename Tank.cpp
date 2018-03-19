#include "Tank.h"



Tank::Tank():posX(0), posY(0), speed(1), dir(direction::STOP),alive(true)
{
}


Tank::Tank(int posX, int posY, int speed, direction dir) : 
posX(posX), posY(posY), speed(speed), dir(dir),alive(true)
{

}

Tank::~Tank()
{
}

const int & Tank::getPosX() const
{
	return posX;
}

void Tank::setPosX(int posX)
{
	this->posX = posX;
}

const int & Tank::getPosY() const
{
	return posY;
}

void Tank::setPosY(int posY)
{
	this->posY = posY;
}

const int & Tank::getSpeed() const
{
	return speed;
}

void Tank::setSpeed(int speed)
{
	this->speed = speed;
}

const direction & Tank::getDir() const
{
	return dir;
}

void Tank::setDir(direction dir)
{
	this->dir = dir;
}


const bool & Tank::isAlive() const
{
	return alive;
}

void Tank::setAlive(bool alive)
{
	this->alive = alive;
}

void Tank::update()
{
	switch (dir) {
	case direction::UP:
		posY -= speed;
		break;
	case direction::RIGHT:
		posX += speed;
		break;
	case direction::DOWN:
		posY += speed;
		break;
	case direction::LEFT:
		posX -= speed;
		break;
	case direction::STOP:
		break;
	}
}
