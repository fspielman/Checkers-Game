#include <iostream>
#include "board.hpp"
#include <set>

#define tile 100
#define halfTile 50
#define DarkGray sf::Color(99, 102, 106)

//getters
sf::CircleShape Piece::getChecker() const { return checker; }

sf::Color Piece::getColor(const std::string& Color) const {
	if (Color == "White") return sf::Color::White;
	return sf::Color::Red;
}

std::string Piece::getColorString() const { return color; }

bool Piece::getisKing() const { return isKing; }

bool Piece::getTurnEnd() const { return turnEnd; }

bool Piece::getMultiJumpPiece() const { return multiJumpPiece; }

std::vector<std::pair<int, int>> Piece::getVisitedSquares() const { return visitedSquares; }

std::vector < std::vector<std::pair<int, int>>> Piece::getLongestPath() const { return longestPath; }

sf::Color Piece::getOutlineColor() const { return outlineColor; }

//used to set selected piece to original outline color when a piece is deselected
std::string Piece::getOutlineColorString() const {
	if (outlineColor == sf::Color::Blue) {
		return "Blue";
	}
	else {
		return "DarkGray";
	}
}

//setters
void Piece::setMultiJumpPiece(bool boolean) { 
	if (boolean == true) {
		multiJumpPiece = true;
	}
	else {
		multiJumpPiece = false;
	}
}

void Piece::setOutlineColor(sf::Color color) { 
	outlineColor = color;
	return checker.setOutlineColor(color); 
}

//methods
Piece::Piece(std::string Color, int row, int col) : color(Color), isKing(false) {
	bool shapeSelected = false;
	//piece display settings
	checker.setRadius(35.f);
	checker.setPosition(col * tile + halfTile, row * tile + halfTile);
	checker.setFillColor(getColor(Color));
	checker.setOutlineThickness(5.f);
	checker.setOutlineColor(DarkGray);
	checker.setOrigin(35.f, 35.f);
}

void Piece::checkKingPromotion(const int endRow, const std::string& color) {
	if ((color == "White" && endRow == 7) || (color == "Red" && endRow == 0) && !isKing) {
		this->makeKing();
	}
}

