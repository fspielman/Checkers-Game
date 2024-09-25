#pragma once
#include <SFML\Graphics.hpp>
//using std::string;
#include <iostream>

class Timer {
private:
	sf::Time time;
	sf::Clock clock;
	bool isRunning;
public:
	Timer();
	void startTimer();
	sf::Time getTime() const;
	void stopTimer();
	void DisplayTime(sf::Text& txt);
	bool isTimeFinished() const;
};
