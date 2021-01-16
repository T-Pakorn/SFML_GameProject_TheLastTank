#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Animation {
public://Function
	Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime);
	~Animation();

	void Update(int row, float deltaTime, bool ReFirstRowAnimation); //Update Function

public://Variable
	sf::IntRect uvRect; // XY position to display animation


private://Variable
	sf::Vector2u imageCount;
	sf::Vector2u currentImage;

	float totalTime; // Time last to change image
	float switchTime;
};

