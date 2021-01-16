#include "Animation.h"


Animation::Animation(sf::Texture* texture, sf::Vector2u imageCount, float switchTime) {
	this->imageCount = imageCount; // Because variable are the same name
	this->switchTime = switchTime; // Because variable are the same name
	totalTime = 0.0f; // Set TIME
	currentImage.x = 0; // Start from first image

	uvRect.width = texture->getSize().x / float(imageCount.x); // Cal width of each picture
	uvRect.height = texture->getSize().y / float(imageCount.y); // Cal height of each picture

}

Animation::~Animation() {
}

void Animation::Update(int row, float deltaTime, bool ReFirstRowAnimation) {

	currentImage.y = row;
	totalTime += deltaTime;


	if (totalTime >= switchTime) {
		totalTime = totalTime - switchTime; //Or = 0

		currentImage.x++; // Go to next image


		if (currentImage.x >= imageCount.x && ReFirstRowAnimation == true) {
			currentImage.x = 0; // Back to first image
		}

	}

	uvRect.left = currentImage.x*uvRect.width; // Set Size of a new image
	uvRect.top = currentImage.y*uvRect.height;
}





