/*#include "menu.hpp"


void Menu::mainMenu() {

	sf::Font TimesNewRoman;
	
	TimesNewRoman.loadFromFile("times.ttf");

	//play button
	sf::RectangleShape play(sf::Vector2f(100,500));
	play.setPosition(200, 350);
	sf::Text playTxt;
	playTxt.setFont(TimesNewRoman);
	playTxt.setString("Play");
	playTxt.setColor(sf::Color::White);
	playTxt.setCharacterSize(36);

	sf::RectangleShape quit(sf::Vector2f(100, 100));
	quit.setPosition(500, 350);
	sf::Text quitTxt;
	quitTxt.setFont(TimesNewRoman);
	quitTxt.setString("Play");
	quitTxt.setColor(sf::Color::White);
	quitTxt.setCharacterSize(36);

	sf::RectangleShape setTimer3(sf::Vector2f(50, 50));
	setTimer3.setPosition(700, 350);
	sf::Text timer3Txt;
	timer3Txt.setFont(TimesNewRoman);
	timer3Txt.setString("Play");
	timer3Txt.setColor(sf::Color::White);
	timer3Txt.setCharacterSize(36);

}
/*
void Menu::losingScreen() {

}

void Menu::pausingScreen() {

}*/