void Piece::makeKing() {
	isKing = true;
	crownTexture.loadFromFile("images\\king.png");
	checker.setTexture(&crownTexture);
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

//used in the findAttacks method in board.cpp
bool Piece::jumpDetected(const int& currentRow, const int& currentCol, Board& board){
	//posible end positions from single jump - two tiles diagonally from every direction 
    std::vector<std::pair<int, int>> possibleJumps = 
	{
		{currentCol + 2, currentRow + 2}, {currentCol - 2, currentRow + 2},
		{currentCol + 2, currentRow - 2}, {currentCol - 2, currentRow - 2}
    };
	
	bool foundJump = false;
	multiJumpPiece = false; //flag that piece has multijump

	for (const auto& jump : possibleJumps) {
		int endCol = jump.first;
		int endRow = jump.second;
		if (board.isWithinBounds(endRow,endCol)) {
			if (this->isValidAttack(currentRow, currentCol, endRow, endCol, board)) {
				//if the current square has not been visited yet -> add it to visitedSquares
				if (std::find(visitedSquares.begin(), visitedSquares.end(), std::make_pair(currentRow, currentCol)) == visitedSquares.end()) {
					visitedSquares.push_back({ currentRow, currentCol }); 
				}
				//check if the end position has not been visited and detect further jumps from that position
				if (std::find(visitedSquares.begin(), visitedSquares.end(), std::make_pair(endRow, endCol)) == visitedSquares.end()
					&& this->jumpDetected(endRow, endCol, board)) {
					//if another jump is found -> multi jump is true
					board.setMultiJump(true); //flag used to draw multijump pathing when piece is selected set to true
					this->setMultiJumpPiece(true);
				}

				foundJump = true; //jump is found if valid atttack is found 
			}
		}
	}

	return foundJump;
}

void Piece::findLongestPath(const std::pair<int, int>& current, std::vector<std::pair<int, int>>& path, Board& board) {
	//add current position
    path.push_back(current);

    // Define possible attack moves
    std::vector<std::pair<int, int>> directions = {
        {2, 2}, {-2, 2}, {2, -2}, {-2, -2}
    };

    bool morePaths = false;

    for (const auto& direction : directions) {
		//next position
        std::pair<int, int> nextPos = {current.first + direction.first, current.second + direction.second};
		
		//check if next position has been visited, not part of the current path, and is a valid attack from the current position
        if (std::find(visitedSquares.begin(), visitedSquares.end(), nextPos) != visitedSquares.end() &&
            std::find(path.begin(), path.end(), nextPos) == path.end()
			&& this->isValidAttack(current.first, current.second, nextPos.first, nextPos.second, board)) {
			//another path is found -> search recursively again to find if path continues on
            morePaths = true;
            findLongestPath(nextPos, path, board);
        }
    }

	//if no more paths were found and the longest path vector is empty -> push path to logest path vector since it is currently the longest path
    if (!morePaths) {
		if (longestPath.empty()) {
            longestPath.push_back(path);
		}
		//if there is other paths of similar size -> add path to current grouping of longest paths
		else if (path.size() == longestPath[0].size()) {
			longestPath.push_back(path);
		}
		//if new path is longer than the current longest path -> replace the current longest path with the new path
        else if (path.size() > longestPath[0].size()) {
			longestPath.clear();
            longestPath.push_back(path);
        }
    }
	
	//pop current position from path for backtracking -> explore unvisited directions due to their position in the directions vector
	path.pop_back();
}

bool Piece::isValidAttack(const int& startRow, const int& startCol, const int& endRow, const int& endCol, Board board) const {
	//the current position and end position must be 2 tiles apart diagonally
	if (!(abs(endCol - startCol) == 2) || !(abs(endRow - startRow) == 2)) return false; 
	int midCol = (startCol + endCol) / 2;
	int midRow = (startRow + endRow) / 2;

	auto middlePiece = board.getBoard(midRow, midCol);

	auto endPoint = board.getBoard(endRow, endCol);

	//end position has to be empty (null) and middle piece has to be an opponent piece
	return !endPoint &&  this->isOpponent(middlePiece); }

//perform single jump
void Piece::attack(const int& startRow, const int& startCol, const int& endRow, const int& endCol, Board& board) {
	turnEnd = true;
	checker.setPosition(endCol * tile + halfTile, endRow * tile + halfTile);
	//set miuddle position to nullptr -> piece has been taken
	board.setBoard((startRow + endRow) / 2, (startCol + endCol) / 2, nullptr);
	this->checkKingPromotion(endRow, color);
}

//perform regular move
void Piece::move(const int& startRow, const int& startCol, int endRow, int endCol) {
	if (isValidMove(startRow, startCol, endRow, endCol)) {
        checker.setPosition(endCol * tile + halfTile, endRow * tile + halfTile);
		turnEnd = true;
		this->checkKingPromotion(endRow, color);
    }
	else {
		//not a valid regular move
		turnEnd = false;
    }
}

//perform multijump
void Piece::multiJump(const int& startRow, const int& startCol, const int& endRow, const int& endCol, Board& board, std::pair<int,int> endPos) {
	turnEnd = true;
	checker.setPosition(endCol * tile + halfTile, endRow * tile + halfTile);
	std::vector<std::pair<int, int>> chosenPath;

	//find chosen path
	for (const auto& path : longestPath) {
		if (std::find(path.begin(), path.end(), endPos) != path.end()) {
			chosenPath = path;
			break;
		}
	}
	
	for (int i = 0; i < chosenPath.size() - 1; ++i) {
		int midRow = (chosenPath[i].first + chosenPath[i + 1].first) / 2;
		int midCol = (chosenPath[i].second+ chosenPath[i + 1].second) / 2;
		//remove jumped pieces
		board.setBoard(midRow, midCol, nullptr); 	}

	this->checkKingPromotion(endRow, color);
}

//handles piece selection and movement
void Piece::selectPiece(bool& shapeSelected, Board& board, bool attackFound) {
	std::vector<std::pair<int, int>> pathPoints; //contains endpoints for the multijump attacks
	bool isLongestAttackFound = false; //flag used for multijumps to handle incorrect multijump moves without recalculating pathPoints

	if (!shapeSelected) {
		//if attack available, then only pieces with a blue outline (piece with attack) can be selected
		if (attackFound && !(this->getOutlineColor() == sf::Color::Blue)) {
			shapeSelected = false;
		}
		else {
			shapeSelected = true;
			checker.setOutlineColor(sf::Color::Yellow);
		}
	}
	else {
		//get current position of mouse in the window
		sf::Vector2i mousePosition = sf::Mouse::getPosition(board.getWindow());
		sf::Vector2f mousePos_f(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y));

		int startCol = (this->getChecker().getPosition().x) / tile;
		int startRow = (this->getChecker().getPosition().y) / tile;

		int endCol = (mousePos_f.x) / tile;
		int endRow = (mousePos_f.y) / tile;
		std::pair<int, int> end = { endRow, endCol };
		
		//if attack is found -> either multijump or single jump
		if (attackFound) {
			//if multijump is found -> must perform multijump
			if (board.getMultiJump()) {
				//checks for valid attacks if they havent been found yet
				if (!isLongestAttackFound) {
					if (!longestPath.empty()) {
						//add endpoints for when there are multple longest paths for piece
						if (longestPath.size() > 1) {
							for (const auto& path : longestPath) {
								pathPoints.push_back(path.back());
							}
							isLongestAttackFound = true;
						}
						//add enpoint for when there is only one longest path
						else {
							pathPoints.push_back(longestPath[0].back());
							isLongestAttackFound = true;
						}
					}
				}

				auto it = std::find(pathPoints.begin(), pathPoints.end(), end);
				//if longest attack is found and the iterator found the end position -> perform multijump
				if (isLongestAttackFound && it != pathPoints.end()) {
					this->multiJump(startRow, startCol, endRow, endCol, board, end);
					//revert multijump boolean and longest attack found boolean
					board.setMultiJump(false);
					isLongestAttackFound = false;
				}
				//invalid multijump move
				else {
					turnEnd = false;
					checker.setOutlineColor(sf::Color::Blue);
				}
			}
			//handle single jump
			else if (this->isValidAttack(startRow, startCol, endRow, endCol, board)) {
				this->attack(startRow, startCol, endRow, endCol, board);
				checker.setOutlineColor(DarkGray);
			}
			//invalid attack
			else {
				turnEnd = false;
				checker.setOutlineColor(sf::Color::Blue);
			}
		}
		//handle regular move
		else {
			this->move(startRow, startCol, endRow, endCol);
			checker.setOutlineColor(DarkGray);
		}

		//deselect piece after move is completed
		shapeSelected = false;
	}
}

//draw checker to board
void Piece::draw(sf::RenderWindow& win) { win.draw(checker); }

//reset all of the visited squares and longest paths after turn is over
void Piece::resetJumpMoves() {
	if (!visitedSquares.empty()) {
		visitedSquares.clear();
		longestPath.clear();
	}
}

