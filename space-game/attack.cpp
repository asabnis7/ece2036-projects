// attack.cpp ---------------------------------------------

#include "mbed.h"
#include "uLCD_4DGL.h"
#include "attack.h"
#include "sprites.h"



// define class Spaceship ---------------------------------

Spaceship::Spaceship()
{
	shipxPos = 64;
	shipyPos = 110;
}

void Spaceship::draw(uLCD_4DGL& LCD)
{
	LCD.BLIT(shipxPos, shipyPos, 19, 8, ship_sprite);
}

void Spaceship::moveRight()
{
	if (shipxPos < 118)
	{
		shipxPos += 2;
	}
}

void Spaceship::moveLeft()
{
	if (shipxPos > 10)
	{
		shipxPos -= 2;
	}
}

int Spaceship::getxPos() { return shipxPos; }
int Spaceship::getyPos() { return shipyPos; }

// end class Spaceship ------------------------------------



// class Bullet -------------------------------------------

Bullet::Bullet(): oneBullet(false) { }

void Bullet::fireBullet(Spaceship &s1, uLCD_4DGL& LCD)
{
	bulletxPos = s1.getxPos()+ 8;
	bulletyPos = s1.getyPos() - 4;

	LCD.filled_circle(bulletxPos, bulletyPos, 3, WHITE);
}

void Bullet::moveBullet(uLCD_4DGL& LCD)
{	
	if (bulletyPos >= 4 && oneBullet == true)
	{
		bulletyPos -= 2;
		LCD.filled_circle(bulletxPos, bulletyPos + 2, 3, BLACK);
		LCD.filled_circle(bulletxPos, bulletyPos, 3, WHITE);
		if (bulletyPos < 4)
		{
			LCD.filled_circle(bulletxPos, 2, 6, BLACK);
			oneBullet = false;
		}
	}
}

void Bullet::setBullet(bool b) { oneBullet = b; }

bool Bullet::getBullet() { return oneBullet; }

int Bullet::getxBullet() { return bulletxPos; }

int Bullet::getyBullet() { return bulletyPos; }

// end class Bullet ---------------------------------------



// class ScreenAliens -------------------------------------

void ScreenAliens::changexPos(int s)
{
	if (xPos < 110 && direc == true)
	{
		xPos += s;
		if (xPos >= 110)
		{
			direc = false;
			xPos = 109;
		}
	}
	else if (xPos < 110 && direc == false)
	{
		xPos -= s;
		if (xPos <= 10)
		{
			direc = true;
			xPos = 11;
		}
	}
}

bool ScreenAliens::hitTest(Bullet& b1)
{
	int bxPos = b1.getxBullet();
	int byPos = b1.getyBullet();
	
	hit = ((bxPos >= (xPos-7)) && (bxPos <= (xPos + 7)) && (byPos >= (yPos-5)) && (byPos <= (yPos + 5)));
	return hit;
}

void ScreenAliens::blackOut(uLCD_4DGL& LCD)
{
	LCD.BLIT(xPos, yPos, 15, 8, blacksprite);
}

void ScreenAliens::setxPos(int x) { xPos = x; }

void ScreenAliens::setyPos(int y) { yPos = y; }

void ScreenAliens::setDirec(bool d) { direc = d; }

int ScreenAliens::getxPos() { return xPos; }

int ScreenAliens::getyPos() { return yPos; }

// end class ScreenAliens ---------------------------------



// class AlienBob -----------------------------------------

AlienBob::AlienBob(int n)
{
	setxPos(18 + rand() % 110);
	setyPos((10+10*(n-1)) + rand() % 2);
	setDirec(true);
}

void AlienBob::draw(uLCD_4DGL& LCD)
{
	LCD.BLIT(getxPos(), getyPos(), 15, 8, alienBobUp_sprite);
}

void AlienBob::update(uLCD_4DGL& LCD)
{
	changexPos(1);
	int res = getxPos() % 2;
	switch(res)
	{
		case(0):
		{
			LCD.BLIT(getxPos(), getyPos(), 15, 8, alienBobDown_sprite);
			break;
		}
		case(1):
		{
			LCD.BLIT(getxPos(), getyPos(), 15, 8, alienBobUp_sprite);
			break;
		}
	}
}

// end class AlienBob -------------------------------------



// class AlienAlice ---------------------------------------
AlienAlice::AlienAlice(int n)
{
	setxPos(18 + rand() % 110);
	setyPos((10+10*(n-1)) + rand() % 2);
	setDirec(true);
}

void AlienAlice::draw(uLCD_4DGL& LCD)
{
	LCD.BLIT(getxPos(), getyPos(), 15, 8, alienAlice_sprite);
}

void AlienAlice::update(uLCD_4DGL& LCD)
{
	changexPos(2);
	LCD.BLIT(getxPos(), getyPos(), 15, 8, alienAlice_sprite);
}
// end class AlienAlice -----------------------------------



// class AlienJim -----------------------------------------
AlienJim::AlienJim(int n)
{
	setxPos(18 + rand() % 110);
	setyPos((10+10*(n-1)) + rand() % 2);
	setDirec(true);
}

void AlienJim::draw(uLCD_4DGL& LCD)
{
	LCD.BLIT(getxPos(), getyPos(), 15, 8, alienJim_sprite);
}

void AlienJim::update(uLCD_4DGL& LCD)
{
	changexPos(2);
	LCD.BLIT(getxPos(), getyPos(), 15, 8, alienJim_sprite);
}
// end class AlienJim -------------------------------------



// class AlienDan -----------------------------------------
AlienDan::AlienDan(int n)
{
	setxPos(18 + rand() % 110);
	setyPos((10+10*(n-1)) + rand() % 2);
	setDirec(true);
}

void AlienDan::draw(uLCD_4DGL& LCD)
{
	LCD.BLIT(getxPos(), getyPos(), 15, 8, alienDan_sprite);
}

void AlienDan::update(uLCD_4DGL& LCD)
{
	changexPos(2);
	LCD.BLIT(getxPos(), getyPos(), 15, 8, alienDan_sprite);
}
// end class AlienDan -------------------------------------

// end of attack.cpp --------------------------------------