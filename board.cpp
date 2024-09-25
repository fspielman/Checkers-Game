#include <SFML/Graphics.hpp>
#include <iostream>
#include "board.hpp"

#define boardSize 8
#define tile 100
#define quarterTile 25
#define DarkGray sf::Color(99, 102, 106)

Board::Board(sf::RenderWindow& win) : window(win) {
	multiJump = false;
}

void Board::initBoard() {
	board = std::vector<std::vector<std::shared_ptr<Piece>>>(8, std::vector<std::shared_ptr<Piece>>(8, nullptr));

/*	board[4][1] = std::make_shared<Piece>(Piece("Red", 4, 1));
	board[2][3] = std::make_shared<Piece>(Piece("White", 2, 3));
	board[1][2] = std::make_shared<Piece>(Piece("White", 1, 2));
	board[1][4] = std::make_shared<Piece>(Piece("White", 1, 4));*/
	/*board[1][2] = std::make_shared<Piece>(Piece("White", 1, 2));
	board[1][4] = std::make_shared<Piece>(Piece("White", 1, 4));
	board[2][3] = std::make_shared<Piece>(Piece("White", 2, 3));
	//board[0][3] = std::make_shared<Piece>(Piece("White", 0, 3));
	board[0][3] = std::make_shared<Piece>(Piece("White", 0, 3));
	board[0][5] = std::make_shared<Piece>(Piece("White", 0, 5));
	board[1][0] = std::make_shared<Piece>(Piece("White", 1, 0));
	board[4][1] = std::make_shared<Piece>(Piece("Red", 4, 1));
	board[4][3] = std::make_shared<Piece>(Piece("Red", 4, 3));//*/
	///*
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
//	*/
	available = true;
}

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
	//board[startRow][startCol]->getVisitedSquares();
	auto jumpSquare = piece->getVisitedSquares();
	int size = piece->getVisitedSquares().size();
	
	//std::vector<std::vector<std::pair<int, int>>> longestPath;
	//int cnt = 0;

	if (multiJump) {
		std::cout << "LONGEST PATH:::::::::::::\n";

		for (const auto& p : piece->getLongestPath()) {
			//std::cout << "PATHS - " << cnt << std::endl;
			for (const auto& path : p) {
				std::cout << path.first << " " << path.second << "\t";
			}
			std::cout << std::endl;
		}

		for (const auto& p : piece->getLongestPath()) {
			for (int i = 1; i < p.size(); ++i) {	// skip piece's initial square
				this->testing(p[i].first, p[i].second);
			}
		}

		/*for (int i = 0; i < longestPath.size(); ++i) {
			for (int j = 1; j < path.size(); ++j) {
				this->testing(longestPath[i][path[j].first], longestPath[i][path[j].second]);
			//	std::cout << "DRAW PURPLE SQUARES AT " << path[j].first << " " << path[j].second << std::endl;
			}
		}*/

		for (const auto& p : piece->getLongestPath()) {
			for (const auto& path : p) {
				this->drawingMove(p.back().first, p.back().second);
			}
		}
/*
		for (int i = 1; i < size; ++i) {
			this->testing(jumpSquare[i].first, jumpSquare[i].second);
			//std::cout << visitedSquares[i].first << " NIOSGDD " << visitedSquares[i].second << std::endl;
		}
		this->drawingMove(jumpSquare[size - 1].first, jumpSquare[size - 1].second);
*/
		

	}
	else if (board[startRow][startCol]->getOutlineColorString() == "Blue") {
		this->drawSingleJump(color, startRow, startCol);
	}
/*	else if (board[startRow][startCol]->getisKing()) {	
	}*/
	else {
		this->drawSingleMoves(color, startRow, startCol);
	}
}

bool Board::isWithinBounds(int row, int col) {
    return row >= 0 && row < boardSize && col >= 0 && col < boardSize;
}

void Board::drawSingleJump(const std::string& color, int startRow, int startCol) {
	std::shared_ptr<Piece> piece = board[startRow][startCol];

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
	std::vector < std::pair<int, int>> directions;
	/*
	if (board[startRow][startCol]->getisKing()) {
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
	*/

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
		//std::cout << "YO DREW AT " << currentRow << " " << currentCol << '\n';
	}
}

