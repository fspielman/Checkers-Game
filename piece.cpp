#include <iostream>
#include "board.hpp"
#include <set>

#define tile 100
#define halfTile 50
#define DarkGray sf::Color(99, 102, 106)

sf::Color Piece::getColor(const std::string& Color) {
	if (Color == "White") return sf::Color::White;
	return sf::Color::Red;
}

std::string Piece::getColorString() const { return color; }

Piece::Piece(std::string Color, int row, int col) : color(Color), isKing(false) {
	bool shapeSelected = false;
	checker.setRadius(35.f);
	checker.setPosition(col * 100 + 50, row * 100 + 50);
	checker.setFillColor(getColor(Color));
	checker.setOutlineThickness(5.f);
	checker.setOutlineColor(DarkGray);
	checker.setOrigin(35.f, 35.f);
}

void Piece::makeKing() {
	isKing = true;
	crownTexture.loadFromFile("images\\king.png");
	checker.setTexture(&crownTexture);
	std::cout << color << " KING\n";
}

bool Piece::isValidMove(const int& startRow, const int& startCol, const int& endRow, const int& endCol) const {
	if (isKing) 
		return (endRow == startRow + 1 || endRow == startRow - 1) && (endCol == startCol - 1 || endCol == startCol + 1); 
	if (color == "White") 
		return (endRow == startRow + 1) && (endCol == startCol - 1 || endCol == startCol + 1);
	else 
		return (endRow == startRow - 1) && (endCol == startCol - 1 || endCol == startCol + 1);
}

bool Piece::isOpponent(const std::shared_ptr<Piece>& piece) const {
	if (!piece) 
		return false;
	else
		return this->getColorString() != piece->getColorString();
}

bool Piece::jumpDetected(const int& currentRow, const int& currentCol, Board& board/*, std::vector<std::pair<int, int>>& visitedSquares*/){
    std::vector<std::pair<int, int>> possibleJumps = 
	{
		{currentCol + 2, currentRow + 2}, {currentCol - 2, currentRow + 2},
		{currentCol + 2, currentRow - 2}, {currentCol - 2, currentRow - 2}
    };
	
	bool foundJump = false;
	multiJumpPiece = false;

	for (const auto& jump : possibleJumps) {
		int endCol = jump.first;
		int endRow = jump.second;
		if (board.isWithinBounds(endRow,endCol)) {

			if (this->isValidAttack(currentRow, currentCol, endRow, endCol, board)) {
				if (std::find(visitedSquares.begin(), visitedSquares.end(), std::make_pair(currentRow, currentCol)) == visitedSquares.end()) {
					visitedSquares.push_back({ currentRow, currentCol });
				}
				if (std::find(visitedSquares.begin(), visitedSquares.end(), std::make_pair(endRow, endCol)) == visitedSquares.end()
					&& this->jumpDetected(endRow, endCol, board)) {
					board.setMultiJump(true);
					this->setMultiJumpPiece(true);

					for (const auto& visit : visitedSquares) {
						std::cout << "VISITED --- " << visit.first << " " << visit.second << std::endl;
					}

					std::cout << "ATTACK ENDS AT --- ROW: " << endRow << " Col: " << endCol << std::endl;
				}

				foundJump = true;
			}
		}
	}

	return foundJump;
}

void Piece::findLongestPath(const std::pair<int, int>& current, std::vector<std::pair<int, int>>& path, Board& board) {
    path.push_back(current);

    // Define possible attack moves
    std::vector<std::pair<int, int>> directions = {
        {2, 2}, {-2, 2}, {2, -2}, {-2, -2}
    };

    bool morePaths = false;

    for (const auto& direction : directions) {
        std::pair<int, int> nextPos = {current.first + direction.first, current.second + direction.second};

        if (std::find(visitedSquares.begin(), visitedSquares.end(), nextPos) != visitedSquares.end() &&
            std::find(path.begin(), path.end(), nextPos) == path.end()
			&& this->isValidAttack(current.first, current.second, nextPos.first, nextPos.second, board)) {
            morePaths = true;
            findLongestPath(nextPos, path, board);
        }
    }

    if (!morePaths) {
		if (longestPath.empty()) {
            longestPath.push_back(path);
		}
		else if (path.size() == longestPath[0].size()) {
			longestPath.push_back(path);
		}
        else if (path.size() > longestPath[0].size()) {
			longestPath.clear();
            longestPath.push_back(path);
        }
    }

    path.pop_back();
}

std::vector < std::vector<std::pair<int, int>>> Piece::getLongestPath() const { return longestPath; }

bool Piece::isValidAttack(const int& startRow, const int& startCol, const int& endRow, const int& endCol, Board board) const {
	if (!(abs(endCol - startCol) == 2) || !(abs(endRow - startRow) == 2)) return false;

	int midCol = (startCol + endCol) / 2;
	int midRow = (startRow + endRow) / 2;

	auto middlePiece = board.getBoard(midRow, midCol);

	auto endPoint = board.getBoard(endRow, endCol);

	return !endPoint &&  this->isOpponent(middlePiece);
}

void Piece::attack(const int& startRow, const int& startCol, const int& endRow, const int& endCol, Board& board) {
	turnEnd = true;
	checker.setPosition(endCol * tile + halfTile, endRow * tile + halfTile);
	board.setBoard((startRow + endRow) / 2, (startCol + endCol) / 2, nullptr);
	if ((color == "White" && endRow == 7) || (color == "Red" && endRow == 0) && !isKing) {
		this->makeKing();
	}
}

