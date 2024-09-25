#include <SFML\Graphics.hpp>
#include <string>

class Menu {
private:
	sf::RectangleShape box;
	sf::Font font;
	std::vector<sf::Text> menuOptions;
public:
	void mainMenu();
	//void losingScreen();
	//void pausingScreen();
};