#pragma once
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include <iostream>

class Enemy {
public:
	Enemy(sf::Texture *texture, sf::Vector2f pos, int inputHP);
	~Enemy();

public:
	int HP;
	int HPMax;
	sf::Sprite shape;

};