void Piece::move(const int& startRow, const int& startCol, int endRow, int endCol) {
	if (isValidMove(startRow, startCol, endRow, endCol)) {
        checker.setPosition(endCol * tile + halfTile, endRow * tile + halfTile);
		turnEnd = true;
        //std::cout << color << " MOVED to - x " << endX << " y " << endY << std::endl;
        if ((color == "White" && endRow == 7) || (color == "Red" && endRow == 0) && !isKing) {
			this->makeKing();
        }
    }
	else {
		std::cout << endRow << " " << endCol << std::endl;
        std::cout << "NOT VALID MOVE, SELECT DIFFERENT SPOT\n";
		turnEnd = false;
    }
}

// if two moves occur -> one as attack and one as multi -> both do same move 
void Piece::multiJump(const int& startRow, const int& startCol, const int& endRow, const int& endCol, Board& board, std::pair<int,int> endPos) {
	turnEnd = true;
	checker.setPosition(endCol * tile + halfTile, endRow * tile + halfTile);
	std::vector<std::pair<int, int>> chosenPath;

	for (const auto& path : longestPath) {
		if (std::find(path.begin(), path.end(), endPos) != path.end()) {
			chosenPath = path;
			break;
		}
	}
	
	for (int i = 0; i < chosenPath.size() - 1; ++i) {
		int midRow = (chosenPath[i].first + chosenPath[i + 1].first) / 2;
		int midCol = (chosenPath[i].second+ chosenPath[i + 1].second) / 2;
		board.setBoard(midRow, midCol, nullptr); //remove jumped pieces
	}

	if ((color == "White" && endRow == 7) || (color == "Red" && endRow == 0) && !isKing) {
		this->makeKing();
	}
}

sf::CircleShape Piece::getChecker() const { return checker; }

void Piece::selectPiece(bool& shapeSelected, Board& board, bool attackFound) {
	sf::Vector2i mousePosition = sf::Mouse::getPosition(board.getWindow());
	sf::Vector2f mousePos_f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

	bool isLongestAttackFound = false;
	std::vector<std::pair<int, int>> pathPoints;

	if (!shapeSelected) {
		if (attackFound && !(this->getOutlineColorString() == "Blue")) {
			shapeSelected = false;
		}
		else {
			shapeSelected = true;
			checker.setOutlineColor(sf::Color::Yellow);
		}
		isLongestAttackFound = false;
	}
	else {

		int startCol = (this->getChecker().getPosition().x) / tile;
		int startRow = (this->getChecker().getPosition().y) / tile;
		std::cout << "START - ROW: " << startRow << " COL: " << startCol << std::endl;

		int endCol = (mousePos_f.x) / tile;
		int endRow = (mousePos_f.y) / tile;
		std::pair<int, int> end = { endRow, endCol };
		std::cout << "END - ROW " << endRow << " ::::::::::::::: COL:" << endCol << std::endl;
		if (attackFound) {
			if (board.getMultiJump()) {
				if (!isLongestAttackFound) {
					if (!longestPath.empty()) {
						if (longestPath.size() > 1) {
							for (const auto& path : longestPath) {
								pathPoints.push_back(path.back());
							}
							isLongestAttackFound = true;
						}
						else {
							std::cout << "UNO LMULTI PATH\n";
							pathPoints.push_back(longestPath[0].back());
							isLongestAttackFound = true;
						}
					}
				}

				auto it = std::find(pathPoints.begin(), pathPoints.end(), end);
				if (isLongestAttackFound && it != pathPoints.end()) {
					this->multiJump(startRow, startCol, endRow, endCol, board, end);
					board.setMultiJump(false);
					isLongestAttackFound = false;
				}
				else {
					turnEnd = false;
					checker.setOutlineColor(sf::Color::Blue);
				}
			}
			else if (this->isValidAttack(startRow, startCol, endRow, endCol, board)) {
				this->attack(startRow, startCol, endRow, endCol, board);
				checker.setOutlineColor(DarkGray);
			}
			else {
				turnEnd = false;
				checker.setOutlineColor(sf::Color::Blue);
			}
		}
		else {
			this->move(startRow, startCol, endRow, endCol);
			checker.setOutlineColor(DarkGray);
		}

		shapeSelected = false;
	}
}

bool Piece::getisKing() const { return isKing; }

void Piece::draw(sf::RenderWindow& win) { win.draw(checker); }

bool Piece::getTurnEnd() const { return turnEnd; }

void Piece::setOutlineColor(sf::Color color) { 
	outlineColor = color;
	return checker.setOutlineColor(color); 
}

sf::Color Piece::getOutlineColor() const { return outlineColor; }

std::string Piece::getOutlineColorString() {
	if (outlineColor == sf::Color::Blue) {
		return "Blue";
	}
	else {
		return "DarkGray";
	}
}


void Piece::resetJumpMoves() {
	if (!visitedSquares.empty()) {
		visitedSquares.clear();
		longestPath.clear();
	}
}

std::vector<std::pair<int, int>> Piece::getVisitedSquares() const { return visitedSquares; }


void Piece::setMultiJumpPiece(bool boolean) { 
	if (boolean == true) {
		multiJumpPiece = true;
	}
	else {
		multiJumpPiece = false;
	}
}

bool Piece::getMultiJumpPiece() const { return multiJumpPiece; }
