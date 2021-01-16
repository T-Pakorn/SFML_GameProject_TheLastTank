#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <chrono>
#include <ctime> 
#include <vector>
#include <cstdlib>
#include <math.h>
#include <iomanip>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>


#include "Animation.h"
#include "TankControl.h"
#include "Bullet.h"
#include "Background.h"
#include "Enemy.h"
#include "Menu.h"
#include "Explosion.h"
#include "Friendly.h"
#include "TextBox.h"

sf::RenderWindow window;
sf::RectangleShape player;
sf::RectangleShape turret;
sf::RectangleShape fireball;

Textbox textbox1(120, sf::Color::White, false);

TankControl tankPlayer; // Create OBJECT for TankControl CLASS
Background wallpaper;

// reverses a string 'str' of length 'len' 
void reverse(char *str, int len) {
	int i = 0, j = len - 1, temp;
	while (i < j) {
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++; j--;
	}
}

// Converts a given integer x to string str[].  d is the number 
// of digits required in output. If d is more than the number 
// of digits in x, then 0s are added at the beginning. 
int intToStr(int x, char str[], int d) {
	int i = 0;
	while (x) {
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}

	// If number of digits required is more, then 
	// add 0s at the beginning 
	while (i < d)
		str[i++] = '0';

	reverse(str, i);
	str[i] = '\0';
	return i;
}

// Converts a floating point number to string. 
void ftoa(float n, char *res, int afterpoint) {
	// Extract integer part 
	int ipart = (int)n;

	// Extract floating part 
	float fpart = n - (float)ipart;

	// convert integer part to string 
	int i = intToStr(ipart, res, 0);

	// check for display option after point 
	if (afterpoint != 0) {
		res[i] = '.';  // add dot 

		// Get the value of fraction part upto given no. 
		// of points after dot. The third parameter is needed 
		// to handle cases like 233.007 
		fpart = static_cast<float>(fpart * pow(10, afterpoint));

		intToStr((int)fpart, res + i + 1, afterpoint);
	}
}




