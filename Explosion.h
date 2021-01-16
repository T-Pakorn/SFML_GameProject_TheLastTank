#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Explosion {
public:
	Explosion(sf::Texture *texture, sf::Vector2f pos, float ratioSize);
	~Explosion();

public:

	sf::Sprite explosionShape;
};

