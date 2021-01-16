#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <ctime>    
#include <vector>    
#include <stdio.h>
#include <math.h>


class TankControl {
public:
	TankControl();  // Constructor
	~TankControl(); // Deconstructor
public:
	void MoveTank(sf::RectangleShape &player, sf::RectangleShape &turret, bool howToPlay, float itemSpeedPick, bool mouseControl);
	void AimGun(sf::RectangleShape &player, sf::RenderWindow &window, sf::RectangleShape &turret, sf::RectangleShape &fireball);

public:
	float x, y, width, height;
	float tan_value, New_tan_value, Real_value, New_tan_value_mi;

};