void Board::draw() {
	for (int row = 0; row < boardSize; ++row) {
		for (int col = 0; col < boardSize; ++col) {
			if (board[row][col] != nullptr) {
				board[row][col]->draw(window);
			}
		}
	}
}

std::shared_ptr<Piece> Board::getBoard(int row, int col) { return board[row][col]; }

void Board::setBoard(int row, int col, std::shared_ptr<Piece> piece) { board[row][col] = piece; }

void Board::changeTurn(std::string& turn) {
	if (turn == "White") {
		turn = "Red";
		std::cout << "RED TURN\n";
	}
	else {
		turn = "White";
		std::cout << "WHITE TURN\n";
	}
}

// need it to get place to attack to 
bool Board::findAttacks(const std::string& turn) {
	found = false;

	std::vector<std::vector<std::pair<int, int>>> longestAttack;  
	int maxPathLength = 0;  

	std::vector<std::pair<int, int>> attacksFound;
	std::vector<std::pair<int, int>> singleAttacks;

	for (int row = 0; row < boardSize; ++row) {
		for (int col = 0; col < boardSize; ++col) {
			if (board[row][col] != nullptr && board[row][col]->getColorString() == turn) {
				if (board[row][col]->jumpDetected(row, col, *this/*, visitedSquares*/)) {
					if (!(board[row][col]->getMultiJumpPiece())) { 
						singleAttacks.push_back({ row,col });
						std::cout << "TRUE\n";
					}
					//board[row][col]->setOutlineColor(sf::Color::Blue);
					//attacksFound.push_back({ row, col });

					std::vector<std::pair<int, int>> path;
					std::shared_ptr<Piece> piece = board[row][col];
					piece->findLongestPath(board[row][col]->getVisitedSquares()[0], path,/* longestPath,*/ *this);

					std::vector<std::vector<std::pair<int, int>>> currentPath = piece->getLongestPath();
					int currentPathLength = piece->getLongestPath()[0].size();

					std::cout << "PATH SIZE----" << piece->getLongestPath()[0].size() << std::endl;
					if (currentPathLength > maxPathLength) {
						maxPathLength = currentPathLength;
						longestAttack = currentPath;
                    }
					else if (currentPathLength == maxPathLength) {
						longestAttack.insert(longestAttack.end(), currentPath.begin(), currentPath.end());
					}

					found = true;
				}
				/*else {
					std::cout << "BOARD[" << row << "][" << col << "] NULL\n";
				}*/
			}
		}
	}

	if (this->getMultiJump()) {
		std::cout << "CHECKING MATEY\n";
		for (const auto& attack : singleAttacks) {
			board[attack.first][attack.second]->setOutlineColor(DarkGray);
			std::cout << "BOARD[" << attack.first << "][" << attack.second << "] SET TO GRAY \n";
		}
	}
	if (found) {
		std::cout << "Longest attack path has length: " << maxPathLength << std::endl;
		for (const auto& pos : longestAttack) {
			//for (int i = 0; i < pos.size(); ++i) {
			std::cout << "Longest attack at ROW: " << pos[0].first << " COL: " << pos[0].second << std::endl;
			if (board[pos[0].first][pos[0].second])
				board[pos[0].first][pos[0].second]->setOutlineColor(sf::Color::Blue);
			//}
		}
	}

/*
	for (const auto& attacks : attacksFound) {
		std::cout << "ATTACKS AT ROW: " << attacks.first << " COL: " << attacks.second << std::endl;
	}
*/
	return found;
}

void Board::resetAttackOutline() {
	for (int row = 0; row < boardSize; ++row) {
		for (int col = 0; col < boardSize; ++col) {
			if (board[row][col] != nullptr && board[row][col]->getOutlineColor() == sf::Color::Blue) {
				board[row][col]->setOutlineColor(DarkGray);
			}
		}
	}
}

void Board::drawingMove(const int& row, const int& col) {
	sf::CircleShape moveCircle(25);
	moveCircle.setFillColor(sf::Color::Green);
	moveCircle.setPosition(col * tile + quarterTile, row * tile + quarterTile);
	window.draw(moveCircle);
}

