#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#define MAX_NUMBER_OF_ITEMS 5

class Menu {
public:
	Menu(float width, float height);
	~Menu();

	void drawMenu(sf::RenderWindow &window, std::string name);
	void MoveUp();
	void MoveDown();
	int GetPressedItem() { return selectedItemIndex; }
	bool isMouseOver(sf::RenderWindow &window, int numMenu);
	void changeItem(int numMenu);
	std::string playerNameString = "New Player";

	int selectedItemIndex;
	sf::Font font;
	sf::Text menu[MAX_NUMBER_OF_ITEMS];

	sf::Text detail;

	sf::Text gameName;
	sf::Font mainFont;

	sf::Text playerName;

	sf::Text studentID;
};

