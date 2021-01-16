#include "Menu.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>


Menu::Menu(float width, float height) {
	if (!font.loadFromFile("Font/coolvetica/coolvetica condensed rg.ttf")) {
		//Error
		std::cout << "Error FONT coolvetica.";
	}
	float fontSize = 120;

	float startPosX = (width / 2) + 100;


	menu[0].setFont(font);
	menu[0].setCharacterSize(fontSize + 150);
	menu[0].setFillColor(sf::Color::Red);
	menu[0].setString("PLAY");
	menu[0].setPosition(sf::Vector2f(startPosX, 150 + 10));

	menu[1].setFont(font);
	menu[1].setCharacterSize(fontSize);
	menu[1].setFillColor(sf::Color::White);
	menu[1].setString("OPTIONS");
	menu[1].setPosition(sf::Vector2f(startPosX, 400 + 10));

	menu[2].setFont(font);
	menu[2].setCharacterSize(fontSize);
	menu[2].setFillColor(sf::Color::White);
	menu[2].setString("HOW TO PLAY");
	menu[2].setPosition(sf::Vector2f(startPosX, 475 + 20));

	menu[3].setFont(font);
	menu[3].setCharacterSize(fontSize);
	menu[3].setFillColor(sf::Color::White);
	menu[3].setString("LEADERBOARD");
	menu[3].setPosition(sf::Vector2f(startPosX, 550 + 30));

	menu[4].setFont(font);
	menu[4].setCharacterSize(fontSize);
	menu[4].setFillColor(sf::Color::White);
	menu[4].setString("EXIT");
	menu[4].setPosition(sf::Vector2f(startPosX, 625 + 40));

	selectedItemIndex = 0;


}

Menu::~Menu() {
}



void Menu::drawMenu(sf::RenderWindow & window, std::string name) {
	for (int i = 0; i < MAX_NUMBER_OF_ITEMS; i++) {
		window.draw(menu[i]);
	}

	float detailX = 475;
	float detailY = 840;

	std::string playerNameString = name;

	if (selectedItemIndex == 0) {
		detail.setFont(font);
		detail.setCharacterSize(50);
		detail.setFillColor(sf::Color::White);
		detail.setLineSpacing(0.75);
		detail.setString("			Set out on a multi-staged\n	survival campaign to obliterate\n				the forces of tyranny!");
		detail.setPosition(sf::Vector2f(detailX, detailY));
		window.draw(detail);
	}
	else if (selectedItemIndex == 1) {
		detail.setFont(font);
		detail.setCharacterSize(50);
		detail.setFillColor(sf::Color::White);
		detail.setLineSpacing(0.75);
		detail.setString("					Modify difficulty of\n						 the game here.");
		detail.setPosition(sf::Vector2f(detailX, detailY + 25));
		window.draw(detail);
	}
	else if (selectedItemIndex == 2) {
		detail.setFont(font);
		detail.setCharacterSize(50);
		detail.setFillColor(sf::Color::White);
		detail.setLineSpacing(0.75);
		detail.setString("							Get briefed on\n					    how to play here!");
		detail.setPosition(sf::Vector2f(detailX, detailY + 25));
		window.draw(detail);
	}
	else if (selectedItemIndex == 3) {
		detail.setFont(font);
		detail.setCharacterSize(50);
		detail.setFillColor(sf::Color::White);
		detail.setLineSpacing(0.75);
		detail.setString("				Check the high scroes\n					   to see if you are\n				     a Hero of Freedom!");
		detail.setPosition(sf::Vector2f(detailX, detailY));
		window.draw(detail);
	}
	else if (selectedItemIndex == 4) {
		detail.setFont(font);
		detail.setCharacterSize(50);
		detail.setFillColor(sf::Color::White);
		detail.setLineSpacing(0.75);
		detail.setString("				Surrender and return\n						  to Windows.");
		detail.setPosition(sf::Vector2f(detailX, detailY + 25));
		window.draw(detail);
	}

	if (!mainFont.loadFromFile("Font/chernobyl/Chernobyl.otf")) {
		//Error
		std::cout << "Error FONT Modern Machine.";
	}
	gameName.setFont(mainFont);
	gameName.setCharacterSize(215);
	gameName.setFillColor(sf::Color::White);
	gameName.setLineSpacing(0.75);
	gameName.setString("  The\nLast\nTank");
	gameName.setPosition(sf::Vector2f(200, 200));
	window.draw(gameName);


	detail.setFont(font);
	detail.setCharacterSize(50);
	detail.setFillColor(sf::Color::White);
	detail.setLineSpacing(0.75);
	detail.setString("Welcome Back,\n	  If this isn't you, Press 'Ctrl'.");
	detail.setPosition(sf::Vector2f(detailX + 30, 80));
	window.draw(detail);

	playerName.setFont(font);
	playerName.setCharacterSize(50);
	playerName.setFillColor(sf::Color::Yellow);
	playerName.setLineSpacing(0.75);
	playerName.setString(playerNameString);
	playerName.setOrigin(playerName.getGlobalBounds().width, 0);
	playerName.setPosition(sf::Vector2f(detailX + 470, 80));
	window.draw(playerName);

	studentID.setFont(font);
	studentID.setCharacterSize(30);
	studentID.setFillColor(sf::Color::Black);
	studentID.setLineSpacing(0.75);
	studentID.setString("   62010694 Pakorn Thanaprachanon");
	studentID.setOrigin(0, playerName.getGlobalBounds().height);
	studentID.setPosition(sf::Vector2f(0, 1080));
	window.draw(studentID);

}

void Menu::MoveUp() {
	if (selectedItemIndex - 1 >= 0) {
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex--;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
	}


}

void Menu::MoveDown() {
	if (selectedItemIndex + 1 < MAX_NUMBER_OF_ITEMS) {
		menu[selectedItemIndex].setFillColor(sf::Color::White);
		selectedItemIndex++;
		menu[selectedItemIndex].setFillColor(sf::Color::Red);
	}


}

// Check if the mouse is within the bounds of the button:
bool Menu::isMouseOver(sf::RenderWindow &window, int numMenu) {
	int mouseX = sf::Mouse::getPosition(window).x;
	int mouseY = sf::Mouse::getPosition(window).y;

	int btnPosX = menu[numMenu].getPosition().x;
	int btnPosY = menu[numMenu].getPosition().y;

	int btnxPosWidth = menu[numMenu].getPosition().x + menu[numMenu].getGlobalBounds().width;
	int btnyPosHeight = menu[numMenu].getPosition().y + menu[numMenu].getGlobalBounds().height + 100;

	if (mouseX <= btnxPosWidth && mouseX >= btnPosX && mouseY <= btnyPosHeight && mouseY >= btnPosY) {
		return true;
	}
	return false;
}

void Menu::changeItem(int numMenu) {
	menu[selectedItemIndex].setFillColor(sf::Color::White);
	selectedItemIndex = numMenu;
	menu[selectedItemIndex].setFillColor(sf::Color::Red);
}
