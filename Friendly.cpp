#include "Friendly.h"




Friendly::Friendly(sf::Texture * texture, sf::Vector2f pos, int inputHP, float ratioSize) {
	this->HPMax = inputHP;
	this->HP = this->HPMax;

	this->shape.setTexture(*texture);
	this->shape.setScale(1.10f*ratioSize, 1.10f*ratioSize);

	this->shape.setPosition(pos);
}

Friendly::~Friendly() {
}
