#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Bullet {
public:
	sf::CircleShape shape; // Circle to be Bullet
	sf::Vector2f currVelocity; // Current Velocity
	float maxSpeed;


public:
	Bullet(float radius = 5.f)
		: currVelocity(0.f, 0.f), maxSpeed(25.f) {
		this->shape.setRadius(radius);
		this->shape.setFillColor(sf::Color::Yellow);

	}

};

