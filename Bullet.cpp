#include "Bullet.h"

Bullet::Bullet() : posX(0), posY(0), inFlight(false), speed(1),dir(direction::STOP)
{
}

Bullet::Bullet(int posX, int posY, int speed, bool inFlight, direction dir) : posX(posX), posY(posY), speed(speed), inFlight(inFlight), dir(dir)
{
}

Bullet::~Bullet()
{
}

const int & Bullet::getPosX() const
{
	return posX;
}

void Bullet::setPosX(int posX)
{
	this->posX = posX;
}

const int & Bullet::getPosY() const
{
	return posY;
}

void Bullet::setPoxY(int posY)
{
	this->posY = posY;
}

const int & Bullet::getSpeed() const
{
	return speed;
}

void Bullet::setSpeed(int speed)
{
	this->speed = speed;
}

const direction& Bullet::getDir() const
{
	return dir;
}

void Bullet::setDir(direction dir)
{
	this->dir = dir;
}

const bool& Bullet::getInFlight() const
{
	return inFlight;
}

void Bullet::setInFlight(bool inFlight)
{
	this->inFlight = inFlight;
}

void Bullet::update()
{
	if(!inFlight) {
		inFlight = true;
	}
	else
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
		}
	}
}