void Board::testing(const int& row, const int& col) {
	sf::CircleShape moveCircle(25);
	moveCircle.setFillColor(sf::Color::Magenta);
	moveCircle.setPosition(col * tile + quarterTile, row * tile + quarterTile);
	window.draw(moveCircle);
}

sf::RenderWindow& Board::getWindow() const { return window; }

/*
void Board::resetJumpMoves() {
	if (!visitedSquares.empty()) {
		visitedSquares.clear();
	}
}

std::vector<std::pair<int, int>> Board::getVisitedSquares() const { return visitedSquares; }

*/

void Board::setMultiJump(bool boolean) { 
	if (boolean == true) {
		multiJump = true;
	}
	else {
		multiJump = false;
	}
}

bool Board::getMultiJump() const { return multiJump; }

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

	//show when one side of pieces taken
	if (pieceCnt["Red"] < 12 || pieceCnt["White"] < 12) {
		txt.setString("PIECES CAPTURED BY:\t\t RED - " + std::to_string(12 - pieceCnt["White"]) + "\t\t White - " + std::to_string(12 - pieceCnt["Red"]));
	}

	/*if (pieceCnt["Red"] == 0) {
		std::cout << "RED LOST\n";
		return false;
	}
	else if (pieceCnt["White"] == 0) {
		std::cout << "WHITE LOST\n";
		return false;
	}

	return true;*/
}

bool Board::hasPlayerLost(const std::string& turn) const {
	auto itr = pieceCnt.find(turn);
	if (itr != pieceCnt.end()) {
		if (itr->second == 0) {
			//std::cout << turn << " LOST\n";
			return true;
		}
	}
	return false;
}

bool Board::movesAvailable(const std::string& turn) {
	available = false;
	std::vector<std::pair<int, int>> directions;

	if (found) {
		//std::cout << "T\n";
		return true;
	}
	for (int row = 0; row < boardSize; ++row) {
		for (int col = 0; col < boardSize; ++col) {
			if (board[row][col] != nullptr) {
				// make into function
				if (board[row][col]->getisKing() && board[row][col]->getColorString() == turn) {
					directions.push_back({ row + 1,col + 1 });
					directions.push_back({ row + 1,col - 1 });
					directions.push_back({ row - 1,col + 1 });
					directions.push_back({ row - 1,col - 1 });

					for (const auto& direction : directions) {
						int Row = direction.first;
						int Col = direction.second;

						if (this->isWithinBounds(Row, Col) && board[Row][Col] == nullptr) {
							std::cout << "TRUE AT " << row << " " << col << "\n";
							return true;
						}
					}
				}
				else if (turn == "White" && board[row][col]->getColorString() == "White") {
					directions.push_back({ row + 1,col + 1 });
					directions.push_back({ row + 1,col - 1 });

					for (const auto& direction : directions) {
						int Row = direction.first;
						int Col = direction.second;

						if (this->isWithinBounds(Row, Col) && board[Row][Col] == nullptr) {
							std::cout << "TRUE for " << row << " " << col << "\n";
							return true;
						}
					}
				}
				else if (turn == "Red" && board[row][col]->getColorString() == "Red") {
					directions.push_back({ row - 1,col + 1 });
					directions.push_back({ row - 1,col - 1 });

					for (const auto& direction : directions) {
						int Row = direction.first;
						int Col = direction.second;

						if (this->isWithinBounds(Row, Col) && board[Row][Col] == nullptr) {
							std::cout << "TRUE\n";
							return true;
						}
					}
				}
			}
		}
	}
	std::cout << "NO AVAILABLE MOVES\n";
	return available;
}

void Board::populateDirections(const std::string& color, const int& row, const int& col, std::vector<std::pair<int, int>>& directions) {
//	std::vector < std::pair<int, int>> directions;
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

bool Board::getAvailable() const { return available; }

void Board::rotateBoard() {
	std::reverse(board.begin(), board.end());
	sf::View view = window.getView();
	view.rotate(180);
	std::cout << "ROTATED\n";
}
