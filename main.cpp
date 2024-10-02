#include <SFML/Graphics.hpp>
#include <iostream>
#include "board.hpp"
#include "timer.hpp"

int main()
{
	bool isFullscreen = false;

	sf::RenderWindow window(sf::VideoMode(900, 850), "Checkers", sf::Style::Titlebar | sf::Style::Close);
	sf::View view(sf::FloatRect(0, 0, 900, 850)); //in progress for rotating board

	//initializing board
	Board board(window);
	board.initBoard();

	//setting turn to white
	std::string turn = "White";

	//no piece is selected at beginning
	bool shapeSelected = false;
	//moves are available at beginning
	bool movesAvailable = true;
	
	//initiating positions used
	int startRow = -1;
	int startCol = -1;
	int row;
	int col;

	std::shared_ptr<Piece> selectedPiece = nullptr;
	std::shared_ptr<Piece> isPieceAtTargetPos = nullptr;
	bool jumpsAvailable = false;
	std::string colorOfSelectedPiece;

	//handling text for timers and piece counter
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

	redClock.DisplayTime(redClockTxt); //display red timer at the beginning -> turn != red


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		//checking lose conditions
		if (!board.hasPlayerLost(turn) && movesAvailable && !whiteClock.isTimeFinished() && !redClock.isTimeFinished()) {
			//if the left mouse button is pressed
			if (event.type == sf::Event::MouseButtonPressed) {
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
					//if user left clicked on the board
					sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
					//board is 800x800 - 100x100 per tile
					if (mousePosition.x >= 0 && mousePosition.x < 800 && mousePosition.y >= 0 && mousePosition.y < 800){
						col = mousePosition.x / 100;
						row = mousePosition.y / 100;
					}
					//if user clicked outside of the board -> continue (no selections can be made)
					else{
						continue;
					}

					//pointer to check if there is piece at the end position of move -> used after piece is selected
					isPieceAtTargetPos = board.getBoard(row, col);
					//is there a piece selected
					if (selectedPiece && shapeSelected) {
						sf::Color DarkGray(99, 102, 106);
						//is there a piece at the end position where the user tried to move to
						if (isPieceAtTargetPos) {
							//reseting outline of selected piece 
							if (colorOfSelectedPiece == "DarkGray") {
								selectedPiece->setOutlineColor(DarkGray);
							}
							else {
								selectedPiece->setOutlineColor(sf::Color::Blue);
							}

							shapeSelected = false;
						}
						else {
							selectedPiece->selectPiece(shapeSelected, board, jumpsAvailable);
							//if turnEnd=true then move is valid
							if (selectedPiece->getTurnEnd()) {
								//update piece position and set its previous position to null
								board.setBoard(row, col, selectedPiece);
								board.setBoard(startRow, startCol, nullptr);

								//reset multijump pathing 
								selectedPiece->resetJumpMoves();

								selectedPiece = nullptr;

								//reset attack outlines and change turn
								board.resetAttackOutline();
								board.changeTurn(turn);

								/****** in progress for rotating board  *******/
								//board.rotateBoard();
								//view.rotate(180);
								//whiteClockTxt.rotate(180);
								//redClockTxt.rotate(180);

								// find other color's attacks
								jumpsAvailable = board.findAttacks(turn);

								//check if there are regular moves avaiable if no jump moves are found
								if (!jumpsAvailable) {
									movesAvailable = board.movesAvailable(turn);
								}
								
								//count pieces
								board.countPieces(pieceCntTxt);
								
								//starting and stopping timers based on the turn
								if (turn == "Red") {
									redClock.startTimer();
									whiteClock.stopTimer();
								}
								else {
									whiteClock.startTimer();
									redClock.stopTimer();
								}
							}
						}
					}
					else {
						//get selected piece from where the user clicked mouse
						selectedPiece = board.getBoard(row, col);
						//if selectedPiece is not null and the selected piece corresponds to the correct turn
						if (selectedPiece && selectedPiece->getColorString() == turn) {
							//beginning of selected piece, changes outline to yellow and sets shapeSelected to true
							selectedPiece->selectPiece(shapeSelected, board, jumpsAvailable);
							startRow = row;
							startCol = col;

							//if correct piece is selected, get the its outline color->function only returns blue and dark gray(not yellow)
							//used to reset color when piece is deselected or wrong move is made
							colorOfSelectedPiece = selectedPiece->getOutlineColorString();
						}

					}

					sf::sleep(sf::milliseconds(100));

				}
			}
		}
		//one of the players lost
		else {
			/******************** Implement end menu here (Work in progress) **********************/

			//display losing conditions in the terminal/console window
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
		}

		//display
		window.clear();
		board.drawBoard();
		//draw moves for the selected shape
		if (shapeSelected) board.drawMoves(selectedPiece->getColorString(), row, col);
		board.draw();

		window.draw(pieceCntTxt);
		//display timer for current color's turn -> other timer is not changing
		(turn == "White") ? whiteClock.DisplayTime(whiteClockTxt) : redClock.DisplayTime(redClockTxt);
		window.draw(redClockTxt);
		window.draw(whiteClockTxt);

		window.display();

		//press q to quit the game
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
			window.close();
		}

	}

	return 0;
}