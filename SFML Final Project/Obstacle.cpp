#include "Obstacle.h"

obstacle::obstacle() {
}

obstacle::obstacle(sf::Vector2f position, sf::Texture* obstacleTexture)
{
	obstacleSprite.setTexture(*obstacleTexture);
	obstacleSprite.setScale(1.5, 1.5);
	obstacleSprite.setPosition(position.x, position.y);

	srand(time(NULL));

	speed = -5;
}

void obstacle::reposition()
{
	obstacleSprite.setPosition(sf::Vector2f(852, randomizeYPos()));
}

void obstacle::update()
{
	obstacleSprite.move(speed, 0);
	if (obstacleSprite.getPosition().x < 0)
	{
		reposition();
	}
}

int obstacle::randomizeYPos()
{
	int randomY;
	int thePosition = 0;
	randomY = rand() % 2;
	if (randomY == 0)
	{
		thePosition = 392;
	}
	else if (randomY == 1)
	{
		thePosition = 300;
	}
	return thePosition;
}
