#include <SFML\Graphics.hpp>
#include <string>
//#include "board.hpp"

class Board;

class Piece {
private:
	sf::Texture crownTexture;
	std::string color;
	sf::CircleShape checker;
	bool isKing;
	bool turnEnd;
	sf::Color outlineColor;
	std::vector<std::pair<int, int>> visitedSquares;
	std::vector<std::vector<std::pair<int, int>>> longestPath;
	bool multiJumpPiece;
public:
	sf::Color getColor(const std::string& Color);
	std::string getColorString() const;
	Piece(std::string Color, int row, int col);
	void makeKing();
	bool isValidMove(const int& startRow, const int& startCol, const int& endRow, const int& endCol) const;
	bool isOpponent(const std::shared_ptr<Piece>& piece) const;
	bool jumpDetected(const int& currentRow, const int& currentCol, Board& board);
	void findLongestPath(const std::pair<int, int>& current, std::vector<std::pair<int, int>>& path, Board& board);
	std::vector < std::vector<std::pair<int, int>>> getLongestPath() const;
	bool isValidAttack(const int& startRow, const int& startCol, const int& endRow, const int& endCol, Board board) const;
	void attack(const int& startRow, const int& startCol, const int& endRow, const int& endCol, Board& board);
	void move(const int& startRow, const int& startCol, int endRow, int endCol);
	void multiJump(const int& startRow, const int& startCol, const int& endRow, const int& endCol, Board& board, std::pair<int,int> endPos);
	sf::CircleShape getChecker() const;
	void selectPiece(bool& shapeSelected, Board& board, bool attackFound);
	bool getisKing() const;
	void draw(sf::RenderWindow& win);
	bool getTurnEnd() const;
	void setOutlineColor(sf::Color);
	sf::Color getOutlineColor() const;
	std::string getOutlineColorString();
	void resetJumpMoves();
	std::vector<std::pair<int, int>> getVisitedSquares() const;
	void setMultiJumpPiece(bool boolean);
	bool getMultiJumpPiece() const;
};
