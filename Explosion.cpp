#include "Explosion.h"




Explosion::Explosion(sf::Texture *textureExplosion, sf::Vector2f pos, float ratioSize) {
	this->explosionShape.setTexture(*textureExplosion);

	this->explosionShape.setScale(1.10f*ratioSize, 1.10f*ratioSize);
	this->explosionShape.setPosition(pos);
}

Explosion::~Explosion() {

}

