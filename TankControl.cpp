#include "TankControl.h"


TankControl::TankControl() {
}


TankControl::~TankControl() {
}

void TankControl::MoveTank(sf::RectangleShape &player, sf::RectangleShape &turret, bool howToPlay, float itemSpeedPick, bool mouseControl) {
	if (mouseControl == false) {
		if (howToPlay == false) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && player.getPosition().x >= 60) {
				player.move((-8.0f*itemSpeedPick), 0.0f); // Go LEFT
				turret.move((-8.0f*itemSpeedPick), 0.0f);

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && player.getPosition().x <= 1380) {
				player.move((+8.0f*itemSpeedPick), 0.0f); // Go RIGHT
				turret.move((+8.0f*itemSpeedPick), 0.0f);
			}
		}
		else if (howToPlay == true) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && player.getPosition().x >= 110) {
				player.move(-8.0f, 0.0f); // Go LEFT
				turret.move(-8.0f, 0.0f);

			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && player.getPosition().x <= 1330) {
				player.move(+8.0f, 0.0f); // Go RIGHT
				turret.move(+8.0f, 0.0f);
			}
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) {
		// SHOOT Main Gun
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) {
		// Launch NUKE
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
		// PAUSE Game
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J)) {
		// Turn Gun to the LEFT
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
		// Turn Gun to the RIGHT
	}

}


void TankControl::AimGun(sf::RectangleShape & player, sf::RenderWindow &window, sf::RectangleShape &turret, sf::RectangleShape &fireball) {

	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	width = static_cast<float>(window.getSize().x);
	height = static_cast<float>(window.getSize().y - 75);
	if (mousePos.y <= 850.0f) {
		x = static_cast<float>(mousePos.x) - turret.getPosition().x;
		y = height - static_cast<float>(mousePos.y);
	}

	tan_value = y / x;
	tan_value = atan(tan_value);
	New_tan_value = static_cast<float>((tan_value * 360) / (2 * 3.14));
	//printf("%f\n", tan_value);

	if (New_tan_value >= 0) {
		New_tan_value = 90 - New_tan_value;
	}
	if (New_tan_value < 0) {
		New_tan_value = 270 - New_tan_value;
	}
	if (New_tan_value < 271 && New_tan_value > 270) {
		New_tan_value = 0;
	}

	turret.setRotation(New_tan_value);
	fireball.setRotation(New_tan_value);

}
