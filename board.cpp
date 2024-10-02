#include <SFML/Graphics.hpp>
#include <iostream>
#include "board.hpp"

#define boardSize 8
#define tile 100
#define quarterTile 25
#define DarkGray sf::Color(99, 102, 106)

//getters
sf::RenderWindow& Board::getWindow() const { return window; }

bool Board::getMultiJump() const { return multiJump; }

std::shared_ptr<Piece> Board::getBoard(int row, int col) { return board[row][col]; }

//setters
void Board::setMultiJump(bool boolean) { 
	if (boolean == true) {
		multiJump = true;
	}
	else {
		multiJump = false;
	}
}

void Board::setBoard(int row, int col, std::shared_ptr<Piece> piece) { board[row][col] = piece; }

//methods
Board::Board(sf::RenderWindow& win) : window(win) {
	multiJump = false;
}

//initialize starting piece positions to board
void Board::initBoard() {
	for (int row = 0; row < boardSize; ++row) {
		for (int col = 0; col < boardSize; ++col) {
			if (row < 3 && ((row + col) % 2 == 1)) {
				board[row][col] = std::make_shared<Piece>(Piece("White", row, col));
			}
		}
	}

	for (int row = 0; row < boardSize; ++row) {
		for (int col = 0; col < boardSize; ++col) {
			if (row > 4 && ((row + col) % 2 == 1)) {
				board[row][col] = std::make_shared<Piece>(Piece("Red", row, col));
			}
		}
	}
}

//drawing tiles to screen
void Board::drawBoard() {
	int squareSize = 100;
	sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
	for (int row = 0; row < boardSize; ++row) {
		for (int col = 0; col < boardSize; ++col) {
			if ((row + col) % 2 == 0) {
				square.setFillColor(sf::Color::Red);
			}
			else {
				square.setFillColor(sf::Color::Black);
			}
			square.setPosition(row * squareSize, col * squareSize);
			window.draw(square);
		}
	}
}

void Board::drawMoves(const std::string& color, const int& startRow, const int& startCol) {
	std::shared_ptr<Piece> piece = board[startRow][startCol];
	auto jumpSquare = piece->getVisitedSquares();
	int size = piece->getVisitedSquares().size();

	if (multiJump) {
		//draws purple circles to show attack path (not on final attack positions) 
		for (const auto& p : piece->getLongestPath()) {
			for (int i = 1; i < p.size(); ++i) {	// skip piece's initial square -> don't display purple circle on coordinate of selected piece
				this->drawMultiPath(p[i].first, p[i].second);
			}
		}
		
		//draws green circles for final attack positions
		for (const auto& p : piece->getLongestPath()) {
			this->drawingMove(p.back().first, p.back().second);
		}
	}
	else if (board[startRow][startCol]->getOutlineColor() == sf::Color::Blue) {
		this->drawSingleJump(color, startRow, startCol);
	}
	else {
		this->drawSingleMoves(color, startRow, startCol);
	}
}

bool Board::isWithinBounds(int row, int col) {
    return row >= 0 && row < boardSize && col >= 0 && col < boardSize;
}

void Board::drawSingleJump(const std::string& color, int startRow, int startCol) {
	std::shared_ptr<Piece> piece = board[startRow][startCol]; //selected piece

	std::vector<std::pair<int, int>> directions = {
		{1, 1}, {1, -1}, {-1, 1}, {-1, -1}
	};

	for (const auto& direction : directions) {
		int row = startRow + direction.first;
		int col = startCol + direction.second;

		if (isWithinBounds(row, col)) {
			if (board[row][col] && board[row][col]->isOpponent(piece)) {
				int jumpRow = row + direction.first;
				int jumpCol = col + direction.second;

				if (isWithinBounds(jumpRow, jumpCol) && !board[jumpRow][jumpCol]) {
					this->drawingMove(jumpRow, jumpCol);
				}
			}
		}
	}
}

void Board::drawSingleMoves(const std::string& color, const int startRow, const int& startCol) {
	std::vector<std::pair<int, int>> directions;
	
	this->populateDirections(color, startRow, startCol, directions);

	for (int i = 0; i < directions.size(); ++i) {
		int currentRow = startRow;
		int currentCol = startCol;

		currentRow += directions[i].first;
		currentCol += directions[i].second;

		if (!this->isWithinBounds(currentRow, currentCol) || (this->getBoard(currentRow, currentCol) != nullptr)) {
			continue;
		}

		this->drawingMove(currentRow, currentCol);
	}
}

//drawing pieces to board
void Board::draw() {
	for (int row = 0; row < boardSize; ++row) {
		for (int col = 0; col < boardSize; ++col) {
			if (board[row][col] != nullptr) {
				board[row][col]->draw(window);
			}
		}
	}
}

void Board::changeTurn(std::string& turn) {
	if (turn == "White") {
		turn = "Red";
	}
	else {
		turn = "White";
	}
}

