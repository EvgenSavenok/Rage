﻿#include "Animations.h"
#include "Map.h"

Animations::Animations()
{
	cageImg.loadFromFile("Images/cage.png");
	cageTexture.loadFromImage(cageImg);
	cageSprite.setTexture(cageTexture);
	cageSprite.setTextureRect(sf::IntRect(0, 0, 90, 90));


	doorImg.loadFromFile("Images/doors.png");
	doorTexture.loadFromImage(doorImg);
	doorSprite.setTexture(doorTexture);
	doorSprite.setTextureRect(sf::IntRect(107, 7, 95, 95));

	leftDoorImg.loadFromFile("Images/doors.png");
	leftDoorTexture.loadFromImage(leftDoorImg);
	leftDoorSprite.setTexture(leftDoorTexture);
	leftDoorSprite.setTextureRect(sf::IntRect(154.5, 7, 47.5, 95));

	rightDoorImg.loadFromFile("Images/doors.png");
	rightDoorTexture.loadFromImage(rightDoorImg);
	rightDoorSprite.setTexture(rightDoorTexture);
	rightDoorSprite.setTextureRect(sf::IntRect(202, 7, 47.5, 95));

	initializeDoors();
}

void Animations::initializeDoors()
{
	for (int i = 0; i < 36; i++)
	{
		for (int j = 0; j < 43; j++)
		{
			if (map.firstLevelMap[i][j] == 'D')
			{
				numOfDoors++;
				position = sf::Vector2f(j, i);
				doors.push_back(position);
				isCanStartDoorAnimation.push_back(false);
			}
		}
	}
}

void Animations::setCagePos(sf::RenderWindow& window, Boxes& box, Map& map, sf::Clock& cageClock)
{
	if (getCageAnimationState())
	{
		map.firstLevelMap[12][31] = ' ';
	}
	else
	{
		cageSprite.setPosition(brickSize * 31, brickSize * 12);
		window.draw(cageSprite);
		startCageAnimation(box, map, window, cageClock);
	}
}

void Animations::startCageAnimation(Boxes& box, Map& map, sf::RenderWindow& window, sf::Clock& cageClock)
{
	#define numOfActivePoints 2
	int countOfPoints = 0;
	for (int i = 0; i < 36; i++)
	{
		for (int j = 0; j < 43; j++)
		{
			if (map.firstLevelMap[i][j] == 'G')
				countOfPoints++;
		}
	}
	if (countOfPoints == numOfActivePoints)
	{
		sf::Time curTime = cageClock.getElapsedTime();
		if (curTime.asSeconds() >= 0.01) 
		{
			setCageHeight(getCageHeight() - 1);
			cageSprite.setTextureRect(sf::IntRect(0, 0, 90, getCageHeight()));
			window.draw(cageSprite);
			if (getCageHeight() == 0)
				setCageAnimationState(true);
			cageClock.restart();
		}
	}
}

int Animations::checkCurDoorNum(int curRow, int curCol)
{
	for (int i = 0; i < numOfDoors; i++)
	{
		if ((curCol == doors[i].x) && (curRow == doors[i].y))
		{
			return i;
		}
	}
}

void Animations::checkOnOpenDoor(Map& map, sf::RenderWindow& window, sf::Clock& leftDoorClock, sf::Clock& rightDoorClock)
{
	for (int i = 0; i < numOfDoors; i++)
	{
		if (getDoorAnimationState(i))
		{
			startDoorAnimation(i, map, window, leftDoorClock, rightDoorClock);
		}
	}
}

void Animations::startDoorAnimation(int numOfDoor, Map& map, sf::RenderWindow& window, sf::Clock& leftDoorClock, sf::Clock& rightDoorClock)
{
	int curDoorCol = doors[numOfDoor].x;
	int curDoorRow = doors[numOfDoor].y;
	map.firstLevelMap[curDoorRow][curDoorCol] = 'C';
	if ((leftDoorClock.getElapsedTime().asMilliseconds() > 85) && (leftDoorStartX < 252.0) && (leftDoorWidth > 0))
	{
		leftDoorOffset--;//создай массив с координатами и такими штуками для каждой двери
		leftDoorWidth -= 1;//еще есть баг, где если быстро ласт ящик переместить над платформой, то клетка откроется частично
		leftDoorStartX++;
		leftDoorClock.restart();
	}
	if ((rightDoorClock.getElapsedTime().asMilliseconds() > 100) && (rightDoorOffset < 95) && (rightDoorWidth > 0))
	{
		rightDoorOffset++;
		rightDoorWidth -= 1;
		rightDoorClock.restart();
	}
	if (leftDoorWidth <= 0)
	{
		map.firstLevelMap[curDoorRow][curDoorCol] = ' ';
	}
	leftDoorSprite.setTextureRect(sf::IntRect(leftDoorStartX, 7, leftDoorWidth, 95));
	leftDoorSprite.setPosition(brickSize * curDoorCol, brickSize * curDoorRow);
	window.draw(leftDoorSprite);
	rightDoorSprite.setTextureRect(sf::IntRect(250.0, 7, rightDoorWidth, 95));
	rightDoorSprite.setPosition(brickSize * curDoorCol + rightDoorOffset, brickSize * curDoorRow);
	window.draw(rightDoorSprite);
}