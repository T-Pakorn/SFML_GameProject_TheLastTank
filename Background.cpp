#include "Background.h"



Background::Background() {
}


Background::~Background() {
}

void Background::Wall(bool howToPlay) {
	if (howToPlay == false) {
		// SKY UPDATE
		background1.loadFromFile("Background/Background1.jpg"); // LOAD Texture
		background2.loadFromFile("Background/Background1.jpg");
		background1.setRepeated(true);
		background2.setRepeated(true);
		spriteBackground1.setTexture(background1);
		spriteBackground2.setTexture(background2);
		spriteBackground1.setPosition(0, 0);
		spriteBackground2.setPosition(startX - 5, 0);

		// Far background UPDATE
		farBackground1.loadFromFile("Background/FarBackground1.png"); // LOAD Texture
		farBackground2.loadFromFile("Background/FarBackground1.png");
		farBackground1.setRepeated(true);
		farBackground2.setRepeated(true);
		spriteFarBackground1.setTexture(farBackground1);
		spriteFarBackground2.setTexture(farBackground2);
		spriteFarBackground1.setPosition(0, startMidY);
		spriteFarBackground2.setPosition(startX - 5, startMidY);

		// Near background UPDATE
		nearBackground1.loadFromFile("Background/NearBackground1.png"); // LOAD Texture
		nearBackground2.loadFromFile("Background/NearBackground1.png");
		nearBackground1.setRepeated(true);
		nearBackground2.setRepeated(true);
		spriteNearBackground1.setTexture(nearBackground1);
		spriteNearBackground2.setTexture(nearBackground2);
		spriteNearBackground1.setPosition(0, startMidY);
		spriteNearBackground2.setPosition(startX - 5, startMidY);

	}

	// Close background UPDATE
	closeBackground1.loadFromFile("Background/CloseBackground1.jpg"); // LOAD Texture
	closeBackground2.loadFromFile("Background/CloseBackground1.jpg");
	closeBackground1.setRepeated(true);
	closeBackground2.setRepeated(true);
	spriteCloseBackground1.setTexture(closeBackground1);
	spriteCloseBackground2.setTexture(closeBackground2);




	if (howToPlay == false) {
		spriteCloseBackground1.setPosition(0, startGroundY);
		spriteCloseBackground2.setPosition(startX - 5, startGroundY);
	}
	else if (howToPlay == true) {
		spriteCloseBackground1.setPosition(0, startGroundY - 70);
		spriteCloseBackground2.setPosition(startX - 5, startGroundY - 70);

	}
}

void Background::WallUpdate(bool howToPlay) {
	if (howToPlay == false) {

		countBackgroundF1 = spriteBackground1.getPosition().x; //SKY UPDATE
		countBackgroundF2 = spriteBackground2.getPosition().x;
		if (countBackgroundF1 >= -startX) {
			spriteBackground1.move(-0.35f, 0.0f);
		}
		else {
			spriteBackground1.setPosition(startX - 10, 0);
		}
		if (countBackgroundF2 >= -startX) {
			spriteBackground2.move(-0.35, 0.0f);
		}
		else {
			spriteBackground2.setPosition(startX - 10, 0);
		}

		countFarBackgroundF1 = spriteFarBackground1.getPosition().x; //Far Background UPDATE
		countFarBackgroundF2 = spriteFarBackground2.getPosition().x;
		if (countFarBackgroundF1 >= -startX) {
			spriteFarBackground1.move(-0.5f, 0.0f);
		}
		else {
			spriteFarBackground1.setPosition(startX - 10, startMidY);
		}
		if (countFarBackgroundF2 >= -startX) {
			spriteFarBackground2.move(-0.5, 0.0f);
		}
		else {
			spriteFarBackground2.setPosition(startX - 10, startMidY);
		}

		countNearBackgroundF1 = spriteNearBackground1.getPosition().x; //Near Background UPDATE
		countNearBackgroundF2 = spriteNearBackground2.getPosition().x;
		if (countNearBackgroundF1 >= -startX) {
			spriteNearBackground1.move(-1.0f, 0.0f);
		}
		else {
			spriteNearBackground1.setPosition(startX - 10, startMidY);
		}
		if (countNearBackgroundF2 >= -startX) {
			spriteNearBackground2.move(-1.0, 0.0f);
		}
		else {
			spriteNearBackground2.setPosition(startX - 10, startMidY);
		}
	}

	countCloseBackgroundF1 = spriteCloseBackground1.getPosition().x; //Close Background UPDATE
	countCloseBackgroundF2 = spriteCloseBackground2.getPosition().x;
	if (howToPlay == false) {
		if (countCloseBackgroundF1 >= -startX) {
			spriteCloseBackground1.move(-4.5f, 0.0f);
		}
		else {
			spriteCloseBackground1.setPosition(startX - 20, startGroundY);
		}
		if (countCloseBackgroundF2 >= -startX) {
			spriteCloseBackground2.move(-4.5, 0.0f);
		}
		else {
			spriteCloseBackground2.setPosition(startX - 20, startGroundY);
		}
	}
	else if (howToPlay == true) {
		if (countCloseBackgroundF1 >= -startX) {
			spriteCloseBackground1.move(-4.5f, 0.0f);
		}
		else {
			spriteCloseBackground1.setPosition(startX - 20, startGroundY - 70);
		}
		if (countCloseBackgroundF2 >= -startX) {
			spriteCloseBackground2.move(-4.5, 0.0f);
		}
		else {
			spriteCloseBackground2.setPosition(startX - 20, startGroundY - 70);
		}

	}
}