bool Board::findAttacks(const std::string& turn) {
	isAttackFound = false;

	std::vector<std::vector<std::pair<int, int>>> longestAttack;  
	int maxPathLength = 0;  
	
	//keep track of attacks found
	std::vector<std::pair<int, int>> attacksFound;
	//keep track of single attacks
	std::vector<std::pair<int, int>> singleAttacks;

	for (int row = 0; row < boardSize; ++row) {
		for (int col = 0; col < boardSize; ++col) {
			if (board[row][col] != nullptr && board[row][col]->getColorString() == turn) {
				//if jump found for piece
				if (board[row][col]->jumpDetected(row, col, *this)) {
					if (!(board[row][col]->getMultiJumpPiece())) { 
						singleAttacks.push_back({ row,col }); //if multijump is not found for piece add it to single attack vector
					}
					//multi-jump found -> find longest path
					std::vector<std::pair<int, int>> path;
					std::shared_ptr<Piece> piece = board[row][col];
					piece->findLongestPath(board[row][col]->getVisitedSquares()[0], path, *this);

					std::vector<std::vector<std::pair<int, int>>> currentPath = piece->getLongestPath();
					int currentPathLength = piece->getLongestPath()[0].size();

					std::cout << "PATH SIZE----" << currentPathLength << std::endl;
					//if current multi-jump path length is greater than the current maximum multi-jump path length
					if (currentPathLength > maxPathLength) {
						maxPathLength = currentPathLength;
						longestAttack = currentPath;
                    }
					//if current path length = max path length -> add it to longest attack vector -> multiple multi-jumps for piece
					else if (currentPathLength == maxPathLength) {
						longestAttack.insert(longestAttack.end(), currentPath.begin(), currentPath.end());
					}

					isAttackFound = true;
				}
			}
		}
	}

	//setting single jump outline to gray if multijump is found
	if (this->getMultiJump()) {
		for (const auto& attack : singleAttacks) {
			board[attack.first][attack.second]->setOutlineColor(DarkGray);
		}
	}

	//setting the outline of the checker with the longest attack path to blue
	if (isAttackFound) {
		for (const auto& pos : longestAttack) {
			if (board[pos[0].first][pos[0].second])
				board[pos[0].first][pos[0].second]->setOutlineColor(sf::Color::Blue);
		}
	}

	return isAttackFound;
}

//resetting blue attack outline after player attacks and next turn is started
void Board::resetAttackOutline() {
	for (int row = 0; row < boardSize; ++row) {
		for (int col = 0; col < boardSize; ++col) {
			if (board[row][col] != nullptr && board[row][col]->getOutlineColor() == sf::Color::Blue) {
				board[row][col]->setOutlineColor(DarkGray);
			}
		}
	}
}

//drawing green circles -> end positions
void Board::drawingMove(const int& row, const int& col) {
	sf::CircleShape moveCircle(25);
	moveCircle.setFillColor(sf::Color::Green);
	moveCircle.setPosition(col * tile + quarterTile, row * tile + quarterTile);
	window.draw(moveCircle);
}

//drawing purple circles -> multi-jump path taken to get to possible end position
void Board::drawMultiPath(const int& row, const int& col) {
	sf::CircleShape moveCircle(25);
	moveCircle.setFillColor(sf::Color::Magenta);
	moveCircle.setPosition(col * tile + quarterTile, row * tile + quarterTile);
	window.draw(moveCircle);
}

//counting pieces to display the number of pieces taken to window
void Board::countPieces(sf::Text& txt) {
	pieceCnt["Red"] = 0;
    pieceCnt["White"] = 0;

	for (int row = 0; row < boardSize; ++row) {
		for (int col = 0; col < boardSize; ++col) {
			if (board[row][col]) {
				pieceCnt[board[row][col]->getColorString()]++;
			}
		}
	}
	
	//12 is the total number of checkers pieces
	if (pieceCnt["Red"] < 12 || pieceCnt["White"] < 12) {
		txt.setString("PIECES CAPTURED BY:\t\t RED - " + std::to_string(12 - pieceCnt["White"]) + "\t\t White - " + std::to_string(12 - pieceCnt["Red"]));
	}
}

//check if current player doesn't have any pieces left
bool Board::hasPlayerLost(const std::string& turn) const {
	auto itr = pieceCnt.find(turn);
	if (itr != pieceCnt.end()) {
		if (itr->second == 0) {
			return true;
		}
	}
	return false;
}

//check if current player has moves available
bool Board::movesAvailable(const std::string& turn) {
	areMovesAvailable = false;
	std::vector<std::pair<int, int>> directions;

	if (isAttackFound) {
		return true;
	}
	//checking for single moves since attacks weren't found 
	for (int row = 0; row < boardSize; ++row) {
		for (int col = 0; col < boardSize; ++col) {
			if (board[row][col] != nullptr && board[row][col]->getColorString() == turn) {
				directions.clear();
				populateDirections(turn, row, col, directions);

				for (const auto& direction : directions) {
					int Row = row + direction.first;
					int Col = col + direction.second;

					if (this->isWithinBounds(Row, Col) && board[Row][Col] == nullptr) {
						return true;
					}
				}
			}
		}
	}
	std::cout << "NO AVAILABLE MOVES\n";
	return areMovesAvailable;
}

//find possible directions based on isKing or color
void Board::populateDirections(const std::string& color, const int& row, const int& col, std::vector<std::pair<int, int>>& directions) {
	if (board[row][col]->getisKing()) {
		directions.push_back({ 1, -1 });
		directions.push_back({ 1, 1 });
		directions.push_back({ -1, -1 });
		directions.push_back({ -1, 1 });
	}
	else if (color == "White") {
		directions.push_back({ 1, -1 });
		directions.push_back({ 1, 1 });
	}
	else {
		directions.push_back({ -1, -1 });
		directions.push_back({ -1, 1 });
	}
}

//work in progress -> rotating board every turn
void Board::rotateBoard() {
	std::reverse(board.begin(), board.end());
	sf::View view = window.getView();
	view.rotate(180);
	std::cout << "ROTATED\n";
}
