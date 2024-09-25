#include <SFML/Graphics.hpp>
#include <iostream>
#include "board.hpp"
#include "timer.hpp"

int main()
{
	bool isFullscreen = false;

	sf::RenderWindow window(sf::VideoMode(900, 850), "Checkers", sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize);
	sf::View view(sf::FloatRect(0, 0, 900, 850));

	Board board(window);
	board.initBoard();

	std::string turn = "White";

	bool shapeSelected = false;
	bool movesAvailable = true;
	
	int startRow = -1;
	int startCol = -1;
	int row;
	int col;

	std::shared_ptr<Piece> selectedPiece = nullptr;
	std::shared_ptr<Piece> isPieceAtTargetPos = nullptr;
	bool jumpsAvailable = false;
	std::string otherColor;
	std::vector<std::pair<int, int>> visitedSquare;

	sf::Font TimesNewRoman;
	if (!TimesNewRoman.loadFromFile("times.ttf")) {
		return -1;
	}

	sf::Text redClockTxt, whiteClockTxt, pieceCntTxt;
	Timer redClock, whiteClock;

	for (auto& text : { &redClockTxt, &whiteClockTxt, &pieceCntTxt }) {
		text->setFont(TimesNewRoman);
		text->setCharacterSize(36);
	}

	pieceCntTxt.setFillColor(sf::Color::White);
	pieceCntTxt.setPosition(10, 800);

	redClockTxt.setFillColor(sf::Color::Red);
	redClockTxt.setPosition(820, 725);

	whiteClockTxt.setFillColor(sf::Color::White);
	whiteClockTxt.setPosition(820, 25);

	redClock.DisplayTime(redClockTxt); //display red at beginning -> turn != red

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		if (!board.hasPlayerLost(turn) && movesAvailable && !whiteClock.isTimeFinished() && !redClock.isTimeFinished()) {
			if (event.type == sf::Event::MouseButtonPressed) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
					if (mousePosition.x >= 0 && mousePosition.x < 800 && mousePosition.y >= 0 && mousePosition.y < 800){
						col = mousePosition.x / 100;
						row = mousePosition.y / 100;
					}
					else{
						continue;
					}
					std::cout << "Clicked at R " << row << " C " << col << std::endl;

					isPieceAtTargetPos = board.getBoard(row, col);
					if (selectedPiece && shapeSelected) {
						sf::Color DarkGray(99, 102, 106);
						if (isPieceAtTargetPos) {
							if (otherColor == "DarkGray") {

								if (selectedPiece->isOpponent(isPieceAtTargetPos)) {
									std::cout << "OCCUPIED BY OPPOSITE PIECE\n";
								}
								else {
									std::cout << "OCUPPIED BY SAME PIECE\n";
								}

								selectedPiece->setOutlineColor(DarkGray);
							}
							else {
								selectedPiece->setOutlineColor(sf::Color::Blue);
							}

							shapeSelected = false;
						}
						else {
							selectedPiece->selectPiece(shapeSelected, board, jumpsAvailable);
							if (selectedPiece->getTurnEnd()) {
								std::cout << "Moved to R " << row << " C " << col << std::endl;

								board.setBoard(row, col, selectedPiece);
								board.setBoard(startRow, startCol, nullptr);
								selectedPiece->resetJumpMoves();

								selectedPiece = nullptr;

								// reset attacks and change turn
								board.resetAttackOutline();
								board.changeTurn(turn);

							//	board.rotateBoard();
							//	view.rotate(180);
							//	whiteClockTxt.rotate(180);
							//	redClockTxt.rotate(180);

								//board.resetJumpMoves();

								// find other color attacks
								jumpsAvailable = board.findAttacks(turn/*, window*/);

								//regular moves
								if (!jumpsAvailable) {
									movesAvailable = board.movesAvailable(turn);
								}
								
								//count pieces and game conditions
								board.countPieces(pieceCntTxt);
								
								if (turn == "Red") {
									redClock.startTimer();
									whiteClock.stopTimer();
								}
								else {
									whiteClock.startTimer();
									redClock.stopTimer();
								}


								//std::cout << turn << std::endl;
							}
						}
					}
					else {
						selectedPiece = board.getBoard(row, col);

						if (selectedPiece && selectedPiece->getColorString() == turn) {
							selectedPiece->selectPiece(shapeSelected, board, jumpsAvailable);
							startRow = row;
							startCol = col;
							std::cout << "Selected piece at R " << row << " C " << col << std::endl;
						}
						else if (selectedPiece && selectedPiece->getColorString() != turn) std::cout << "WRONG TURN\n";
						else std::cout << "NO PIECE AT R " << row << " C " << col << std::endl;

						if (selectedPiece != nullptr) {
							otherColor = selectedPiece->getOutlineColorString();
						}

					}

					sf::sleep(sf::milliseconds(100));

					/*
					while (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

					}
					*/
				}
			}
		}
		else {
			// DO END MENU
			if (board.hasPlayerLost(turn)) std::cout << turn << " LOST -- all pieces were taken\n";
			else if (!movesAvailable) std::cout << turn << " LOST -- no moves available\n";
			else if (whiteClock.isTimeFinished()) {
				std::cout << "White Ran Out Of Time\n";
			}
			else {
				std::cout << "Red Ran Out Of Time\n";
			}
			whiteClock.stopTimer();
			redClock.stopTimer();
			//break;
		}

		//display
		window.clear();
		board.drawBoard();
		if (shapeSelected) board.drawMoves(selectedPiece->getColorString(), row, col);
		board.draw();

		window.draw(pieceCntTxt);

		(turn == "White") ? whiteClock.DisplayTime(whiteClockTxt) : redClock.DisplayTime(redClockTxt);
		window.draw(redClockTxt);
		window.draw(whiteClockTxt);

		window.display();

		//keybinds
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
			window.close();
		}

	}

	return 0;
}