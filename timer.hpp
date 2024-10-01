#pragma once
#include <SFML\Graphics.hpp>

class Timer {
private:
	sf::Time time;
	sf::Clock clock;
	bool isRunning;
public:
	sf::Time getTime() const;
	Timer();
	void startTimer();
	void stopTimer();
	void DisplayTime(sf::Text& txt);
	bool isTimeFinished() const;
};
