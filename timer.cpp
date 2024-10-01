#include "timer.hpp"

//getter
sf::Time Timer::getTime() const {
    if (isRunning) {
        sf::Time remaining = time - clock.getElapsedTime();
        return (remaining > sf::Time::Zero) ? remaining : sf::Time::Zero;
    }
    return time;
}

//methods
Timer::Timer() {
    time = sf::seconds(300);
    isRunning = false;
}

void Timer::startTimer() {
    if (!isRunning) {
        clock.restart();         
        isRunning = true;
   }
}

void Timer::stopTimer() {
    if (isRunning) {
        time = getTime();
        isRunning = false;
    }
}

void Timer::DisplayTime(sf::Text& txt) {
    sf::Time remainingTime = getTime();
    int remaining = static_cast<int>(remainingTime.asSeconds());
    int min = remaining / 60;
    int sec = remaining % 60;
    
    //add a 0 in front of seconds when seconds is less than 10
    txt.setString(std::to_string(min) + ":" + (sec < 10 ? "0" : "") + std::to_string(sec));
}

bool Timer::isTimeFinished() const {
    sf::Time remainingTime = getTime();
    return remainingTime == sf::Time::Zero;
}

