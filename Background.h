#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <ctime>    
#include <vector>    
#include <stdio.h>
#include <math.h>

class Background {
public:
	Background();
	~Background();

public:
	void Wall(bool howToPlay);
	void WallUpdate(bool howToPlay);

public:
	float startX = 1440;
	float startMidY = 535;
	float startGroundY = 950;

	sf::Texture background1; //Sky
	sf::Texture background2;
	sf::Sprite spriteBackground1;
	sf::Sprite spriteBackground2;

	float countBackgroundF1;
	float countBackgroundF2;

	sf::Texture farBackground1; //Far Background
	sf::Texture farBackground2;
	sf::Sprite spriteFarBackground1;
	sf::Sprite spriteFarBackground2;

	float countFarBackgroundF1;
	float countFarBackgroundF2;

	sf::Texture nearBackground1; //Near Background
	sf::Texture nearBackground2;
	sf::Sprite spriteNearBackground1;
	sf::Sprite spriteNearBackground2;

	float countNearBackgroundF1;
	float countNearBackgroundF2;

	sf::Texture closeBackground1; //Close Background
	sf::Texture closeBackground2;
	sf::Sprite spriteCloseBackground1;
	sf::Sprite spriteCloseBackground2;

	float countCloseBackgroundF1;
	float countCloseBackgroundF2;



};

