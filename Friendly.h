#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include <iostream>

class Friendly {
public:
	Friendly(sf::Texture * texture, sf::Vector2f pos, int inputHP, float ratioSize);
	~Friendly();

public:
	int HP;
	int HPMax;
	sf::Sprite shape;
};