int main() {
	bool reSet = false;


	//HOW TO PLAY
	bool howToPlay = false;

	sf::Texture helpTex;
	sf::Sprite help;

	helpTex.loadFromFile("Background/help.jpg"); // LOAD Texture
	helpTex.setRepeated(true);
	help.setTexture(helpTex);
	help.setPosition(0, 0);

	sf::Texture helpOuterTex;
	sf::Sprite helpOuter;

	helpOuterTex.loadFromFile("Background/helpOuter.png"); // LOAD Texture
	helpOuterTex.setRepeated(true);
	helpOuter.setTexture(helpOuterTex);
	helpOuter.setPosition(0, 0);


	//HELI AIR SUPPORT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	std::vector<bool> stateFriendlyHit;
	std::vector<float> timeBlinkFriendly;

	int friendlyPoint = -1000;

	std::vector<Friendly> friendlyVector;

	float timeDiffAnimationFriendly = 0.01f; // Time to switch animation

	sf::Texture FriendlyTex; // Variable Texture
	FriendlyTex.loadFromFile("Enemy/friendly.png");
	FriendlyTex.setSmooth(true);
	std::vector<Animation> friendlyAnimation;/////////////////////////////

	bool airSupportNow = false;
	bool alreadyAirSupport = false;

	sf::Sprite dontShoot;
	sf::Texture dontShootTex;
	dontShootTex.loadFromFile("Enemy/dontshoot.png");
	dontShootTex.setSmooth(true);
	dontShoot.setTexture(dontShootTex);
	dontShoot.setScale(sf::Vector2f(2.6f, 2.6f));

	bool callAirSupport = false;

	bool friendlyCallOut1 = true;
	bool friendlyCallOut2 = true;
	bool friendlyCallOut3 = true;


	// TANK ICON

	sf::Sprite tankIcon;
	sf::Texture tankIconTex;
	tankIconTex.loadFromFile("Tank/tankicon.png");
	tankIconTex.setSmooth(true);
	tankIcon.setTexture(tankIconTex);
	tankIcon.setScale(sf::Vector2f(2.0f, 2.0f));
	tankIcon.setPosition(sf::Vector2f(290.0f, 32.0f));

	//MENU WALL
	sf::Sprite menuWall;
	sf::Texture menuWallTex;
	menuWallTex.loadFromFile("Background/MenuSFML.jpg");
	menuWallTex.setSmooth(true);
	menuWall.setTexture(menuWallTex);
	menuWall.setPosition(sf::Vector2f(0, 0));

	//shield tank
	sf::Sprite tankShield;
	sf::Texture tankShieldTex;
	tankShieldTex.loadFromFile("Tank/TankShield.png");
	tankShieldTex.setSmooth(true);
	tankShield.setTexture(tankShieldTex);
	tankShield.setScale(0.3f, 0.3f);


	//Blank
	sf::Sprite blank;
	sf::Texture blankTex;
	blankTex.loadFromFile("Background/BlankSFML.png");
	blankTex.setSmooth(true);
	blank.setTexture(blankTex);
	blank.setPosition(sf::Vector2f(0, 0));

	//BAR
	sf::Sprite bar;
	sf::Texture batTex;
	batTex.loadFromFile("Background/BarSFML.png");
	batTex.setSmooth(true);
	bar.setTexture(batTex);
	bar.setPosition(sf::Vector2f(0, 0));

	//BLINK
	std::vector<bool> stateBomberHit;
	std::vector<float> timeBlinkBomber;

	std::vector<bool> stateJetFighterHit;
	std::vector<float> timeBlinkJetFighter;

	std::vector<bool> stateMissleHit;
	std::vector<float> timeBlinkMissle;

	//DEATH
	bool playerAlive = false;
	float deathTime = 0;

	//TIME
	sf::Time micro = sf::microseconds(10000);
	sf::Time milli = sf::milliseconds(10);
	sf::Time second = sf::seconds(0.01f);

	sf::Clock clock;
	sf::Clock TimeMain;
	sf::Clock TimeItem;
	sf::Clock TimeItemDelay;
	float timeSinceStart;



	float timeSinceSupport;

	sf::Clock TimeMissleAttack;
	float timeMissleAttack;
	bool alreadyMissleFar = false;

	//SOUND START
	sf::Sound voiceGetReady;
	sf::SoundBuffer bufferVoiceGetReady;
	bufferVoiceGetReady.loadFromFile("Sound/v_getready.ogg");
	voiceGetReady.setBuffer(bufferVoiceGetReady);
	int counterStartVoice = 0;
	int voiceStartBegin = 10;

	//SOUND GAMEOVER
	sf::Sound voiceGameover;
	sf::SoundBuffer bufferVoiceGameover;
	bufferVoiceGameover.loadFromFile("Sound/v_gameover.ogg");
	voiceGameover.setBuffer(bufferVoiceGameover);

	//SOUND LAST ONE
	sf::Sound voiceLastOne;
	sf::SoundBuffer bufferVoiceLastOne;
	bufferVoiceLastOne.loadFromFile("Sound/v_lastone.ogg");
	voiceLastOne.setBuffer(bufferVoiceLastOne);

	//SOUND Speed Up
	sf::Sound speedUp;
	sf::SoundBuffer bufferSpeedUp;
	bufferSpeedUp.loadFromFile("Sound/speedup.ogg");
	speedUp.setBuffer(bufferSpeedUp);

	//SOUND shield
	sf::Sound shield;
	sf::SoundBuffer bufferShield;
	bufferShield.loadFromFile("Sound/shieldup.ogg");
	shield.setBuffer(bufferShield);

	//SOUND Nuke Up
	sf::Sound nukeUp;
	sf::SoundBuffer bufferNukeUp;
	bufferNukeUp.loadFromFile("Sound/powerup.ogg");
	nukeUp.setBuffer(bufferNukeUp);

	//SOUND Gun Up
	sf::Sound gunUp;
	sf::SoundBuffer bufferGunUp;
	bufferGunUp.loadFromFile("Sound/gunpowerup.ogg");
	gunUp.setBuffer(bufferGunUp);

	//SOUND rein
	sf::Sound rein;
	sf::SoundBuffer bufferRein;
	bufferRein.loadFromFile("Sound/reinforcements.ogg");
	rein.setBuffer(bufferRein);


	//SOUND SMALL EXPLOSION
	sf::Sound voiceSmallExplosion;
	sf::SoundBuffer bufferVoiceSmallExplosion;
	bufferVoiceSmallExplosion.loadFromFile("Sound/smallexplode.ogg");
	voiceSmallExplosion.setBuffer(bufferVoiceSmallExplosion);

	//SOUND TANK EXPLOSION
	sf::Sound voiceTankExplosion;
	sf::SoundBuffer bufferVoiceTankExplosion;
	bufferVoiceTankExplosion.loadFromFile("Sound/tankexplode.ogg");
	voiceTankExplosion.setBuffer(bufferVoiceTankExplosion);

	//SOUND STAR
	sf::Sound star;
	sf::SoundBuffer bufferStar;
	bufferStar.loadFromFile("Sound/tankexplode.ogg");
	star.setBuffer(bufferStar);

	//SOUND FRIENDLY
	sf::Sound voiceFriendly;
	sf::SoundBuffer bufferVoiceFriendly;
	bufferVoiceFriendly.loadFromFile("Sound/friendly1.ogg");
	voiceFriendly.setBuffer(bufferVoiceFriendly);

	int friendlyChangeSound = 1;

	//SOUND AIRRAID
	sf::Sound voiceAirRaid;
	sf::SoundBuffer bufferAirRaid;
	bufferAirRaid.loadFromFile("Sound/airraid.ogg");
	voiceAirRaid.setBuffer(bufferAirRaid);

	//SOUND MISSLE EXPLOSION
	sf::Sound voiceMissleExplosion;
	sf::SoundBuffer bufferMissleExplosion;
	bufferMissleExplosion.loadFromFile("Sound/bossblast.ogg");
	voiceMissleExplosion.setBuffer(bufferMissleExplosion);

	//SOUND LEADERBOARD
	sf::Sound voiceStats;
	sf::SoundBuffer bufferVoiceStats;
	bufferVoiceStats.loadFromFile("Sound/stats.ogg");
	voiceStats.setBuffer(bufferVoiceStats);

	//SOUND COPTER
	sf::Sound voiceCopter;
	sf::SoundBuffer bufferCopter;
	bufferCopter.loadFromFile("Sound/pupcopter.ogg");
	voiceCopter.setBuffer(bufferCopter);

	//SOUND CARE PACKAGE
	sf::Sound voiceCare;
	sf::SoundBuffer bufferCare;
	bufferCare.loadFromFile("Sound/v_care1.ogg");
	voiceCare.setBuffer(bufferCare);

	//SOUND CARE PACKAGE
	sf::Sound voicePrep;
	sf::SoundBuffer bufferPrep;
	bufferPrep.loadFromFile("Sound/v_prepare.ogg");
	voicePrep.setBuffer(bufferPrep);

	//SOUND lostShield
	sf::Sound lostShield;
	sf::SoundBuffer bufferLostShield;
	bufferLostShield.loadFromFile("Sound/shielddown.ogg");
	lostShield.setBuffer(bufferLostShield);

	//SOUND no
	sf::Sound no;
	sf::SoundBuffer bufferNo;
	bufferNo.loadFromFile("Sound/denied.ogg");
	no.setBuffer(bufferNo);

	//SOUND no
	sf::Sound fall;
	sf::SoundBuffer bufferFall;
	bufferFall.loadFromFile("Sound/nukeblast.ogg");
	fall.setBuffer(bufferFall);


	//SOUND MENU
	sf::Sound selectedMenuUp;
	sf::SoundBuffer bufferSelectedMenuUp;
	bufferSelectedMenuUp.loadFromFile("Sound/buttonup.ogg");
	selectedMenuUp.setBuffer(bufferSelectedMenuUp);
	sf::Sound selectedMenuDown;
	sf::SoundBuffer bufferSelectedMenuDown;
	bufferSelectedMenuDown.loadFromFile("Sound/buttondown.ogg");
	selectedMenuDown.setBuffer(bufferSelectedMenuDown);

	float VIEW_HEIGHT = 1080.0f;
	float VIEW_WIDTH = 1440.0f;

	//BLACK SCREEN
	sf::RectangleShape hideVoid(sf::Vector2f(3000.f, 1500.f));
	hideVoid.setFillColor(sf::Color::Black);
	hideVoid.setOrigin(0.0f, 0.0f);
	hideVoid.setPosition(1440, 0);

	//Escape Factor of Enemy
	int factorEscapeEnemy = 12;

	//MUSIC
	sf::Music MainTheme;

	// SOUND GUN
	sf::SoundBuffer bufferGunSound; //Buffer SOUND load and wait to use
	sf::Sound soundGun; //Sound Variable
	int soundGunChange = 1;
	sf::SoundBuffer bufferBulletHit; //Buffer SOUND load and wait to use
	sf::Sound soundBulletHit; //Sound Variable
	sf::SoundBuffer bufferBombFall;
	sf::Sound soundBombFall;
	sf::Sound soundBigExplode;
	sf::SoundBuffer bufferBigExplode;


	//Background
	wallpaper.Wall(howToPlay);

	//HOW TO
	sf::Sprite howTo;
	sf::Texture howToTex;
	howToTex.loadFromFile("Background/HowToSFML.png");
	howToTex.setSmooth(true);
	howTo.setTexture(howToTex);
	howTo.setPosition(sf::Vector2f(0, 0));

	//Shaking
	int shaking = 0;

	//Random Time
	srand(static_cast<int>(time(NULL)));

	//View
	sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));

	//Window
	sf::RenderWindow window(sf::VideoMode(1440, 1080), "The Last Tank", sf::Style::Close | sf::Style::Titlebar); // Screen SET-UP
	window.setFramerateLimit(60);
	window.setPosition(sf::Vector2i(0, -40));

	//Init FONT
	sf::Font smallFont;
	smallFont.loadFromFile("Font/coolvetica/coolvetica condensed rg.ttf");

	//Init Enemy Texture
	sf::Texture EnemyBomberTex;
	EnemyBomberTex.loadFromFile("Enemy/bomber.png");

	//Init Enemy Texture
	sf::Texture DumbBombTex;
	DumbBombTex.loadFromFile("Enemy/dumbbomb.png");
	DumbBombTex.setSmooth(true);

	//Init Enemy Texture
	sf::Texture EnemyJetFighterTex;
	EnemyJetFighterTex.loadFromFile("Enemy/jetfighter.png");

	//Init Enemy Texture
	sf::Texture EnemyMissleTex;
	EnemyMissleTex.loadFromFile("Enemy/bigmissilenear.png");
	EnemyMissleTex.setSmooth(true);
	std::vector<Animation> missleNearAnimation;/////////////////////////////

	//SCORE
	int fontSizeTop = 55;
	sf::Text score;
	score.setFont(smallFont);
	score.setCharacterSize(fontSizeTop);
	score.setFillColor(sf::Color::Yellow);
	score.setPosition(30.0f, 10.0f);
	int totalScore = 0;
	int bomberPoint = 200; //Point For KILL BOMBER
	int dumbBombPoint = 20;

	int jetFighterPoint = 500; //Point For KILL Jet

	int misslePoint = 700; //Point For KILL BOMBER

	//Reinforcement TEXT
	sf::Text reinforcement;
	reinforcement.setFont(smallFont);
	reinforcement.setCharacterSize(fontSizeTop);
	reinforcement.setFillColor(sf::Color::Green);
	reinforcement.setPosition(340.0f, 10.0f);
	int totalReinforcement = 2;

	//TIME TEXT
	sf::Text timeSurvival;
	timeSurvival.setFont(smallFont);
	timeSurvival.setCharacterSize(fontSizeTop);
	timeSurvival.setFillColor(sf::Color::Yellow);
	timeSurvival.setPosition(665.0f, 10.0f);
	float survivalTime;

	//TIME TILL ITEM TEXT
	sf::Text timeSupport;
	timeSupport.setFont(smallFont);
	timeSupport.setCharacterSize(fontSizeTop);
	timeSupport.setFillColor(sf::Color::Yellow);
	timeSupport.setPosition(1015.0f, 10.0f);
	float timeTillSupport = 35;

	//Announcement TEXT
	sf::Text announcement;
	announcement.setFont(smallFont);
	announcement.setCharacterSize(150);
	announcement.setFillColor(sf::Color::Green);
	announcement.setStyle(sf::Text::Bold);
	announcement.setOutlineColor(sf::Color::White);
	announcement.setOutlineThickness(10);
	announcement.setPosition(static_cast<float>(window.getSize().x / 2) - 240, static_cast<float>(window.getSize().y / 2) - 100);

	//Announcement TEXT
	sf::Text itemCallOut;
	itemCallOut.setFont(smallFont);
	itemCallOut.setCharacterSize(150);
	itemCallOut.setFillColor(sf::Color::Yellow);
	itemCallOut.setStyle(sf::Text::Bold);




	//Game Over TEXT
	sf::Text gameover;
	gameover.setFont(smallFont);
	gameover.setCharacterSize(150);
	gameover.setFillColor(sf::Color::Red);
	gameover.setStyle(sf::Text::Bold);
	gameover.setOutlineColor(sf::Color::White);
	gameover.setOutlineThickness(10);
	gameover.setPosition(static_cast<float>(window.getSize().x / 2) - 240, static_cast<float>(window.getSize().y / 2) - 100);

	//Player TANK
	sf::RectangleShape player(sf::Vector2f(96.3f, 52.5f)); // CUBE to represent Player
	//player.setFillColor(sf::Color::Red); // Set COLOR to cube
	player.setOrigin(50.0f, 50.0f); // Set origin of the player
	player.setScale(static_cast<float>(1.3), static_cast<float>(1.3));
	player.setPosition(720.0f, 1047.0f);
	sf::Texture playerTexture; // Variable Texture
	playerTexture.setSmooth(true);
	playerTexture.loadFromFile("Tank/TankMoving.png"); // LOAD Texture
	player.setTexture(&playerTexture); // Add Texture
	float timeDiffAnimation = 0.02f; // Time to switch animation
	Animation animation(&playerTexture, sf::Vector2u(5, 2), timeDiffAnimation);
	float deltaTime = 0.0f;

	//CURSOR
	float timeCursorDiffAnimation = 0.05f;
	sf::Texture newCursorTex; // Variable Texture
	newCursorTex.loadFromFile("Cursor/targetAll.png");
	newCursorTex.setSmooth(true);
	Explosion newCursor(&newCursorTex, sf::Vector2f(static_cast<float>(sf::Mouse::getPosition().x), static_cast<float>(sf::Mouse::getPosition().y)), 2.0f);
	Animation newCursorAnimation(&newCursorTex, sf::Vector2u(8, 1), timeCursorDiffAnimation);

	//MISSILE FAR

	std::vector<Explosion> missleFarVector;


	float timeDiffAnimationMissleFar = 0.05f; // Time to switch animation

	sf::Texture missleFarTexture; // Variable Texture
	missleFarTexture.loadFromFile("Enemy/bigmissilefar.png");
	std::vector<Animation> missleFarAnimation;/////////////////////////////

	//NORMAL EXPLOSION!

	std::vector<Explosion> normalExplosionVector;
	std::vector<float> timeStartNormalExplosion;

	float timeDiffAnimationNormalExplosion = 0.05f; // Time to switch animation

	sf::Texture normalExplosionTexture; // Variable Texture
	normalExplosionTexture.loadFromFile("Enemy/NormalExplosion.png");
	std::vector<Animation> normalExplosionAnimation;/////////////////////////////

	//MID AIR EXPLOSION!

	std::vector<Explosion> midAirExplosionVector;
	std::vector<float> timeStartMidAirExplosion;

	float timeDiffAnimationMidAirExplosion = 0.05f; // Time to switch animation

	sf::Texture midAirExplosionTexture; // Variable Texture
	midAirExplosionTexture.loadFromFile("Enemy/MidAirExplosion.png");
	std::vector<Animation> midAirExplosionAnimation;/////////////////////////////

	//MID EXPLOSION 2 EXPLOSION!

	std::vector<Explosion> midAir2ExplosionVector;
	std::vector<float> timeStartMidAir2Explosion;

	float timeDiffAnimationMidAir2Explosion = 0.05f; // Time to switch animation

	sf::Texture midAir2ExplosionTexture; // Variable Texture
	midAir2ExplosionTexture.loadFromFile("Enemy/MidAirExplosion.png");
	std::vector<Animation> midAir2ExplosionAnimation;/////////////////////////////

	//SKY BOMB EXPLOSION!

	std::vector<Explosion> skyBombExplosionVector;
	std::vector<float> timeStartSkyBombExplosion;

	float timeDiffAnimationSkyBombExplosion = 0.03f; // Time to switch animation

	sf::Texture SkyBombExplosionTexture; // Variable Texture
	SkyBombExplosionTexture.loadFromFile("Enemy/SkyBombExplosion.png");
	std::vector<Animation> skyBombExplosionAnimation;/////////////////////////////

	//TANK EXPLOSION!

	std::vector<Explosion> tankExplosionVector;
	std::vector<float> timeStartTankExplosion;

	float timeDiffAnimationTankExplosion = 0.08f; // Time to switch animation

	sf::Texture TankExplosionTexture; // Variable Texture
	TankExplosionTexture.loadFromFile("Enemy/NormalExplosion.png");
	std::vector<Animation> tankExplosionAnimation;/////////////////////////////
	bool playerDeadExplosive = false;

	//TANK SHADOW

	sf::Texture tankShadowtex;
	tankShadowtex.loadFromFile("Tank/tankshadow.png");
	Explosion tankShadow(&tankShadowtex, sf::Vector2f(static_cast<float>(player.getPosition().x), static_cast<float>(player.getPosition().y)), 2.0f);



	//Tank Turret
	sf::RectangleShape turret(sf::Vector2f(45.0f, 60.0f)); // CUBE to represent Turret
	//turret.setFillColor(sf::Color::Green); // Set COLOR to cube
	turret.setScale(static_cast<float>(1.3), static_cast<float>(1.3));
	turret.setOrigin(22.5f, 40.0f);
	turret.setPosition(713.0f, 994.0f);
	sf::Texture turretTexture;
	turretTexture.setSmooth(true);
	turretTexture.loadFromFile("Tank/TankTurret.gif");
	turret.setTexture(&turretTexture);


	//Shooting
	sf::Vector2f gunCenter;
	sf::Vector2f aimDir;
	sf::Vector2f aimDirNormalized;
	sf::Vector2f mousePosWindow;

	//Bullet
	Bullet b1;
	int bulletCounter = 0;
	std::vector<Bullet> bullets; // Vector Bullet

		//Fireball
	int fireGun = 0;
	sf::RectangleShape fireball(sf::Vector2f(39.0f, 48.0f));
	fireball.setScale(static_cast<float>(1.3), static_cast<float>(1.3));
	fireball.setOrigin(19.5f, turret.getSize().y + 5);
	fireball.setPosition(gunCenter);
	sf::Texture fireballTexture;
	fireballTexture.loadFromFile("Tank/Fireball.png");
	fireball.setTexture(&fireballTexture);


	for (size_t i = 0; i < bullets.size(); i++) {
		bullets[i].shape.move(bullets[i].currVelocity);
	}

	//Enemy
	std::vector<Enemy> bomber;
	int spawnCounter = 0;
	int PosStartEnemyX = -500;
	int PosStartEnemyY = 200;

	//Enemy
	std::vector<Enemy> jetfighter;
	int spawnCounter2 = 0;

	//Enemy
	std::vector<Enemy> missle;
	int maxMissleOnScreen = 3;
	int missleOnScreen = 0;

	//DUMBBOMB
	std::vector<Enemy> dumbBombVector;
	int spawnDumbBombCounter = 0;
	int spawnDumbBombCounter2 = 0;
	//float timeDiffAnimationDumbBomb = 0.1f;



		//Enemy TEXT
	sf::Text eHpText;
	eHpText.setFont(smallFont);
	eHpText.setCharacterSize(80);
	eHpText.setFillColor(sf::Color::Green);



	//Set Spawn Rate
	int spawnBomberRate = 50; // Enemy BOMBER spawn Rate
	int spawnbulletRate = 12; // Fire Rate
	int maxBomberOnScreen = 15; // Max BOMBER on Screen

	int spawnJetFighterRate = 100; // Enemy JETFIGHTER spawn Rate
	int maxJetFighterOnScreen = 2; // Max JETFIGHTER on Screen

	int spawnDumbBombRate = 20;
	int maxDumbBombOnScreen = 15;

	//Clock Set
	clock.restart();


	//Menu
	Menu menu(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));

	//GAME STATE???????????????????++++<++++++++++++++++++++
	int gameState = 0;
	MainTheme.openFromFile("Music/MainTheme2.ogg");
	MainTheme.play();

	//TEXTBOX1
	bool editNameNow = false;


	textbox1.setFont(smallFont);
	textbox1.setPosition({ 530,570 });
	textbox1.setLimit(true, 9);
	textbox1.textbox.setString(textbox1.getText());

	//LEADERBOARD
	sf::Text leaderboard;
	leaderboard.setFont(smallFont);
	leaderboard.setCharacterSize(130);
	leaderboard.setFillColor(sf::Color::Yellow);
	leaderboard.setPosition(520, 150);
	leaderboard.setString("Top 5 Hero!");

	sf::Text nameList;
	nameList.setFont(smallFont);
	nameList.setCharacterSize(90);
	nameList.setFillColor(sf::Color::White);

	//Diffi
	sf::Text choose;
	choose.setFont(smallFont);
	choose.setCharacterSize(130);
	choose.setFillColor(sf::Color::Yellow);
	choose.setPosition(300, 170);
	choose.setString("Choose Your Difficulty!");

	//Diffi normal
	sf::Text normal;
	normal.setFont(smallFont);
	normal.setCharacterSize(100);
	normal.setFillColor(sf::Color::Red);
	normal.setPosition(220, 370);
	normal.setString("NORMAL");

	sf::Text normalD;
	normalD.setFont(smallFont);
	normalD.setCharacterSize(60);
	normalD.setFillColor(sf::Color::White);
	normalD.setPosition(185, 480);
	normalD.setString("          Press 'A'\nUse Keyboard to\nControl The Tank.\n Small Penalty for\n Escape Aircraft");

	//Diffi hard
	sf::Text hard;
	hard.setFont(smallFont);
	hard.setCharacterSize(100);
	hard.setFillColor(sf::Color::White);
	hard.setPosition(540, 370);
	hard.setString("HARDCORE");

	sf::Text hardD;
	hardD.setFont(smallFont);
	hardD.setCharacterSize(60);
	hardD.setFillColor(sf::Color::White);
	hardD.setPosition(540, 480);
	hardD.setString("          Press 'S'\n   Use Mouse to\nControl The Tank.\nHave Penalty for\n    Every Escape\n          Aircraft");

	//Diffi last stand
	sf::Text lastStand;
	lastStand.setFont(smallFont);
	lastStand.setCharacterSize(100);
	lastStand.setFillColor(sf::Color::White);
	lastStand.setPosition(890, 370);
	lastStand.setString("LAST_STAND");

	sf::Text lastStandD;
	lastStandD.setFont(smallFont);
	lastStandD.setCharacterSize(60);
	lastStandD.setFillColor(sf::Color::White);
	lastStandD.setPosition(915, 480);
	lastStandD.setString("          Press 'D'\n   Use Mouse to\nControl The Tank.\nHeavy Penalty for\n    Every Escape\n          Aircraft\n     Only One Life.");

	//Enter Your Name TEXT
	sf::Text EnterYourName;
	EnterYourName.setFont(smallFont);
	EnterYourName.setCharacterSize(130);
	EnterYourName.setFillColor(sf::Color::Yellow);
	EnterYourName.setPosition(300, 380);
	EnterYourName.setString("Enter Your Name Here!");
	//Click here after Enter Your Name TEXT
	sf::Text clickHere;
	clickHere.setFont(smallFont);
	clickHere.setCharacterSize(50);
	clickHere.setFillColor(sf::Color::Yellow);
	clickHere.setPosition(570, 520);
	clickHere.setString("PRESS 'Esc' TO CONFIRM.");

	//EndGame TEXT
	sf::Text endGame;
	endGame.setFont(smallFont);
	endGame.setCharacterSize(130);
	endGame.setFillColor(sf::Color::Yellow);
	endGame.setPosition(530, 380);
	endGame.setString("Game Over");

	sf::Text clickHere2;
	clickHere2.setFont(smallFont);
	clickHere2.setCharacterSize(50);
	clickHere2.setFillColor(sf::Color::Yellow);
	clickHere2.setPosition(520, 520);
	clickHere2.setString("PRESS 'Esc' TO RETURN TO MANU.");

	sf::Text clickHere3;
	clickHere3.setFont(smallFont);
	clickHere3.setCharacterSize(50);
	clickHere3.setFillColor(sf::Color::Yellow);
	clickHere3.setPosition(520, 280);
	clickHere3.setString("PRESS 'Esc' TO RETURN TO MANU.");

	sf::Text clickHere4;
	clickHere4.setFont(smallFont);
	clickHere4.setCharacterSize(50);
	clickHere4.setFillColor(sf::Color::Yellow);
	clickHere4.setPosition(520, 310);
	clickHere4.setString("PRESS 'Esc' TO RETURN TO MANU.");

	sf::Text endScore;
	endScore.setFont(smallFont);
	endScore.setCharacterSize(100);
	endScore.setFillColor(sf::Color::Green);
	endScore.setPosition(220, 570);

	//ITEM
	sf::Sprite item;
	sf::Texture itemText; // Variable Texture
	itemText.setSmooth(true);
	itemText.loadFromFile("Tank/nuke.png"); // LOAD Texture
	item.setTexture(itemText); // Add Texture
	item.setScale(static_cast<float>(1.3), static_cast<float>(1.3));
	item.setOrigin(item.getGlobalBounds().width / 2, item.getGlobalBounds().height / 2);

	//Nuke icon
	sf::Sprite nukeIcon;
	sf::Texture nukeIconTex; // Variable Texture
	nukeIconTex.setSmooth(true);
	nukeIconTex.loadFromFile("Tank/nukeIcon.png"); // LOAD Texture
	nukeIcon.setTexture(nukeIconTex); // Add Texture
	nukeIcon.setScale(static_cast<float>(2.3), static_cast<float>(2.3));
	nukeIcon.setOrigin(nukeIcon.getGlobalBounds().width / 2, nukeIcon.getGlobalBounds().height / 2);
	nukeIcon.setPosition(1430.0f, 195.0f);

	//GAME OVER
	bool gameOverAlready = false;

	//Item
	bool alreadyDropItem = false;
	int itemOnScreen = 0;
	int itemRand = -1;
	float speedItemPick = 1;

	float itemPickTime = 0;


	bool shieldState = true;

	//REPORT SCORE 
	bool reportScore = true;
	bool callScore = false;

	int x, y, tempScore;
	std::string tempName;
	std::string currentName;
	std::string topName5[6];
	int topScore5[6];

	std::string callName5[6];
	int callScore5[6];

	//Leaderboard
	bool leaderboardState = false;

	//Option
	bool optionState = false;
	bool mouseControl = false;
	factorEscapeEnemy = 100;
	bool last = false;

	//NUKE

	int numberOfNuke = 1;


	// LOOP WINDOW
	while (window.isOpen()) {
		deltaTime = clock.restart().asSeconds(); // Return deltaTime
		srand((unsigned int)time(NULL));

		if (reSet == true) {
			//HOW TO PLAY
			bool howToPlay = false;

			//DEATH
			bool playerAlive = false;
			float deathTime = 0;
			bool alreadyMissleFar = false;
			int counterStartVoice = 0;
			int voiceStartBegin = 10;

			// SOUND GUN

			int soundGunChange = 1;

			//Background
			wallpaper.Wall(howToPlay);

			//Shaking
			int shaking = 0;

			//Random Time
			srand(static_cast<int>(time(NULL)));

			int totalScore = 0;

			player.setPosition(720.0f, 1047.0f);


			bool playerDeadExplosive = false;

			//Tank Turret
			turret.setPosition(713.0f, 994.0f);

			int bulletCounter = 0;

			//Fireball
			int fireGun = 0;

			//Enemy
			int spawnCounter = 0;
			int PosStartEnemyX = -500;
			int PosStartEnemyY = 200;

			//Enemy
			int spawnCounter2 = 0;

			//Enemy
			int maxMissleOnScreen = 3;
			int missleOnScreen = 0;

			//DUMBBOMB
			int spawnDumbBombCounter = 0;
			int spawnDumbBombCounter2 = 0;
			//float timeDiffAnimationDumbBomb = 0.1f;

			//Set Spawn Rate
			int spawnBomberRate = 50; // Enemy BOMBER spawn Rate
			int spawnbulletRate = 12; // Fire Rate
			int maxBomberOnScreen = 15; // Max BOMBER on Screen

			int spawnJetFighterRate = 100; // Enemy JETFIGHTER spawn Rate
			int maxJetFighterOnScreen = 2; // Max JETFIGHTER on Screen

			int spawnDumbBombRate = 20;
			int maxDumbBombOnScreen = 15;

			//Clock Set
			clock.restart();

			//GAME STATE???????????????????++++<++++++++++++++++++++
			int gameState = 0;

			//TEXTBOX1
			bool editNameNow = false;

			for (size_t b = 0; b < bomber.size(); b++) {

				bomber.erase(bomber.begin() + b);//OUT OF SCREEN
				stateBomberHit[b] = false;
				timeBlinkBomber.erase(timeBlinkBomber.begin() + b);
				stateBomberHit.erase(stateBomberHit.begin() + b);

			}

			for (size_t i = 0; i < bullets.size(); i++) {
				bullets.erase(bullets.begin() + i);
			}

			for (size_t c = 0; c < midAirExplosionVector.size(); c++) {

				midAirExplosionVector.erase(midAirExplosionVector.begin() + c);//OUT OF SCREEN
				timeStartMidAirExplosion.erase(timeStartMidAirExplosion.begin() + c);
				midAirExplosionAnimation.erase(midAirExplosionAnimation.begin() + c);



			}

			totalScore = 0;
			//totalReinforcement = 2;

			shieldState = true;
			spawnbulletRate = 12; // Fire Rate

			speedItemPick = 1;


			reSet = false;
		}

		//OPEN WINDOW===============================================================================================================================
		sf::Event evnt;


		while (window.pollEvent(evnt)) {

			if ((evnt.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) && (editNameNow == false)) { // If CLICK 'X' , CLOSE the window
				window.clear();
				window.close();
			}



			if (howToPlay == true) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {

					window.clear();
					howToPlay = false;
					gameState = 0;
					editNameNow = false;
					reSet = true;
					window.setMouseCursorVisible(true);


				}
			}

			if (gameOverAlready == true) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					gameOverAlready = false;
					window.clear();
					howToPlay = false;
					gameState = 0;
					editNameNow = false;
					reSet = true;
					window.setMouseCursorVisible(true);

					//totalReinforcement = 2;
					totalScore = 0;

					MainTheme.stop();
					MainTheme.openFromFile("Music/MainTheme2.ogg");
					MainTheme.play();


				}
			}

			if ((editNameNow == true) && (howToPlay == false)) {


				if (evnt.type == sf::Event::TextEntered) {
					textbox1.typedOn(evnt);
				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					window.clear();
					textbox1.setSelected(false);
					editNameNow = false;
					window.setMouseCursorVisible(true);
					textbox1.textbox.setString(textbox1.getText());

				}
			}

			if ((optionState == true)) {
				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::A))) {

					normal.setFillColor(sf::Color::Red);
					hard.setFillColor(sf::Color::White);
					lastStand.setFillColor(sf::Color::White);

					mouseControl = false;
					factorEscapeEnemy = 100;
					last = false;
					totalReinforcement = 2;
				}
				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::S))) {

					normal.setFillColor(sf::Color::White);
					hard.setFillColor(sf::Color::Red);
					lastStand.setFillColor(sf::Color::White);

					mouseControl = true;
					factorEscapeEnemy = 10;
					last = false;
					totalReinforcement = 2;
				}
				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::D))) {

					normal.setFillColor(sf::Color::White);
					hard.setFillColor(sf::Color::White);
					lastStand.setFillColor(sf::Color::Red);

					mouseControl = true;
					factorEscapeEnemy = 4;
					last = true;
					totalReinforcement = 0;

				}

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					window.clear();
					textbox1.setSelected(false);
					editNameNow = false;
					window.setMouseCursorVisible(true);

					leaderboardState = false;
					textbox1.textbox.setString(textbox1.getText());
					optionState = false;

				}
			}

			if (leaderboardState == true) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					window.clear();
					textbox1.setSelected(false);
					editNameNow = false;
					window.setMouseCursorVisible(true);

					leaderboardState = false;
					textbox1.textbox.setString(textbox1.getText());
					reSet = true;
				}
			}

			if ((editNameNow == false) && (leaderboardState == false) && (optionState == false)) {

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
					window.clear();
					textbox1.setSelected(true);
					editNameNow = true;
					reSet = true;

				}


				if (evnt.type == sf::Event::TextEntered) { // Enter TEXT
					if (evnt.text.unicode < 128) {
						printf("%c", evnt.text.unicode);
					}

				}

				if (evnt.type == sf::Event::MouseMoved) {
					if (menu.isMouseOver(window, 0)) {
						menu.changeItem(0);
					}
					if (menu.isMouseOver(window, 1)) {
						menu.changeItem(1);
					}
					if (menu.isMouseOver(window, 2)) {
						menu.changeItem(2);
					}
					if (menu.isMouseOver(window, 3)) {
						menu.changeItem(3);
					}
					if (menu.isMouseOver(window, 4)) {
						menu.changeItem(4);
					}
					break;
				}

				if (((sf::Mouse::isButtonPressed(sf::Mouse::Left)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))) && (gameState == 0) && (howToPlay == false) && (editNameNow == false)) {

					switch (menu.GetPressedItem()) {
					case 0:
						if (menu.isMouseOver(window, 0) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))) {

							std::cout << "Play" << std::endl;
							reSet = true;
							gameState = 1;
							MainTheme.stop();
							MainTheme.openFromFile("Music/MainTheme.ogg");
							MainTheme.play();

							item.setPosition(-500, -500);
							itemOnScreen = 0;

							if (last == false) {
								totalReinforcement = 2;
							}
							else if (last == true) {
								totalReinforcement = 0;
							}
							totalScore = 0;

							reportScore = false;

							voicePrep.play();

							//itemPickTime = 0;

							TimeMain.restart();
							TimeItem.restart();
							TimeItemDelay.restart();
							playerAlive = true;

							TimeMissleAttack.restart();


							shieldState = true;
							spawnbulletRate = 12; // Fire Rate

							numberOfNuke = 1;

							break;
						}
					case 1:
						if (menu.isMouseOver(window, 1) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))) {
							std::cout << "Option" << std::endl;
							optionState = true;

							bufferVoiceStats.loadFromFile("Sound/stats.ogg");
							voiceStats.setBuffer(bufferVoiceStats);
							voiceStats.play();

							break;
						}
					case 2:
						if (menu.isMouseOver(window, 2) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))) {
							std::cout << "How to play" << std::endl;
							howToPlay = true;
							shieldState = true;
							spawnbulletRate = 12; // Fire Rate

							speedItemPick = 1;
							numberOfNuke = 1;

							gameState = 1;
							wallpaper.Wall(howToPlay);


							playerAlive = true;


							if (howToPlay == true) {
								player.setPosition(720.0f, 950.0f);
								turret.setPosition(713.0f, 897.0f);
								window.draw(howTo);
							}
							break;
						}
					case 3:
						if (menu.isMouseOver(window, 3) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))) {
							std::cout << "Leaderboard" << std::endl;
							leaderboardState = true;

							bufferStar.loadFromFile("Sound/star.ogg");
							star.setBuffer(bufferStar);
							star.play();

							break;
						}
					case 4:
						if (menu.isMouseOver(window, 4) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))) {
							std::cout << "Exit" << std::endl;

							window.close();
							break;
						}
					}
				}


				if (((evnt.type == sf::Event::KeyReleased)) && (gameState == 0) && (leaderboardState == false)) {
					switch (evnt.key.code) {

					case sf::Keyboard::W:
						menu.MoveUp();
						selectedMenuUp.play();
						break;
					case sf::Keyboard::Up:
						menu.MoveUp();
						selectedMenuUp.play();
						break;

					case sf::Keyboard::S:
						menu.MoveDown();
						selectedMenuDown.play();
						break;
					case sf::Keyboard::Down:
						menu.MoveDown();
						selectedMenuDown.play();
						break;




					}
				}
			}



		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && (playerAlive == true) && numberOfNuke <= 0) {
			bufferNo.loadFromFile("Sound/denied.ogg");
			no.setBuffer(bufferNo);
			no.play();
		}

		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && (playerAlive == true) && numberOfNuke > 0) {//RIGHT CLICK NUKE


			bufferFall.loadFromFile("Sound/nukeblast.ogg");
			fall.setBuffer(bufferFall);
			fall.play();


			for (size_t k = 0; k < bomber.size(); k++) {

				midAirExplosionVector.push_back(Explosion(&midAirExplosionTexture, sf::Vector2f(static_cast<float>(bomber[k].shape.getPosition().x + 100), static_cast<float>(bomber[k].shape.getPosition().y - 150)), 1.3f));
				midAirExplosionAnimation.push_back(Animation(&midAirExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAirExplosion));
				timeStartMidAirExplosion.push_back(timeSinceStart);


				bomber.erase(bomber.begin() + k);//BOMBER DES


				stateBomberHit[k] = false;
				timeBlinkBomber.erase(timeBlinkBomber.begin() + k);
				stateBomberHit.erase(stateBomberHit.begin() + k);

				totalScore = totalScore + bomberPoint;

			}
			for (size_t a = 0; a < dumbBombVector.size(); a++) {



				skyBombExplosionVector.push_back(Explosion(&SkyBombExplosionTexture, sf::Vector2f(dumbBombVector[a].shape.getPosition().x, dumbBombVector[a].shape.getPosition().y), 0.75));
				skyBombExplosionAnimation.push_back(Animation(&SkyBombExplosionTexture, sf::Vector2u(32, 1), timeDiffAnimationSkyBombExplosion));
				timeStartSkyBombExplosion.push_back(timeSinceStart);


				dumbBombVector.erase(dumbBombVector.begin() + a);//DUMD BOMB DES

				totalScore = totalScore + dumbBombPoint;

			}
			for (size_t d = 0; d < jetfighter.size(); d++) {

				midAir2ExplosionVector.push_back(Explosion(&midAir2ExplosionTexture, sf::Vector2f(jetfighter[d].shape.getPosition().x + 80, jetfighter[d].shape.getPosition().y - 80), 1));
				midAir2ExplosionAnimation.push_back(Animation(&midAir2ExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAir2Explosion));
				timeStartMidAir2Explosion.push_back(timeSinceStart);


				jetfighter.erase(jetfighter.begin() + d);//JET DES


				stateJetFighterHit[d] = false;
				timeBlinkJetFighter.erase(timeBlinkJetFighter.begin() + d);
				stateJetFighterHit.erase(stateJetFighterHit.begin() + d);

				totalScore = totalScore + dumbBombPoint;
			}
			for (size_t d = 0; d < missle.size(); d++) {


				midAir2ExplosionVector.push_back(Explosion(&midAir2ExplosionTexture, sf::Vector2f(missle[d].shape.getPosition().x + 10, missle[d].shape.getPosition().y - 50), 2));
				midAir2ExplosionAnimation.push_back(Animation(&midAir2ExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAir2Explosion));
				timeStartMidAir2Explosion.push_back(timeSinceStart);


				missle.erase(missle.begin() + d);//MISSLE DES

				totalScore = totalScore + misslePoint;

				stateMissleHit[d] = false;
				timeBlinkMissle.erase(timeBlinkMissle.begin() + d);
				stateMissleHit.erase(stateMissleHit.begin() + d);

				TimeMissleAttack.restart();
				alreadyMissleFar = false;


				missleOnScreen = missleOnScreen - 1;


			}

			for (size_t b = 0; b < friendlyVector.size(); b++) {


				midAirExplosionVector.push_back(Explosion(&midAirExplosionTexture, sf::Vector2f(static_cast<float>(friendlyVector[b].shape.getPosition().x + 50), static_cast<float>(friendlyVector[b].shape.getPosition().y - 100)), 1.3f));
				midAirExplosionAnimation.push_back(Animation(&midAirExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAirExplosion));
				timeStartMidAirExplosion.push_back(timeSinceStart);

				voiceCopter.stop();

				alreadyDropItem = false;

				friendlyVector.erase(friendlyVector.begin() + b);//BOMBER DES

				totalScore = totalScore + friendlyPoint;

				stateFriendlyHit[b] = false;
				timeBlinkFriendly.erase(timeBlinkFriendly.begin() + b);
				stateFriendlyHit.erase(stateFriendlyHit.begin() + b);


				airSupportNow = false;



				friendlyCallOut1 = true;
				friendlyCallOut2 = true;
				friendlyCallOut3 = true;



			}

			for (size_t k = 0; k < bomber.size(); k++) {

				midAirExplosionVector.push_back(Explosion(&midAirExplosionTexture, sf::Vector2f(static_cast<float>(bomber[k].shape.getPosition().x + 100), static_cast<float>(bomber[k].shape.getPosition().y - 150)), 1.3f));
				midAirExplosionAnimation.push_back(Animation(&midAirExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAirExplosion));
				timeStartMidAirExplosion.push_back(timeSinceStart);


				bomber.erase(bomber.begin() + k);//BOMBER DES


				stateBomberHit[k] = false;
				timeBlinkBomber.erase(timeBlinkBomber.begin() + k);
				stateBomberHit.erase(stateBomberHit.begin() + k);

				totalScore = totalScore + bomberPoint;

			}
			for (size_t a = 0; a < dumbBombVector.size(); a++) {



				skyBombExplosionVector.push_back(Explosion(&SkyBombExplosionTexture, sf::Vector2f(dumbBombVector[a].shape.getPosition().x, dumbBombVector[a].shape.getPosition().y), 0.75));
				skyBombExplosionAnimation.push_back(Animation(&SkyBombExplosionTexture, sf::Vector2u(32, 1), timeDiffAnimationSkyBombExplosion));
				timeStartSkyBombExplosion.push_back(timeSinceStart);


				dumbBombVector.erase(dumbBombVector.begin() + a);//DUMD BOMB DES

				totalScore = totalScore + dumbBombPoint;

			}
			for (size_t d = 0; d < jetfighter.size(); d++) {

				midAir2ExplosionVector.push_back(Explosion(&midAir2ExplosionTexture, sf::Vector2f(jetfighter[d].shape.getPosition().x + 80, jetfighter[d].shape.getPosition().y - 80), 1));
				midAir2ExplosionAnimation.push_back(Animation(&midAir2ExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAir2Explosion));
				timeStartMidAir2Explosion.push_back(timeSinceStart);


				jetfighter.erase(jetfighter.begin() + d);//JET DES


				stateJetFighterHit[d] = false;
				timeBlinkJetFighter.erase(timeBlinkJetFighter.begin() + d);
				stateJetFighterHit.erase(stateJetFighterHit.begin() + d);

				totalScore = totalScore + dumbBombPoint;
			}
			for (size_t d = 0; d < missle.size(); d++) {


				midAir2ExplosionVector.push_back(Explosion(&midAir2ExplosionTexture, sf::Vector2f(missle[d].shape.getPosition().x + 10, missle[d].shape.getPosition().y - 50), 2));
				midAir2ExplosionAnimation.push_back(Animation(&midAir2ExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAir2Explosion));
				timeStartMidAir2Explosion.push_back(timeSinceStart);


				missle.erase(missle.begin() + d);//MISSLE DES

				totalScore = totalScore + misslePoint;

				stateMissleHit[d] = false;
				timeBlinkMissle.erase(timeBlinkMissle.begin() + d);
				stateMissleHit.erase(stateMissleHit.begin() + d);

				TimeMissleAttack.restart();
				alreadyMissleFar = false;


				missleOnScreen = missleOnScreen - 1;


			}

			for (size_t b = 0; b < friendlyVector.size(); b++) {


				midAirExplosionVector.push_back(Explosion(&midAirExplosionTexture, sf::Vector2f(static_cast<float>(friendlyVector[b].shape.getPosition().x + 50), static_cast<float>(friendlyVector[b].shape.getPosition().y - 100)), 1.3f));
				midAirExplosionAnimation.push_back(Animation(&midAirExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAirExplosion));
				timeStartMidAirExplosion.push_back(timeSinceStart);

				voiceCopter.stop();

				alreadyDropItem = false;

				friendlyVector.erase(friendlyVector.begin() + b);//BOMBER DES

				totalScore = totalScore + friendlyPoint;

				stateFriendlyHit[b] = false;
				timeBlinkFriendly.erase(timeBlinkFriendly.begin() + b);
				stateFriendlyHit.erase(stateFriendlyHit.begin() + b);


				airSupportNow = false;



				friendlyCallOut1 = true;
				friendlyCallOut2 = true;
				friendlyCallOut3 = true;



			}

			for (size_t k = 0; k < bomber.size(); k++) {

				midAirExplosionVector.push_back(Explosion(&midAirExplosionTexture, sf::Vector2f(static_cast<float>(bomber[k].shape.getPosition().x + 100), static_cast<float>(bomber[k].shape.getPosition().y - 150)), 1.3f));
				midAirExplosionAnimation.push_back(Animation(&midAirExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAirExplosion));
				timeStartMidAirExplosion.push_back(timeSinceStart);


				bomber.erase(bomber.begin() + k);//BOMBER DES


				stateBomberHit[k] = false;
				timeBlinkBomber.erase(timeBlinkBomber.begin() + k);
				stateBomberHit.erase(stateBomberHit.begin() + k);

				totalScore = totalScore + bomberPoint;

			}
			for (size_t a = 0; a < dumbBombVector.size(); a++) {



				skyBombExplosionVector.push_back(Explosion(&SkyBombExplosionTexture, sf::Vector2f(dumbBombVector[a].shape.getPosition().x, dumbBombVector[a].shape.getPosition().y), 0.75));
				skyBombExplosionAnimation.push_back(Animation(&SkyBombExplosionTexture, sf::Vector2u(32, 1), timeDiffAnimationSkyBombExplosion));
				timeStartSkyBombExplosion.push_back(timeSinceStart);


				dumbBombVector.erase(dumbBombVector.begin() + a);//DUMD BOMB DES

				totalScore = totalScore + dumbBombPoint;

			}
			for (size_t d = 0; d < jetfighter.size(); d++) {

				midAir2ExplosionVector.push_back(Explosion(&midAir2ExplosionTexture, sf::Vector2f(jetfighter[d].shape.getPosition().x + 80, jetfighter[d].shape.getPosition().y - 80), 1));
				midAir2ExplosionAnimation.push_back(Animation(&midAir2ExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAir2Explosion));
				timeStartMidAir2Explosion.push_back(timeSinceStart);


				jetfighter.erase(jetfighter.begin() + d);//JET DES


				stateJetFighterHit[d] = false;
				timeBlinkJetFighter.erase(timeBlinkJetFighter.begin() + d);
				stateJetFighterHit.erase(stateJetFighterHit.begin() + d);

				totalScore = totalScore + dumbBombPoint;
			}
			for (size_t d = 0; d < missle.size(); d++) {


				midAir2ExplosionVector.push_back(Explosion(&midAir2ExplosionTexture, sf::Vector2f(missle[d].shape.getPosition().x + 10, missle[d].shape.getPosition().y - 50), 2));
				midAir2ExplosionAnimation.push_back(Animation(&midAir2ExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAir2Explosion));
				timeStartMidAir2Explosion.push_back(timeSinceStart);


				missle.erase(missle.begin() + d);//MISSLE DES

				totalScore = totalScore + misslePoint;

				stateMissleHit[d] = false;
				timeBlinkMissle.erase(timeBlinkMissle.begin() + d);
				stateMissleHit.erase(stateMissleHit.begin() + d);

				TimeMissleAttack.restart();
				alreadyMissleFar = false;


				missleOnScreen = missleOnScreen - 1;


			}

			for (size_t b = 0; b < friendlyVector.size(); b++) {


				midAirExplosionVector.push_back(Explosion(&midAirExplosionTexture, sf::Vector2f(static_cast<float>(friendlyVector[b].shape.getPosition().x + 50), static_cast<float>(friendlyVector[b].shape.getPosition().y - 100)), 1.3f));
				midAirExplosionAnimation.push_back(Animation(&midAirExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAirExplosion));
				timeStartMidAirExplosion.push_back(timeSinceStart);

				voiceCopter.stop();

				alreadyDropItem = false;

				friendlyVector.erase(friendlyVector.begin() + b);//BOMBER DES

				totalScore = totalScore + friendlyPoint;

				stateFriendlyHit[b] = false;
				timeBlinkFriendly.erase(timeBlinkFriendly.begin() + b);
				stateFriendlyHit.erase(stateFriendlyHit.begin() + b);


				airSupportNow = false;



				friendlyCallOut1 = true;
				friendlyCallOut2 = true;
				friendlyCallOut3 = true;



			}



			for (size_t k = 0; k < bomber.size(); k++) {

				midAirExplosionVector.push_back(Explosion(&midAirExplosionTexture, sf::Vector2f(static_cast<float>(bomber[k].shape.getPosition().x + 100), static_cast<float>(bomber[k].shape.getPosition().y - 150)), 1.3f));
				midAirExplosionAnimation.push_back(Animation(&midAirExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAirExplosion));
				timeStartMidAirExplosion.push_back(timeSinceStart);


				bomber.erase(bomber.begin() + k);//BOMBER DES


				stateBomberHit[k] = false;
				timeBlinkBomber.erase(timeBlinkBomber.begin() + k);
				stateBomberHit.erase(stateBomberHit.begin() + k);

				totalScore = totalScore + bomberPoint;

			}
			for (size_t a = 0; a < dumbBombVector.size(); a++) {



				skyBombExplosionVector.push_back(Explosion(&SkyBombExplosionTexture, sf::Vector2f(dumbBombVector[a].shape.getPosition().x, dumbBombVector[a].shape.getPosition().y), 0.75));
				skyBombExplosionAnimation.push_back(Animation(&SkyBombExplosionTexture, sf::Vector2u(32, 1), timeDiffAnimationSkyBombExplosion));
				timeStartSkyBombExplosion.push_back(timeSinceStart);


				dumbBombVector.erase(dumbBombVector.begin() + a);//DUMD BOMB DES

				totalScore = totalScore + dumbBombPoint;

			}
			for (size_t d = 0; d < jetfighter.size(); d++) {

				midAir2ExplosionVector.push_back(Explosion(&midAir2ExplosionTexture, sf::Vector2f(jetfighter[d].shape.getPosition().x + 80, jetfighter[d].shape.getPosition().y - 80), 1));
				midAir2ExplosionAnimation.push_back(Animation(&midAir2ExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAir2Explosion));
				timeStartMidAir2Explosion.push_back(timeSinceStart);


				jetfighter.erase(jetfighter.begin() + d);//JET DES


				stateJetFighterHit[d] = false;
				timeBlinkJetFighter.erase(timeBlinkJetFighter.begin() + d);
				stateJetFighterHit.erase(stateJetFighterHit.begin() + d);

				totalScore = totalScore + dumbBombPoint;
			}
			for (size_t d = 0; d < missle.size(); d++) {


				midAir2ExplosionVector.push_back(Explosion(&midAir2ExplosionTexture, sf::Vector2f(missle[d].shape.getPosition().x + 10, missle[d].shape.getPosition().y - 50), 2));
				midAir2ExplosionAnimation.push_back(Animation(&midAir2ExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAir2Explosion));
				timeStartMidAir2Explosion.push_back(timeSinceStart);


				missle.erase(missle.begin() + d);//MISSLE DES

				totalScore = totalScore + misslePoint;

				stateMissleHit[d] = false;
				timeBlinkMissle.erase(timeBlinkMissle.begin() + d);
				stateMissleHit.erase(stateMissleHit.begin() + d);

				TimeMissleAttack.restart();
				alreadyMissleFar = false;


				missleOnScreen = missleOnScreen - 1;


			}

			for (size_t b = 0; b < friendlyVector.size(); b++) {


				midAirExplosionVector.push_back(Explosion(&midAirExplosionTexture, sf::Vector2f(static_cast<float>(friendlyVector[b].shape.getPosition().x + 50), static_cast<float>(friendlyVector[b].shape.getPosition().y - 100)), 1.3f));
				midAirExplosionAnimation.push_back(Animation(&midAirExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAirExplosion));
				timeStartMidAirExplosion.push_back(timeSinceStart);

				voiceCopter.stop();

				alreadyDropItem = false;

				friendlyVector.erase(friendlyVector.begin() + b);//BOMBER DES

				totalScore = totalScore + friendlyPoint;

				stateFriendlyHit[b] = false;
				timeBlinkFriendly.erase(timeBlinkFriendly.begin() + b);
				stateFriendlyHit.erase(stateFriendlyHit.begin() + b);


				airSupportNow = false;



				friendlyCallOut1 = true;
				friendlyCallOut2 = true;
				friendlyCallOut3 = true;



			}

			numberOfNuke = numberOfNuke - 1;


		}



		if (gameState == 0) { ///////////////MENU///////////////////////////><><>
			timeSinceStart = TimeMain.getElapsedTime().asSeconds();
			//std::cout << timeSinceStart << std::endl;
			window.clear();
			window.draw(menuWall);
			menu.drawMenu(window, textbox1.textbox.getString());

			if (editNameNow == true) {
				window.draw(blank);
				window.draw(EnterYourName);
				window.draw(clickHere);
				textbox1.drawTo(window);


			}

			if (callScore == false) {

				std::ifstream bufferCallInPut("Score/Score.txt");
				for (int z = 0; z < 5; z++) {
					if (bufferCallInPut.is_open()) {
						bufferCallInPut >> callName5[z] >> callScore5[z];
					}
				}

				bufferCallInPut.close();

				callScore = true;

			}

			if ((leaderboardState == true)) {
				callScore = false;
				window.draw(blank);
				window.draw(leaderboard);

				nameList.setString("1. " + callName5[0] + " : " + std::to_string(callScore5[0]) + "\n" + "2. " + callName5[1] + " : " + std::to_string(callScore5[1]) + "\n" + "3. " + callName5[2] + " : " + std::to_string(callScore5[2]) + "\n" + "4. " + callName5[3] + " : " + std::to_string(callScore5[3]) + "\n" + "5. " + callName5[4] + " : " + std::to_string(callScore5[4]) + "\n");
				nameList.setOrigin(nameList.getGlobalBounds().width / 2, nameList.getGlobalBounds().height / 2);
				nameList.setPosition(720, 650);
				window.draw(nameList);
				window.draw(clickHere3);

			}

			if (optionState == true) {
				window.draw(blank);
				window.draw(choose);
				window.draw(clickHere4);
				window.draw(normal);
				window.draw(hard);
				window.draw(lastStand);
				window.draw(normalD);
				window.draw(hardD);
				window.draw(lastStandD);
			}


		}



		if (gameState == 1) { //////START GAME!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!




			timeSinceStart = TimeMain.getElapsedTime().asSeconds();
			timeSinceSupport = TimeItem.getElapsedTime().asSeconds();
			//std::cout << timeSinceStart << std::endl;
			//std::cout << timeSinceSupport << std::endl;

			timeMissleAttack = TimeMissleAttack.getElapsedTime().asSeconds();

			//Spawn RANDOM======================================================================================================================================


			///////////////DIFFICULTY/////////////////////////
			if (timeSinceStart < 35) {

				//Set Spawn Rate
				int maxBomberOnScreen = 2; // Max BOMBER on Screen

				int maxJetFighterOnScreen = 2; // Max JETFIGHTER on Screen

				int maxDumbBombOnScreen = 3;

				spawnBomberRate = (rand() % 500 + 100);

				spawnDumbBombRate = (rand() % 50 + 20);

				spawnJetFighterRate = (rand() % 300 + 100);
			}
			else if ((timeSinceStart >= 35) && (timeSinceStart < 70)) {
				//Set Spawn Rate
				int maxBomberOnScreen = 4; // Max BOMBER on Screen

				int maxJetFighterOnScreen = 3; // Max JETFIGHTER on Screen

				int maxDumbBombOnScreen = 6;

				spawnBomberRate = (rand() % 350 + 100);

				spawnDumbBombRate = (rand() % 30 + 20);

				spawnJetFighterRate = (rand() % 150 + 100);
			}
			else if ((timeSinceStart >= 70) && (timeSinceStart < 100)) {
				//Set Spawn Rate
				int maxBomberOnScreen = 8; // Max BOMBER on Screen

				int maxJetFighterOnScreen = 5; // Max JETFIGHTER on Screen

				int maxDumbBombOnScreen = 12;

				spawnBomberRate = (rand() % 150 + 100);

				spawnDumbBombRate = (rand() % 15 + 20);

				spawnJetFighterRate = (rand() % 100 + 100);
			}
			else if ((timeSinceStart >= 100) && (timeSinceStart < 130)) {
				//Set Spawn Rate
				int maxBomberOnScreen = 12; // Max BOMBER on Screen

				int maxJetFighterOnScreen = 8; // Max JETFIGHTER on Screen

				int maxDumbBombOnScreen = 15;

				spawnBomberRate = (rand() % 70 + 100);

				spawnDumbBombRate = (rand() % 5 + 20);

				spawnJetFighterRate = (rand() % 50 + 100);
			}
			else if ((timeSinceStart >= 130)) {
				//Set Spawn Rate
				int maxBomberOnScreen = 20; // Max BOMBER on Screen

				int maxJetFighterOnScreen = 15; // Max JETFIGHTER on Screen

				int maxDumbBombOnScreen = 30;

				spawnBomberRate = (rand() % 70 + 50);

				spawnDumbBombRate = (rand() % 5 + 13);

				spawnJetFighterRate = (rand() % 50 + 50);
			}



			tankPlayer.MoveTank(player, turret, howToPlay, speedItemPick, mouseControl); // Object of TankControl CLASS , Call MoveTank Function
			tankPlayer.AimGun(player, window, turret, fireball); // Object of TankControl CLASS , Call ClickLeft Function


			//Update Shoot=======================================================================================================================================

			gunCenter = sf::Vector2f(turret.getPosition().x - 6.0f, turret.getPosition().y - 5.0f);
			mousePosWindow = sf::Vector2f(sf::Mouse::getPosition(window));
			aimDir = mousePosWindow - gunCenter; // Aim direction
			aimDirNormalized = aimDir / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
			// CHEAT CODE OF THIS EQUATION OF NORMALIZED VECTOR
			/*
			LENGTH OF VECTOR: |V| = sqrt(V.x^2)+ V.y^2)
			NORMAILIZED VECTOR U = V / |V|
			*/
			//std::cout << "AimDirNormalized X : " << aimDirNormalized.x << " AimDirNormalized Y : " << aimDirNormalized.y << std::endl;





			//Shoot Function=======================================================================================================================================

			if (bulletCounter < spawnbulletRate) {
				bulletCounter++;

			}

			sf::Vector2i mousePos = sf::Mouse::getPosition(window);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mousePos.y <= 850.0f && bulletCounter >= spawnbulletRate && (playerAlive == true)) { // LEFT CLICK

				// IF PICK ITEM SOUND GUN WILL CHANGE

				if (soundGunChange == 1) {
					bufferGunSound.loadFromFile("Sound/tankfire1.ogg");
				}
				else if (soundGunChange == 2) {
					bufferGunSound.loadFromFile("Sound/tankfire2.ogg");
				}
				else if (soundGunChange == 3) {
					bufferGunSound.loadFromFile("Sound/tankfire3.ogg");
				}
				else if (soundGunChange == 4) {
					bufferGunSound.loadFromFile("Sound/tankfire4.ogg");
				}

				soundGun.setBuffer(bufferGunSound); //SET BUFFER
				soundGun.play(); //PLAY SOUND



				//FireBall

				fireGun = 1;

				shaking = 1;
				b1.shape.setPosition(gunCenter);
				b1.currVelocity = aimDirNormalized * b1.maxSpeed;

				bullets.push_back(Bullet(b1));
				bulletCounter = 0;
			}




			if (fireGun > 0) {
				fireballTexture.loadFromFile("Tank/Fireball.png");
				fireball.setPosition(turret.getPosition().x, turret.getPosition().y);
				fireGun = fireGun + 1;
				if (fireGun >= 7) {
					//printf("RemoveFireBall\n");
					fireballTexture.loadFromFile("Tank/Fireball2.png");
					fireGun = 0;
				}
			}



			//Enemy Bomber Update=======================================================================================================================================

			if ((spawnCounter < spawnBomberRate) && (howToPlay == false)) {
				spawnCounter++;
			}
			if (spawnCounter >= spawnBomberRate && bomber.size() < maxBomberOnScreen && (playerAlive == true) && (howToPlay == false)) {
				bomber.push_back(Enemy(&EnemyBomberTex, sf::Vector2f(static_cast<float>(PosStartEnemyX), static_cast<float>(rand() % 400 + PosStartEnemyY)), 20));
				spawnCounter = 0;//RESET TIMER

				///////////////Blink BOMBER INTNI
				timeBlinkBomber.push_back(0);
				stateBomberHit.push_back(false);

			}

			for (size_t b = 0; b < bomber.size(); b++) {
				if ((playerAlive == true)) {
					bomber[b].shape.move(5.0f, 0.0f);
				}
				else {
					bomber[b].shape.move(15.0f, 0.0f);
				}


				if (spawnDumbBombCounter2 >= spawnDumbBombRate && dumbBombVector.size() < maxDumbBombOnScreen && (playerAlive == true)) {
					for (size_t a = 0; a < bomber.size(); a++) {
						if (((player.getPosition().x) - (bomber[a].shape.getPosition().x) >= 400) && ((player.getPosition().x) - (bomber[a].shape.getPosition().x) <= 700)) {
							dumbBombVector.push_back(Enemy(&DumbBombTex, sf::Vector2f((bomber[a].shape.getPosition().x + 100), (bomber[a].shape.getPosition().y + 20)), 5));
							spawnDumbBombCounter2 = 0;//RESET TIMER
							bufferBombFall.loadFromFile("Sound/bombfall.ogg");
							soundBombFall.setBuffer(bufferBombFall); //SET BUFFER
							soundBombFall.play(); //PLAY SOUND
						}
					}

				}



				if (bomber[b].shape.getPosition().x > 1500) {
					bomber.erase(bomber.begin() + b);//OUT OF SCREEN
					totalScore = totalScore - (bomberPoint / factorEscapeEnemy);

					stateBomberHit[b] = false;
					timeBlinkBomber.erase(timeBlinkBomber.begin() + b);
					stateBomberHit.erase(stateBomberHit.begin() + b);
				}


			}


			//Enemy JetFighter Update=======================================================================================================================================

			if ((spawnCounter2 < spawnJetFighterRate) && (howToPlay == false)) {
				spawnCounter2++;
			}
			if (spawnCounter2 >= spawnJetFighterRate && jetfighter.size() < maxJetFighterOnScreen && (playerAlive == true) && (howToPlay == false)) {
				jetfighter.push_back(Enemy(&EnemyJetFighterTex, sf::Vector2f(static_cast<float>(PosStartEnemyX), static_cast<float>(rand() % 400 + PosStartEnemyY)), 10));
				spawnCounter2 = 0;//RESET TIMER

				///////////////Blink JET FIGHTER INTNI
				timeBlinkJetFighter.push_back(0);
				stateJetFighterHit.push_back(false);

			}

			for (size_t b = 0; b < jetfighter.size(); b++) {
				if ((playerAlive == true)) {
					jetfighter[b].shape.move(9.0f, 0.0f);
				}
				else {
					jetfighter[b].shape.move(15.0f, 0.0f);
				}

				if (spawnDumbBombCounter >= spawnDumbBombRate && dumbBombVector.size() < maxDumbBombOnScreen && (playerAlive == true)) {
					for (size_t a = 0; a < jetfighter.size(); a++) {
						if (((player.getPosition().x) - (jetfighter[a].shape.getPosition().x) >= 400) && ((player.getPosition().x) - (jetfighter[a].shape.getPosition().x) <= 700)) {
							dumbBombVector.push_back(Enemy(&DumbBombTex, sf::Vector2f((jetfighter[a].shape.getPosition().x + 100), (jetfighter[a].shape.getPosition().y + 20)), 5));
							spawnDumbBombCounter = 0;//RESET TIMER
							bufferBombFall.loadFromFile("Sound/bombfall.ogg");
							soundBombFall.setBuffer(bufferBombFall); //SET BUFFER
							soundBombFall.play(); //PLAY SOUND
						}
					}

				}

				if (jetfighter[b].shape.getPosition().x > 1500) {
					jetfighter.erase(jetfighter.begin() + b);//OUT OF SCREEN
					totalScore = totalScore - (jetFighterPoint / factorEscapeEnemy);

					stateJetFighterHit[b] = false;
					timeBlinkJetFighter.erase(timeBlinkJetFighter.begin() + b);
					stateJetFighterHit.erase(stateJetFighterHit.begin() + b);
				}

			}

			///////MISSLE!!!!!!!!!!!!!!!!!!!!!!!!!!!!

			//Missle Update=======================================================================================================================================

			for (size_t b = 0; b < missle.size(); b++) {
				if ((playerAlive == true)) {
					missle[b].shape.move(0.0f, 8.0f);
				}
				else {
					missle[b].shape.move(0.0f, 15.0f);
				}


				if (missle[b].shape.getPosition().y >= 950) {

					normalExplosionVector.push_back(Explosion(&normalExplosionTexture, sf::Vector2f(missle[b].shape.getPosition().x - 200, missle[b].shape.getPosition().y - 660), 3));
					normalExplosionAnimation.push_back(Animation(&normalExplosionTexture, sf::Vector2u(25, 1), timeDiffAnimationNormalExplosion));



					bufferMissleExplosion.loadFromFile("Sound/bossblast.ogg");
					voiceMissleExplosion.setBuffer(bufferMissleExplosion);
					voiceMissleExplosion.play();

					timeStartNormalExplosion.push_back(timeSinceStart);

					missle.erase(missle.begin() + b);//OUT OF SCREEN
					missleNearAnimation.erase(missleNearAnimation.begin() + b);


					stateMissleHit[b] = false;
					timeBlinkMissle.erase(timeBlinkMissle.begin() + b);
					stateMissleHit.erase(stateMissleHit.begin() + b);

					TimeMissleAttack.restart();
					alreadyMissleFar = false;

					missleOnScreen = missleOnScreen - 1;

					break;
				}

				if (missle[b].shape.getGlobalBounds().intersects(player.getGlobalBounds()) && (playerAlive == true)) {//MISSLE HIT PLAYER!

					if (shieldState == false) {
						bufferVoiceTankExplosion.loadFromFile("Sound/tankexplode.ogg");
						voiceTankExplosion.setBuffer(bufferVoiceTankExplosion);
						voiceTankExplosion.play();

						tankExplosionVector.push_back(Explosion(&TankExplosionTexture, sf::Vector2f(player.getPosition().x - 150, player.getPosition().y - 500), 2));
						tankExplosionAnimation.push_back(Animation(&TankExplosionTexture, sf::Vector2u(25, 1), timeDiffAnimationTankExplosion));
						timeStartTankExplosion.push_back(timeSinceStart);
						playerDeadExplosive = true;


						playerAlive = false;
						missle.erase(missle.begin() + b);//OUT OF SCREEN
						missleNearAnimation.erase(missleNearAnimation.begin() + b);


						stateMissleHit[b] = false;
						timeBlinkMissle.erase(timeBlinkMissle.begin() + b);
						stateMissleHit.erase(stateMissleHit.begin() + b);

						bufferBigExplode.loadFromFile("Sound/bigexplode.ogg");
						soundBigExplode.setBuffer(bufferBigExplode); //SET BUFFER
						soundBigExplode.play(); //PLAY SOUND

						deathTime = timeSinceStart;

						TimeMissleAttack.restart();
						alreadyMissleFar = false;

						missleOnScreen = missleOnScreen - 1;

						break;
					}

					else if (shieldState == true) {
						shieldState = false;

						midAir2ExplosionVector.push_back(Explosion(&midAir2ExplosionTexture, sf::Vector2f(missle[b].shape.getPosition().x + 10, missle[b].shape.getPosition().y - 50), 2));
						midAir2ExplosionAnimation.push_back(Animation(&midAir2ExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAir2Explosion));
						timeStartMidAir2Explosion.push_back(timeSinceStart);


						missle.erase(missle.begin() + b);//MISSLE DES
						bufferBigExplode.loadFromFile("Sound/bigexplode.ogg");
						soundBigExplode.setBuffer(bufferBigExplode); //SET BUFFER
						soundBigExplode.play();
						totalScore = totalScore + misslePoint;

						stateMissleHit[b] = false;
						timeBlinkMissle.erase(timeBlinkMissle.begin() + b);
						stateMissleHit.erase(stateMissleHit.begin() + b);

						TimeMissleAttack.restart();
						alreadyMissleFar = false;


						missleOnScreen = missleOnScreen - 1;


						bufferLostShield.loadFromFile("Sound/shielddown.ogg");
						lostShield.setBuffer(bufferLostShield);
						lostShield.play();
					}


				}

			}




			for (size_t i = 0; i < bullets.size(); i++) {
				for (size_t d = 0; d < missle.size(); d++) {
					if (bullets[i].shape.getGlobalBounds().intersects(missle[d].shape.getGlobalBounds())) {
						bufferBulletHit.loadFromFile("Sound/bullethit.ogg");
						soundBulletHit.setBuffer(bufferBulletHit); //SET BUFFER
						soundBulletHit.play();
						missle[d].HP = missle[d].HP - 5;

						if (stateMissleHit[d] == false) {
							stateMissleHit[d] = true;
							timeBlinkMissle[d] = timeSinceStart; //RED BLINK!!!!
						}

						if (missle[d].HP <= 0) {

							midAir2ExplosionVector.push_back(Explosion(&midAir2ExplosionTexture, sf::Vector2f(missle[d].shape.getPosition().x + 10, missle[d].shape.getPosition().y - 50), 2));
							midAir2ExplosionAnimation.push_back(Animation(&midAir2ExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAir2Explosion));
							timeStartMidAir2Explosion.push_back(timeSinceStart);


							missle.erase(missle.begin() + d);//MISSLE DES
							bufferBigExplode.loadFromFile("Sound/bigexplode.ogg");
							soundBigExplode.setBuffer(bufferBigExplode); //SET BUFFER
							soundBigExplode.play();
							totalScore = totalScore + misslePoint;

							stateMissleHit[d] = false;
							timeBlinkMissle.erase(timeBlinkMissle.begin() + d);
							stateMissleHit.erase(stateMissleHit.begin() + d);

							TimeMissleAttack.restart();
							alreadyMissleFar = false;


							missleOnScreen = missleOnScreen - 1;


						}
						bullets.erase(bullets.begin() + i);
						break;
					}


				}
			}





			//Dumb Bomb Update=======================================================================================================================================

			if ((spawnDumbBombCounter < spawnDumbBombRate) && (howToPlay == false)) {
				spawnDumbBombCounter++;

			}
			if ((spawnDumbBombCounter2 < spawnDumbBombRate) && (howToPlay == false)) {
				spawnDumbBombCounter2++;

			}


			for (size_t d = 0; d < dumbBombVector.size(); d++) {
				dumbBombVector[d].shape.rotate(0.75f);
				dumbBombVector[d].shape.move(7.0f, 7.0f);

				if (dumbBombVector[d].shape.getPosition().y > 1000) {
					//EXPLOSION ON THE GROUND
					float factorRand1 = 0.1f;

					normalExplosionVector.push_back(Explosion(&normalExplosionTexture, sf::Vector2f(dumbBombVector[d].shape.getPosition().x - 70, dumbBombVector[d].shape.getPosition().y - 220), (((float)rand() / (float)(RAND_MAX)) * factorRand1) + 1));
					normalExplosionAnimation.push_back(Animation(&normalExplosionTexture, sf::Vector2u(25, 1), timeDiffAnimationNormalExplosion));
					timeStartNormalExplosion.push_back(timeSinceStart);

					dumbBombVector.erase(dumbBombVector.begin() + d);//OUT OF SCREEN
					bufferBigExplode.loadFromFile("Sound/bigexplode.ogg");
					soundBigExplode.setBuffer(bufferBigExplode); //SET BUFFER
					soundBigExplode.play(); //PLAY SOUND

				}

				else if (dumbBombVector[d].shape.getGlobalBounds().intersects(player.getGlobalBounds()) && (playerAlive == true)) {//BOMB HIT PLAYER!

					if (shieldState == false) {
						bufferVoiceTankExplosion.loadFromFile("Sound/tankexplode.ogg");
						voiceTankExplosion.setBuffer(bufferVoiceTankExplosion);
						voiceTankExplosion.play();

						tankExplosionVector.push_back(Explosion(&TankExplosionTexture, sf::Vector2f(player.getPosition().x - 150, player.getPosition().y - 500), 2));
						tankExplosionAnimation.push_back(Animation(&TankExplosionTexture, sf::Vector2u(25, 1), timeDiffAnimationTankExplosion));
						timeStartTankExplosion.push_back(timeSinceStart);
						playerDeadExplosive = true;


						playerAlive = false;
						dumbBombVector.erase(dumbBombVector.begin() + d);//OUT OF SCREEN
						bufferBigExplode.loadFromFile("Sound/bigexplode.ogg");
						soundBigExplode.setBuffer(bufferBigExplode); //SET BUFFER
						soundBigExplode.play(); //PLAY SOUND

						deathTime = timeSinceStart;
					}
					else if (shieldState == true) {
						shieldState = false;
						dumbBombVector.erase(dumbBombVector.begin() + d);//OUT OF SCREEN

						bufferLostShield.loadFromFile("Sound/shielddown.ogg");
						lostShield.setBuffer(bufferLostShield);
						lostShield.play();
					}

				}

			}


			//Bullet ERASE and HIT=======================================================================================================================================

			for (size_t i = 0; i < bullets.size(); i++) {
				bullets[i].shape.move(bullets[i].currVelocity);
				// Bullet GO OUT OF SCREEN
				if (bullets[i].shape.getPosition().x < 0 || bullets[i].shape.getPosition().x > window.getSize().x
					|| bullets[i].shape.getPosition().y < 0 || bullets[i].shape.getPosition().y > window.getSize().y) {
					bullets.erase(bullets.begin() + i);
					break;
				}

				else {
					//Enemy Collision
					for (size_t k = 0; k < bomber.size(); k++) {
						if (bullets[i].shape.getGlobalBounds().intersects(bomber[k].shape.getGlobalBounds())) {
							bufferBulletHit.loadFromFile("Sound/bullethit.ogg");
							soundBulletHit.setBuffer(bufferBulletHit); //SET BUFFER
							soundBulletHit.play();
							bomber[k].HP = bomber[k].HP - 5;//BOMBER TAKE DMG



							if (stateBomberHit[k] == false) {
								stateBomberHit[k] = true;
								timeBlinkBomber[k] = timeSinceStart; //RED BLINK!!!!
							}



							if (bomber[k].HP <= 0) {

								midAirExplosionVector.push_back(Explosion(&midAirExplosionTexture, sf::Vector2f(static_cast<float>(bomber[k].shape.getPosition().x + 100), static_cast<float>(bomber[k].shape.getPosition().y - 150)), 1.3f));
								midAirExplosionAnimation.push_back(Animation(&midAirExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAirExplosion));
								timeStartMidAirExplosion.push_back(timeSinceStart);


								bomber.erase(bomber.begin() + k);//BOMBER DES
								bufferBigExplode.loadFromFile("Sound/bigexplode.ogg");
								soundBigExplode.setBuffer(bufferBigExplode); //SET BUFFER
								soundBigExplode.play();
								totalScore = totalScore + bomberPoint;

								stateBomberHit[k] = false;
								timeBlinkBomber.erase(timeBlinkBomber.begin() + k);
								stateBomberHit.erase(stateBomberHit.begin() + k);

							}
							bullets.erase(bullets.begin() + i);
							break;
						}
					}
				}

				//BULLET HIT DUMB BOMB
				for (size_t i = 0; i < bullets.size(); i++) {
					for (size_t d = 0; d < dumbBombVector.size(); d++) {
						if (bullets[i].shape.getGlobalBounds().intersects(dumbBombVector[d].shape.getGlobalBounds())) {
							dumbBombVector[d].HP = dumbBombVector[d].HP - 5;
							if (dumbBombVector[d].HP <= 0) {

								bufferVoiceSmallExplosion.loadFromFile("Sound/smallexplode.ogg");
								voiceSmallExplosion.setBuffer(bufferVoiceSmallExplosion);
								voiceSmallExplosion.play();

								skyBombExplosionVector.push_back(Explosion(&SkyBombExplosionTexture, sf::Vector2f(dumbBombVector[d].shape.getPosition().x, dumbBombVector[d].shape.getPosition().y), 0.75));
								skyBombExplosionAnimation.push_back(Animation(&SkyBombExplosionTexture, sf::Vector2u(32, 1), timeDiffAnimationSkyBombExplosion));
								timeStartSkyBombExplosion.push_back(timeSinceStart);


								dumbBombVector.erase(dumbBombVector.begin() + d);//DUMD BOMB DES
								totalScore = totalScore + dumbBombPoint;
							}
							bullets.erase(bullets.begin() + i);
							break;
						}
					}
				}

				//BULLET HIT JETBOMBER
				for (size_t i = 0; i < bullets.size(); i++) {
					for (size_t d = 0; d < jetfighter.size(); d++) {
						if (bullets[i].shape.getGlobalBounds().intersects(jetfighter[d].shape.getGlobalBounds())) {
							bufferBulletHit.loadFromFile("Sound/bullethit.ogg");
							soundBulletHit.setBuffer(bufferBulletHit); //SET BUFFER
							soundBulletHit.play();
							jetfighter[d].HP = jetfighter[d].HP - 5;

							if (stateJetFighterHit[d] == false) {
								stateJetFighterHit[d] = true;
								timeBlinkJetFighter[d] = timeSinceStart; //RED BLINK!!!!
							}

							if (jetfighter[d].HP <= 0) {

								midAir2ExplosionVector.push_back(Explosion(&midAir2ExplosionTexture, sf::Vector2f(jetfighter[d].shape.getPosition().x + 80, jetfighter[d].shape.getPosition().y - 80), 1));
								midAir2ExplosionAnimation.push_back(Animation(&midAir2ExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAir2Explosion));
								timeStartMidAir2Explosion.push_back(timeSinceStart);


								jetfighter.erase(jetfighter.begin() + d);//JET DES
								bufferBigExplode.loadFromFile("Sound/bigexplode.ogg");
								soundBigExplode.setBuffer(bufferBigExplode); //SET BUFFER
								soundBigExplode.play();
								totalScore = totalScore + dumbBombPoint;

								stateJetFighterHit[d] = false;
								timeBlinkJetFighter.erase(timeBlinkJetFighter.begin() + d);
								stateJetFighterHit.erase(stateJetFighterHit.begin() + d);

							}
							bullets.erase(bullets.begin() + i);
							break;
						}
					}
				}

			}




			//std::cout << bullets.size() << "\n";
			if (mouseControl == true) {
				//TANK FOLLOW AIM (AIM OPTION!)
				if (((sf::Mouse::getPosition(window).x) - player.getPosition().x >= 500) && (player.getPosition().x <= 1380)) {
					player.move(+8.0f*speedItemPick, 0.0f); // Go RIGHT
					turret.move(+8.0f*speedItemPick, 0.0f);
				}
				else if (((sf::Mouse::getPosition(window).x) - player.getPosition().x > 120) && (player.getPosition().x <= 1380)) {
					player.move(+5.0f*speedItemPick, 0.0f); // Go RIGHT
					turret.move(+5.0f*speedItemPick, 0.0f);
				}
				if (((sf::Mouse::getPosition(window).x) - player.getPosition().x <= -500) && (player.getPosition().x >= 60)) {
					player.move(-8.0f*speedItemPick, 0.0f); // Go LEFT
					turret.move(-8.0f*speedItemPick, 0.0f);
				}
				else if (((sf::Mouse::getPosition(window).x) - player.getPosition().x < -120) && (player.getPosition().x >= 60)) {
					player.move(-5.0f*speedItemPick, 0.0f); // Go LEFT
					turret.move(-5.0f*speedItemPick, 0.0f);
				}
				//std::cout << (sf::Mouse::getPosition(window).x) - player.getPosition().x <<std::endl;
			}

			//DEATHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH========================
			if ((timeSinceStart - deathTime <= 0.25) && (playerAlive == false) && totalReinforcement > 0) {

				bufferVoiceGetReady.loadFromFile("Sound/v_getready.ogg");
				voiceGetReady.setBuffer(bufferVoiceGetReady);
				voiceGetReady.play();
				if (totalReinforcement <= 1) {
					bufferVoiceLastOne.loadFromFile("Sound/v_lastone.ogg");
					voiceLastOne.setBuffer(bufferVoiceLastOne);
					voiceLastOne.play();
				}


			}

			else if ((timeSinceStart - deathTime <= 0.25) && (playerAlive == false) && totalReinforcement <= 0) {
				bufferVoiceGameover.loadFromFile("Sound/v_gameover.ogg");
				voiceGameover.setBuffer(bufferVoiceGameover);
				voiceGameover.play();
			}



			if ((timeSinceStart - deathTime >= 5) && (playerAlive == false) && totalReinforcement > 0) {
				player.setPosition(720.0f, 1047.0f);
				turret.setPosition(713.0f, 994.0f);
				--totalReinforcement;
				playerAlive = true;
				TimeItemDelay.restart();
				TimeItem.restart();

			}
			//BLINK!!!!!!!!!!!!========================================================================================================================================
			// BOMBER HIT!!!!!!!!!!/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			for (size_t k = 0; k < stateBomberHit.size(); k++) {
				if (stateBomberHit[k] == true) {

					if (timeSinceStart - timeBlinkBomber[k] < 0.1) {
						bomber[k].shape.setColor(sf::Color(255, 95, 95, 255));
					}

					else if ((timeSinceStart - timeBlinkBomber[k] >= 0.1) && (timeSinceStart - timeBlinkBomber[k] < 0.2)) {
						bomber[k].shape.setColor(sf::Color(255, 255, 255, 255));

					}
					else if (((timeSinceStart - timeBlinkBomber[k] >= 0.2) && (timeSinceStart - timeBlinkBomber[k] < 0.3))) {
						bomber[k].shape.setColor(sf::Color(255, 95, 95, 255));

					}
					else if (timeSinceStart - timeBlinkBomber[k] >= 0.3) {
						bomber[k].shape.setColor(sf::Color(255, 255, 255, 255));
						stateBomberHit[k] = false;
						timeBlinkBomber[k] = 0;
					}

				}
			}

			// JET FIGHTER HIT!!!!!!!!!!/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			for (size_t k = 0; k < stateJetFighterHit.size(); k++) {
				if (stateJetFighterHit[k] == true) {

					if (timeSinceStart - timeBlinkJetFighter[k] < 0.1) {
						jetfighter[k].shape.setColor(sf::Color(255, 95, 95, 255));
					}

					else if ((timeSinceStart - timeBlinkJetFighter[k] >= 0.1) && (timeSinceStart - timeBlinkJetFighter[k] < 0.2)) {
						jetfighter[k].shape.setColor(sf::Color(255, 255, 255, 255));

					}
					else if (((timeSinceStart - timeBlinkJetFighter[k] >= 0.2) && (timeSinceStart - timeBlinkJetFighter[k] < 0.3))) {
						jetfighter[k].shape.setColor(sf::Color(255, 95, 95, 255));

					}
					else if (timeSinceStart - timeBlinkJetFighter[k] >= 0.3) {
						jetfighter[k].shape.setColor(sf::Color(255, 255, 255, 255));
						stateJetFighterHit[k] = false;
						timeBlinkJetFighter[k] = 0;
					}

				}
			}

			// NEAR MISSLE HIT!!!!!!!!!!/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			for (size_t k = 0; k < stateMissleHit.size(); k++) {
				if (stateMissleHit[k] == true) {

					if (timeSinceStart - timeBlinkMissle[k] < 0.1) {
						missle[k].shape.setColor(sf::Color(255, 95, 95, 255));
					}

					else if ((timeSinceStart - timeBlinkMissle[k] >= 0.1) && (timeSinceStart - timeBlinkMissle[k] < 0.2)) {
						missle[k].shape.setColor(sf::Color(255, 255, 255, 255));

					}
					else if (((timeSinceStart - timeBlinkMissle[k] >= 0.2) && (timeSinceStart - timeBlinkMissle[k] < 0.3))) {
						missle[k].shape.setColor(sf::Color(255, 95, 95, 255));

					}
					else if (timeSinceStart - timeBlinkMissle[k] >= 0.3) {
						missle[k].shape.setColor(sf::Color(255, 255, 255, 255));
						stateMissleHit[k] = false;
						timeBlinkMissle[k] = 0;
					}

				}
			}


			//announce UPDATE========================================================================================================================================

			announcement.setString("GET READY!");
			gameover.setString("GAME OVER");


			//BackgroundUpdate=======================================================================================================================================

			wallpaper.WallUpdate(howToPlay);


			window.clear();

			if (howToPlay == false) {
				window.draw(wallpaper.spriteBackground2);
				window.draw(wallpaper.spriteBackground1); //Sky


				window.draw(wallpaper.spriteFarBackground2);
				window.draw(wallpaper.spriteFarBackground1); //Far Background
			}




			// NEAR MISSLE SPAWN & FAR MISSLE DRAW!!!!!!!!!!/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						//MISSLE!!!
			for (size_t c = 0; c < missleFarVector.size(); c++) {

				missleFarAnimation[c].Update(0, deltaTime, true);
				missleFarVector[c].explosionShape.setTextureRect(missleFarAnimation[c].uvRect);
				missleFarVector[c].explosionShape.move(0, -7.0);

				window.draw(missleFarVector[c].explosionShape); // Draw Explosion

				if (missleFarVector[c].explosionShape.getPosition().y <= -300) {



				
						missle.push_back(Enemy(&EnemyMissleTex, sf::Vector2f(missleFarVector[c].explosionShape.getPosition().x, -300), 25));
						missleNearAnimation.push_back(Animation(&missleFarTexture, sf::Vector2u(10, 1), timeDiffAnimationMissleFar));


						missleOnScreen = missleOnScreen + 1;
						///////////////Blink MISSLE INTNI
						timeBlinkMissle.push_back(0);
						stateMissleHit.push_back(false);

						//	std::cout << "MISSLE FIRE/////////////////////\n";

					

					missleFarVector.erase(missleFarVector.begin() + c);//OUT OF SCREEN

					missleFarAnimation.erase(missleFarAnimation.begin() + c);



				}

			}

			if (howToPlay == false) {
				window.draw(wallpaper.spriteCloseBackground2);
				window.draw(wallpaper.spriteCloseBackground1); // Close Background

				window.draw(wallpaper.spriteNearBackground2);
				window.draw(wallpaper.spriteNearBackground1); // Near Background


			}

			//////////////HOW TO PLAY////////////////////////////
			if (howToPlay == true) {
				window.draw(help);
				window.draw(wallpaper.spriteCloseBackground2);
				window.draw(wallpaper.spriteCloseBackground1); // Close Background
				window.draw(helpOuter);
			}

			if (howToPlay == true) {
				window.draw(howTo);

				if ((spawnCounter < 300)) {
					spawnCounter++;
				}
				if (spawnCounter >= 300 && bomber.size() < 2 && (playerAlive == true)) {
					bomber.push_back(Enemy(&EnemyBomberTex, sf::Vector2f(static_cast<float>(PosStartEnemyX), 340), 20));
					spawnCounter = 0;//RESET TIMER

					///////////////Blink BOMBER INTNI
					timeBlinkBomber.push_back(0);
					stateBomberHit.push_back(false);

				}

				for (size_t b = 0; b < bomber.size(); b++) {
					if ((playerAlive == true)) {
						bomber[b].shape.move(1.0f, 0.0f);
					}
					else {
						bomber[b].shape.move(15.0f, 0.0f);
					}


					if (spawnDumbBombCounter2 >= spawnDumbBombRate && dumbBombVector.size() < maxDumbBombOnScreen) {
						for (size_t a = 0; a < bomber.size(); a++) {
							if (((player.getPosition().x) - (bomber[a].shape.getPosition().x) >= 400) && ((player.getPosition().x) - (bomber[a].shape.getPosition().x) <= 700)) {
								dumbBombVector.push_back(Enemy(&DumbBombTex, sf::Vector2f((bomber[a].shape.getPosition().x + 100), (bomber[a].shape.getPosition().y + 20)), 5));
								spawnDumbBombCounter2 = 0;//RESET TIMER
								bufferBombFall.loadFromFile("Sound/bombfall.ogg");
								soundBombFall.setBuffer(bufferBombFall); //SET BUFFER
								soundBombFall.play(); //PLAY SOUND
							}
						}

					}

					if (bomber[b].shape.getPosition().x > 1500) {
						bomber.erase(bomber.begin() + b);//OUT OF SCREEN
						totalScore = totalScore - (bomberPoint / factorEscapeEnemy);

						stateBomberHit[b] = false;
						timeBlinkBomber.erase(timeBlinkBomber.begin() + b);
						stateBomberHit.erase(stateBomberHit.begin() + b);
					}


				}
			}

			// TANK EXPLOSION DRAW!!!!!!!!!!/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			if (playerAlive == true) {
				tankShadow.explosionShape.setPosition(player.getPosition().x - 90, player.getPosition().y - 30);
				window.draw(tankShadow.explosionShape);
			}


			//Shaking=======================================================================================================================================
			/*
			if (shaking > 0) {
				float shakeConstX = static_cast<float>(rand() % 5);
				float shakeConstY = static_cast<float>(rand() % 5);
				window.setPosition(sf::Vector2i(static_cast <int>(10 + shakeConstX), static_cast <int>(10 + shakeConstY)));
				if (++shaking == 5)
					shaking = 0;
			}
			*/

			//Draw===========================================================================================================================================
			// BOMBER DRAW!!!!!!!!!!/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			for (size_t b = 0; b < bomber.size(); b++) {
				eHpText.setString(std::to_string(bomber[b].HP) + "/" + std::to_string(bomber[b].HPMax));
				eHpText.setPosition(bomber[b].shape.getPosition().x + 100, bomber[b].shape.getPosition().y - eHpText.getGlobalBounds().height - 5);
				window.draw(eHpText);
				window.draw(bomber[b].shape); // Draw Enemies Bomber
			}

			// JET FIGHTER DRAW!!!!!!!!!!/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			for (size_t b = 0; b < jetfighter.size(); b++) {
				eHpText.setString(std::to_string(jetfighter[b].HP) + "/" + std::to_string(jetfighter[b].HPMax));
				eHpText.setPosition(jetfighter[b].shape.getPosition().x + 50, jetfighter[b].shape.getPosition().y - eHpText.getGlobalBounds().height - 20);
				window.draw(eHpText);
				window.draw(jetfighter[b].shape); // Draw Enemies JetFighter
			}

			// NEAR MISSLE DRAW!!!!!!!!!!/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			for (size_t b = 0; b < missle.size(); b++) {

				missleNearAnimation[b].Update(0, deltaTime, true);
				missle[b].shape.setTextureRect(missleNearAnimation[b].uvRect);

				eHpText.setString(std::to_string(missle[b].HP) + "/" + std::to_string(missle[b].HPMax));
				eHpText.setPosition(missle[b].shape.getPosition().x + 50, missle[b].shape.getPosition().y - eHpText.getGlobalBounds().height - 20);
				window.draw(eHpText);

				window.draw(missle[b].shape); // Draw Enemies Missle
			}

			// BULLET DRAW!!!!!!!!!!/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			for (size_t i = 0; i < bullets.size(); i++) {
				window.draw(bullets[i].shape); // Draw Bullet
			}

			// DUMB BOMB DRAW!!!!!!!!!!/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			for (size_t b = 0; b < dumbBombVector.size(); b++) {

				window.draw(dumbBombVector[b].shape); // Draw Dumb Bomb
			}

			// PLAYER DRAW //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			animation.Update(0, deltaTime, true);
			player.setTextureRect(animation.uvRect);


			if (playerAlive == true) {
				window.draw(turret);
				window.draw(player);
				window.draw(fireball);

			}

			//HUD ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (howToPlay == false) {
				window.draw(bar);
			}

			if ((timeSinceStart >= 0.05) && (timeSinceStart <= 2.5) && (howToPlay == false)) {
				window.draw(announcement);
			}

			if ((playerAlive == false) && (timeSinceStart - deathTime <= 4) && totalReinforcement <= 0) {
				window.draw(gameover);
			}
			else if ((playerAlive == false) && (timeSinceStart - deathTime <= 4)) {
				window.draw(announcement);
			}

			if ((totalReinforcement <= 0) && (playerAlive == false)) {
				char bufferTime2[50];
				ftoa(deathTime, bufferTime2, 2);
				std::string endTime = bufferTime2;
				endScore.setString("Your Score : " + std::to_string(totalScore) + " + " + "Survival Time : " + endTime);
				endScore.setOrigin(endScore.getGlobalBounds().width / 2, endScore.getGlobalBounds().height / 2);
				endScore.setPosition(720, 600);

				if ((timeSinceStart - deathTime >= 5) && (reportScore == false)) {
					//FILE WRITE


					std::ifstream bufferInPut("Score/Score.txt");
					for (int i = 0; i < 5; i++) {
						if (bufferInPut.is_open()) {
							bufferInPut >> topName5[i] >> topScore5[i];
						}
					}
					bufferInPut.close();
					currentName = textbox1.getText();
					topName5[5] = currentName;
					topScore5[5] = totalScore;

					for (x = 0; x < 6; x++) {
						for (y = x + 1; y < 6; y++) {
							if (topScore5[y] > topScore5[x]) {
								tempScore = topScore5[x];
								topScore5[x] = topScore5[y];
								topScore5[y] = tempScore;

								tempName = topName5[x];
								topName5[x] = topName5[y];
								topName5[y] = tempName;
							}
						}
					}
					if (reportScore == false) {
						std::cout << topName5[0] << topScore5[0] << std::endl;
						std::cout << topName5[1] << topScore5[1] << std::endl;
						std::cout << topName5[2] << topScore5[2] << std::endl;
						std::cout << topName5[3] << topScore5[3] << std::endl;
						std::cout << topName5[4] << topScore5[4] << std::endl;
						std::cout << topName5[5] << topScore5[5] << std::endl;

						std::ofstream bufferOutPut("Score/Score.txt");
						if (bufferOutPut.is_open()) {
							bufferOutPut << topName5[0] << " " << topScore5[0] << std::endl;;
							bufferOutPut << topName5[1] << " " << topScore5[1] << std::endl;;
							bufferOutPut << topName5[2] << " " << topScore5[2] << std::endl;;
							bufferOutPut << topName5[3] << " " << topScore5[3] << std::endl;;
							bufferOutPut << topName5[4] << " " << topScore5[4] << std::endl;;
						}
						bufferOutPut.close();

						reportScore = true;
					}

				}







			}

			if ((playerAlive == false) && (timeSinceStart - deathTime >= 6) && totalReinforcement <= 0) {

				window.draw(blank);
				window.draw(endGame);
				window.draw(clickHere2);

				if (totalScore >= 0) {
					endScore.setFillColor(sf::Color::Green);
				}
				else {
					endScore.setFillColor(sf::Color::Red);
				}

				window.draw(endScore);
				gameOverAlready = true;

			}

			if (((timeSinceStart >= 2.5) && (timeSinceStart <= 2.75)) || ((timeSinceStart >= 3.0) && (timeSinceStart <= 3.25)) || (timeSinceStart >= 3.5)) {
				//Survival time UPDATE========================================================================================================================================

				survivalTime = timeSinceStart;

				char bufferTime[50];
				ftoa(survivalTime, bufferTime, 2);
				std::string bufferStringTime = bufferTime;

				if ((totalReinforcement <= 0) && (playerAlive == false)) {
					ftoa(deathTime, bufferTime, 2);
					std::string bufferStringTime = bufferTime;
					timeSurvival.setFillColor(sf::Color::Red);
					timeSurvival.setString("Survival Time : " + bufferStringTime);
				}

				else {
					timeSurvival.setFillColor(sf::Color::Green);
					timeSurvival.setString("Survival Time : " + bufferStringTime);
				}
				if (howToPlay == false) {
					window.draw(timeSurvival);
				}



				//Support time UPDATE========================================================================================================================================
				timeTillSupport = 25;
				timeTillSupport = timeTillSupport - timeSinceSupport;
				//std::cout << timeTillSupport << std::endl;
				if ((playerAlive == true) && (howToPlay == false)) {
					if ((timeTillSupport <= 1) && (howToPlay == false)) {
						TimeItemDelay.restart();
						TimeItem.restart();
					}
					else if ((TimeItemDelay.getElapsedTime().asSeconds() <= 5 && survivalTime >= 7) && (timeSinceStart - deathTime >= 5)) {
						timeSupport.setFillColor(sf::Color::Green);
						timeSupport.setString("Next Support  : INCOMING");
						window.draw(timeSupport);


						if (callAirSupport == false) {

							airSupportNow = true;
							callAirSupport = true;
						}



					}
					else {
						char bufferTime2[50];
						ftoa(timeTillSupport, bufferTime2, 2);
						std::string bufferStringTime2 = bufferTime2;
						timeSupport.setFillColor(sf::Color::Yellow);
						timeSupport.setString("Next Support  : " + bufferStringTime2);
						window.draw(timeSupport);

						airSupportNow = false;
						callAirSupport = false;


					}
				}
				else {
					timeSupport.setFillColor(sf::Color::Red);
					timeSupport.setString("Next Support  : DELAY");
					if (howToPlay == false) {
						window.draw(timeSupport);
					}
				}

			}


			//FRIENDLY!!!!!!!!!!!!!!!!!!!//////////////////////////////////////////////////////////////////////////////////////////////////////////

			if ((airSupportNow == true) && (playerAlive == true) && (howToPlay == false)) {

				bufferCare.loadFromFile("Sound/v_care1.ogg");
				voiceCare.setBuffer(bufferCare);
				voiceCare.setVolume(70);
				voiceCare.play();

				airSupportNow = false;



				friendlyVector.push_back(Friendly(&FriendlyTex, sf::Vector2f(-200, static_cast<float>(rand() % 200 + PosStartEnemyY)), 100, 0.7f));
				friendlyAnimation.push_back(Animation(&FriendlyTex, sf::Vector2u(15, 1), timeDiffAnimationFriendly));

				bufferCopter.loadFromFile("Sound/pupcopter.ogg");
				voiceCopter.setBuffer(bufferCopter);
				voiceCopter.play();




				///////////////Blink INTNI
				timeBlinkFriendly.push_back(0);
				stateFriendlyHit.push_back(false);

				bool friendlyCallOut1 = true;
				bool friendlyCallOut2 = true;
				bool friendlyCallOut3 = true;




			}




			for (size_t b = 0; b < friendlyVector.size(); b++) {
				eHpText.setString(std::to_string(friendlyVector[b].HP) + "/" + std::to_string(friendlyVector[b].HPMax));
				eHpText.setPosition(friendlyVector[b].shape.getPosition().x + 100, friendlyVector[b].shape.getPosition().y - eHpText.getGlobalBounds().height - 5);
				window.draw(eHpText);
				friendlyAnimation[b].Update(0, deltaTime, true);
				friendlyVector[b].shape.setTextureRect(friendlyAnimation[b].uvRect);
				window.draw(friendlyVector[b].shape); // Draw Friendly Bomber
			}

			for (size_t b = 0; b < friendlyVector.size(); b++) {
				if ((playerAlive == true)) {
					friendlyVector[b].shape.move(5.0f, 0.0f);
				}
				else {
					friendlyVector[b].shape.move(15.0f, 0.0f);
				}

			}

			for (size_t i = 0; i < bullets.size(); i++) {
				for (size_t b = 0; b < friendlyVector.size(); b++) {



					if (bullets[i].shape.getGlobalBounds().intersects(friendlyVector[b].shape.getGlobalBounds())) {
						bufferBulletHit.loadFromFile("Sound/bullethit.ogg");
						soundBulletHit.setBuffer(bufferBulletHit); //SET BUFFER
						soundBulletHit.play();
						friendlyVector[b].HP = friendlyVector[b].HP - 5;

						if (stateFriendlyHit[b] == false) {
							stateFriendlyHit[b] = true;
							timeBlinkFriendly[b] = timeSinceStart; //RED BLINK!!!!
						}

						if ((friendlyVector[b].HP <= 15) && (friendlyCallOut1 == true)) {
							voiceFriendly.pause();
							voiceFriendly.resetBuffer();
							bufferVoiceFriendly.loadFromFile("Sound/friendly2.ogg");
							voiceFriendly.setBuffer(bufferVoiceFriendly);
							voiceFriendly.play();

							friendlyCallOut1 = false;
						}
						else if ((friendlyVector[b].HP <= 40) && (friendlyCallOut2 == true)) {
							voiceFriendly.pause();
							voiceFriendly.resetBuffer();
							bufferVoiceFriendly.loadFromFile("Sound/friendly3.ogg");
							voiceFriendly.setBuffer(bufferVoiceFriendly);
							voiceFriendly.play();

							friendlyCallOut2 = false;
						}
						else if ((friendlyVector[b].HP <= 65) && (friendlyCallOut3 == true)) {
							voiceFriendly.pause();
							voiceFriendly.resetBuffer();
							bufferVoiceFriendly.loadFromFile("Sound/friendly1.ogg");
							voiceFriendly.setBuffer(bufferVoiceFriendly);
							voiceFriendly.play();

							friendlyCallOut3 = false;
						}


						if (friendlyVector[b].HP <= 0) {

							midAirExplosionVector.push_back(Explosion(&midAirExplosionTexture, sf::Vector2f(static_cast<float>(friendlyVector[b].shape.getPosition().x + 50), static_cast<float>(friendlyVector[b].shape.getPosition().y - 100)), 1.3f));
							midAirExplosionAnimation.push_back(Animation(&midAirExplosionTexture, sf::Vector2u(19, 1), timeDiffAnimationMidAirExplosion));
							timeStartMidAirExplosion.push_back(timeSinceStart);

							voiceCopter.stop();

							alreadyDropItem = false;

							friendlyVector.erase(friendlyVector.begin() + b);//BOMBER DES
							bufferBigExplode.loadFromFile("Sound/bigexplode.ogg");
							soundBigExplode.setBuffer(bufferBigExplode); //SET BUFFER
							soundBigExplode.play();
							totalScore = totalScore + friendlyPoint;

							stateFriendlyHit[b] = false;
							timeBlinkFriendly.erase(timeBlinkFriendly.begin() + b);
							stateFriendlyHit.erase(stateFriendlyHit.begin() + b);


							airSupportNow = false;

							bufferVoiceFriendly.loadFromFile("Sound/friendlydie.ogg");
							voiceFriendly.setBuffer(bufferVoiceFriendly);
							voiceFriendly.play();

							friendlyCallOut1 = true;
							friendlyCallOut2 = true;
							friendlyCallOut3 = true;

						}


						bullets.erase(bullets.begin() + i);
						break;
					}

					for (size_t b = 0; b < friendlyVector.size(); b++) {
						if (friendlyVector[b].shape.getPosition().x > 1500) {
							friendlyVector.erase(friendlyVector.begin() + b);//OUT OF SCREEN


							alreadyDropItem = false;

							stateFriendlyHit[b] = false;
							timeBlinkFriendly.erase(timeBlinkFriendly.begin() + b);
							stateFriendlyHit.erase(stateFriendlyHit.begin() + b);


							airSupportNow = false;

							friendlyCallOut1 = true;
							friendlyCallOut2 = true;
							friendlyCallOut3 = true;


						}
					}
				}
			}




			//Score UPDATE=======================================================================================================================================
			score.setString("Score : " + std::to_string(totalScore));
			if (totalScore < 0) {
				score.setFillColor(sf::Color::Red);
			}
			else {
				score.setFillColor(sf::Color::Yellow);
			}
			if (howToPlay == false) {
				window.draw(score);
			}

			//Reinforcement UPDATE=======================================================================================================================================
			reinforcement.setString("Reinforcement : " + std::to_string(totalReinforcement));
			if (totalReinforcement == 1) {
				reinforcement.setFillColor(sf::Color::Yellow);
			}
			else if (totalReinforcement == 0) {
				reinforcement.setFillColor(sf::Color::Red);
			}
			else {
				reinforcement.setFillColor(sf::Color::Green);
			}

			if (howToPlay == false) {
				window.draw(reinforcement);
			}

			//NORMAL EXPLOSION DRAW ///////////////////////////////////////////////////////////////////////////////////////////////

			for (size_t c = 0; c < normalExplosionVector.size(); c++) {

				normalExplosionAnimation[c].Update(0, deltaTime, true);
				normalExplosionVector[c].explosionShape.setTextureRect(normalExplosionAnimation[c].uvRect);
				normalExplosionVector[c].explosionShape.move(-4.5, 0);

				if (timeSinceStart - timeStartNormalExplosion[c] <= 1.22) {
					window.draw(normalExplosionVector[c].explosionShape); // Draw Explosion
				}

				if (normalExplosionVector[c].explosionShape.getPosition().x < -200) {
					normalExplosionVector.erase(normalExplosionVector.begin() + c);//OUT OF SCREEN
					timeStartNormalExplosion.erase(timeStartNormalExplosion.begin() + c);
					normalExplosionAnimation.erase(normalExplosionAnimation.begin() + c);

				}

			}

			//MID AIR EXPLOSION DRAW ///////////////////////////////////////////////////////////////////////////////////////////////

			for (size_t c = 0; c < midAirExplosionVector.size(); c++) {

				midAirExplosionAnimation[c].Update(0, deltaTime, true);
				midAirExplosionVector[c].explosionShape.setTextureRect(midAirExplosionAnimation[c].uvRect);
				midAirExplosionVector[c].explosionShape.move(-4.5, 0);

				if (timeSinceStart - timeStartMidAirExplosion[c] <= 0.9) {
					window.draw(midAirExplosionVector[c].explosionShape); // Draw Explosion
				}

				if (midAirExplosionVector[c].explosionShape.getPosition().x < -200) {
					midAirExplosionVector.erase(midAirExplosionVector.begin() + c);//OUT OF SCREEN
					timeStartMidAirExplosion.erase(timeStartMidAirExplosion.begin() + c);
					midAirExplosionAnimation.erase(midAirExplosionAnimation.begin() + c);

				}

			}

			//MID AIR EXPLOSION 2 DRAW ///////////////////////////////////////////////////////////////////////////////////////////////

			for (size_t c = 0; c < midAir2ExplosionVector.size(); c++) {

				midAir2ExplosionAnimation[c].Update(0, deltaTime, true);
				midAir2ExplosionVector[c].explosionShape.setTextureRect(midAir2ExplosionAnimation[c].uvRect);
				midAir2ExplosionVector[c].explosionShape.move(-4.5, 0);

				if (timeSinceStart - timeStartMidAir2Explosion[c] <= 0.9) {
					window.draw(midAir2ExplosionVector[c].explosionShape); // Draw Explosion
				}

				if (midAir2ExplosionVector[c].explosionShape.getPosition().x < -200) {
					midAir2ExplosionVector.erase(midAir2ExplosionVector.begin() + c);//OUT OF SCREEN
					timeStartMidAir2Explosion.erase(timeStartMidAir2Explosion.begin() + c);
					midAir2ExplosionAnimation.erase(midAir2ExplosionAnimation.begin() + c);

				}

			}


			//SKY EXPLOSION DRAW ///////////////////////////////////////////////////////////////////////////////////////////////


			for (size_t c = 0; c < skyBombExplosionVector.size(); c++) {

				skyBombExplosionAnimation[c].Update(0, deltaTime, true);
				skyBombExplosionVector[c].explosionShape.setTextureRect(skyBombExplosionAnimation[c].uvRect);
				skyBombExplosionVector[c].explosionShape.move(-4.5, 0);

				if (timeSinceStart - timeStartSkyBombExplosion[c] <= 0.9) {
					window.draw(skyBombExplosionVector[c].explosionShape); // Draw Explosion
				}

				if (skyBombExplosionVector[c].explosionShape.getPosition().x < -200) {
					skyBombExplosionVector.erase(skyBombExplosionVector.begin() + c);//OUT OF SCREEN
					timeStartSkyBombExplosion.erase(timeStartSkyBombExplosion.begin() + c);
					skyBombExplosionAnimation.erase(skyBombExplosionAnimation.begin() + c);

				}

			}


			// TANK EXPLOSION /////////////////////////////////////////////////////////////////////////////////////////////////

			for (size_t c = 0; c < tankExplosionVector.size(); c++) {

				tankExplosionAnimation[c].Update(0, deltaTime, true);
				tankExplosionVector[c].explosionShape.setTextureRect(tankExplosionAnimation[c].uvRect);
				tankExplosionVector[c].explosionShape.move(-4.5, 0);

				if ((playerDeadExplosive == true) && (timeSinceStart - timeStartTankExplosion[c] <= 1.9)) {
					window.draw(tankExplosionVector[c].explosionShape); // Draw Explosion
				}


				if (tankExplosionVector[c].explosionShape.getPosition().x < -200) {
					tankExplosionVector.erase(tankExplosionVector.begin() + c);//OUT OF SCREEN
					timeStartTankExplosion.erase(timeStartTankExplosion.begin() + c);
					tankExplosionAnimation.erase(tankExplosionAnimation.begin() + c);

				}

			}


			// FREINDLY HIT///////////////////////////////////////////////////////////////////////////////////////////////////

			for (size_t k = 0; k < stateFriendlyHit.size(); k++) {
				if (stateFriendlyHit[k] == true) {

					if (timeSinceStart - timeBlinkFriendly[k] < 0.1) {
						friendlyVector[k].shape.setColor(sf::Color(255, 95, 95, 255));
					}

					else if ((timeSinceStart - timeBlinkFriendly[k] >= 0.1) && (timeSinceStart - timeBlinkFriendly[k] < 0.2)) {
						friendlyVector[k].shape.setColor(sf::Color(255, 255, 255, 255));
						dontShoot.setPosition(friendlyVector[k].shape.getPosition().x, friendlyVector[k].shape.getPosition().y);
						window.draw(dontShoot);



					}
					else if (((timeSinceStart - timeBlinkFriendly[k] >= 0.2) && (timeSinceStart - timeBlinkFriendly[k] < 0.3))) {
						friendlyVector[k].shape.setColor(sf::Color(255, 95, 95, 255));

					}
					else if (timeSinceStart - timeBlinkFriendly[k] >= 0.3) {
						friendlyVector[k].shape.setColor(sf::Color(255, 255, 255, 255));
						dontShoot.setPosition(friendlyVector[k].shape.getPosition().x, friendlyVector[k].shape.getPosition().y);
						window.draw(dontShoot);
						stateFriendlyHit[k] = false;
						timeBlinkFriendly[k] = 0;
					}

				}
			}

			//MISSLE FAR ATTACK!!!!!!!!!!!!!!!!!!!!! SPAWN FAR MISSLE///////////////////////////////////////////////////////////////////

			if ((timeMissleAttack >= 15) && (alreadyMissleFar == false) && (playerAlive == true) && (missleOnScreen <= maxMissleOnScreen) && (howToPlay == false)) {



				bufferAirRaid.loadFromFile("Sound/airraid.ogg");
				voiceAirRaid.setBuffer(bufferAirRaid);
				voiceAirRaid.play();


				missleFarVector.push_back(Explosion(&missleFarTexture, sf::Vector2f(360, 1200), 2));
				missleFarAnimation.push_back(Animation(&missleFarTexture, sf::Vector2u(10, 1), timeDiffAnimationMissleFar));

				missleFarVector.push_back(Explosion(&missleFarTexture, sf::Vector2f(720, 1200), 2));
				missleFarAnimation.push_back(Animation(&missleFarTexture, sf::Vector2u(10, 1), timeDiffAnimationMissleFar));



				missleFarVector.push_back(Explosion(&missleFarTexture, sf::Vector2f(1080, 1200), 2));
				missleFarAnimation.push_back(Animation(&missleFarTexture, sf::Vector2u(10, 1), timeDiffAnimationMissleFar));


				alreadyMissleFar = true;

				std::cout << "FIRE!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;

				continue;
			}
			///////////////////////////////////////////////////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////TANK ICON
			if (howToPlay == false) {
				window.draw(tankIcon);
			}





			//std::cout << alreadyDropItem << std::endl;

			for (size_t b = 0; b < friendlyVector.size(); b++) {

				if ((friendlyVector[b].shape.getPosition().x > 600) && (friendlyVector[b].shape.getPosition().x < 700) && (alreadyDropItem == false)) {
					alreadyDropItem = true;
					itemOnScreen = itemOnScreen + 1;
					itemRand = static_cast<int>(rand() % 5 + 1);
					//	std::cout << "DROP ITEM!" << std::endl;

					if (itemRand == 1) {
						itemText.setSmooth(true);
						itemText.loadFromFile("Tank/nuke.png"); // LOAD Texture
						item.setTexture(itemText); // Add Texture
						item.setScale(static_cast<float>(2.3), static_cast<float>(2.3));
						item.setOrigin(item.getGlobalBounds().width / 2, item.getGlobalBounds().height / 2);
					}
					else if (itemRand == 2) {
						itemText.setSmooth(true);
						itemText.loadFromFile("Tank/extra.png"); // LOAD Texture
						item.setTexture(itemText); // Add Texture
						item.setScale(static_cast<float>(2.3), static_cast<float>(2.3));
						item.setOrigin(item.getGlobalBounds().width / 2, item.getGlobalBounds().height / 2);
					}
					else if (itemRand == 3) {
						itemText.setSmooth(true);
						itemText.loadFromFile("Tank/speed.png"); // LOAD Texture
						item.setTexture(itemText); // Add Texture
						item.setScale(static_cast<float>(2.3), static_cast<float>(2.3));
						item.setOrigin(item.getGlobalBounds().width / 2, item.getGlobalBounds().height / 2);
					}
					else if (itemRand == 4) {
						itemText.setSmooth(true);
						itemText.loadFromFile("Tank/rapid.png"); // LOAD Texture
						item.setTexture(itemText); // Add Texture
						item.setScale(static_cast<float>(2.3), static_cast<float>(2.3));
						item.setOrigin(item.getGlobalBounds().width / 2, item.getGlobalBounds().height / 2);
					}
					else if (itemRand == 5) {
						itemText.setSmooth(true);
						itemText.loadFromFile("Tank/shield.png"); // LOAD Texture
						item.setTexture(itemText); // Add Texture
						item.setScale(static_cast<float>(2.3), static_cast<float>(2.3));
						item.setOrigin(item.getGlobalBounds().width / 2, item.getGlobalBounds().height / 2);
					}

					item.setPosition(friendlyVector[b].shape.getPosition().x + 150, friendlyVector[b].shape.getPosition().y + 150);


				}




			}





			if ((itemOnScreen > 0) && (gameState == 1) && (howToPlay == false) && (gameOverAlready == false)) {

				if (item.getPosition().y <= 1050) {
					item.move(0, 3);
				}
				if (item.getGlobalBounds().intersects(player.getGlobalBounds()) && (playerAlive == true)) {

					itemPickTime = timeSinceStart;
					itemOnScreen = itemOnScreen - 1;

					if (itemRand == 1) {
						numberOfNuke = numberOfNuke + 1;
						bufferNukeUp.loadFromFile("Sound/powerup.ogg");
						nukeUp.setBuffer(bufferNukeUp);
						nukeUp.play();



					}
					else if (itemRand == 2) {
						++totalReinforcement;
						bufferRein.loadFromFile("Sound/reinforcements.ogg");
						rein.setBuffer(bufferRein);
						rein.play();


					}
					else if (itemRand == 3) {
						speedItemPick = speedItemPick + 0.5f;
						bufferSpeedUp.loadFromFile("Sound/speedup.ogg");
						speedUp.setBuffer(bufferSpeedUp);
						speedUp.play();



					}
					else if (itemRand == 4) {
						if (spawnbulletRate >= 6) {
							spawnbulletRate = spawnbulletRate - 2;
							bufferGunUp.loadFromFile("Sound/gunpowerup.ogg");
							gunUp.setBuffer(bufferGunUp);
							gunUp.play();

							itemCallOut.setString("Rapid Fire");
							itemCallOut.setPosition(static_cast<float>(itemCallOut.getGlobalBounds().width / 2), static_cast<float>(itemCallOut.getGlobalBounds().height / 2));
							itemCallOut.setPosition(720.0f, 540.0f);
						}
					}
					else if (itemRand == 5) {
						shieldState = true;

						bufferShield.loadFromFile("Sound/shieldup.ogg");
						shield.setBuffer(bufferShield);
						shield.play();

						itemCallOut.setString("Energy Shield");
						itemCallOut.setPosition(static_cast<float>(itemCallOut.getGlobalBounds().width / 2), static_cast<float>(itemCallOut.getGlobalBounds().height / 2));
						itemCallOut.setPosition(720.0f, 540.0f);
					}
					alreadyDropItem = false;

				}
				else {
					if ((howToPlay == false) && (gameState == 1)) {
						window.draw(item);
					}
				}
			}

			if ((timeSinceStart - itemPickTime <= 4) && (itemOnScreen <= 0) && (gameState == 1) && (gameOverAlready == false) && (timeSinceStart >= 7)) {

				if (itemRand == 1) {
					itemCallOut.setString("Tactical Nuke");
					itemCallOut.setOrigin(static_cast<float>(itemCallOut.getGlobalBounds().width / 2), static_cast<float>(itemCallOut.getGlobalBounds().height / 2));
					itemCallOut.setPosition(720.0f, 540.0f);
				}

				else if (itemRand == 2) {
					itemCallOut.setString("Extra Reinforcement");
					itemCallOut.setOrigin(static_cast<float>(itemCallOut.getGlobalBounds().width / 2), static_cast<float>(itemCallOut.getGlobalBounds().height / 2));
					itemCallOut.setPosition(720.0f, 540.0f);
				}

				else if (itemRand == 3) {
					itemCallOut.setString("Speed Increase");
					itemCallOut.setOrigin(static_cast<float>(itemCallOut.getGlobalBounds().width / 2), static_cast<float>(itemCallOut.getGlobalBounds().height / 2));
					itemCallOut.setPosition(720.0f, 540.0f);
				}

				else if (itemRand == 4) {
					itemCallOut.setString("Rapid Fire");
					itemCallOut.setOrigin(static_cast<float>(itemCallOut.getGlobalBounds().width / 2), static_cast<float>(itemCallOut.getGlobalBounds().height / 2));
					itemCallOut.setPosition(720.0f, 540.0f);
				}
				else if (itemRand == 5) {
					itemCallOut.setString("Energy Shield");
					itemCallOut.setOrigin(static_cast<float>(itemCallOut.getGlobalBounds().width / 2), static_cast<float>(itemCallOut.getGlobalBounds().height / 2));
					itemCallOut.setPosition(720.0f, 540.0f);
				}


				window.draw(itemCallOut);

			}

			if (numberOfNuke > 0) {
				if (howToPlay == true) {
					nukeIcon.setPosition(1350.0f, 200.0f);
					window.draw(nukeIcon);
				}
				else {
					nukeIcon.setPosition(1430.0f, 195.0f);
					window.draw(nukeIcon);
				}
			}

			if (shieldState == true) {
				tankShield.setPosition(player.getPosition().x - 85, player.getPosition().y - 130);
				window.draw(tankShield);
			}




			std::cout << numberOfNuke << std::endl;
			//std::cout << missleOnScreen << std::endl;
			window.setMouseCursorVisible(false);
			newCursorAnimation.Update(0, deltaTime, true);
			newCursor.explosionShape.setTextureRect(newCursorAnimation.uvRect);
			newCursor.explosionShape.setOrigin(sf::Vector2f((newCursorAnimation.uvRect.width / 2.0f), (newCursorAnimation.uvRect.height / 2.0f)));
			newCursor.explosionShape.setPosition(sf::Mouse::getPosition().x - 23.0f, sf::Mouse::getPosition().y - 65.0f);
			window.draw(newCursor.explosionShape);



			window.draw(hideVoid);
			//window.draw(dumbBomb);


		}



		window.display();

	}

	return 0;
}


