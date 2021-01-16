#include "Enemy.h"



Enemy::Enemy(sf::Texture * texture, sf::Vector2f pos, int inputHP) {
	this->HPMax = inputHP;
	this->HP = this->HPMax;

	this->shape.setTexture(*texture);

	this->shape.setScale(1.75f, 1.75f);

	this->shape.setPosition(pos);

}

Enemy::~Enemy() {
}
