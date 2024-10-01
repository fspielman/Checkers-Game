#pragma once
#include <SFML\Graphics.hpp>
#include <memory>
#include "piece.hpp"
#include <array>

class Board {
private:
	sf::RenderWindow& window;
	std::array<std::array<std::shared_ptr<Piece>, 8>, 8> board;
	bool multiJump;
	std::map<std::string, int> pieceCnt;
	bool isAttackFound;
	bool areMovesAvailable;
public:
	sf::RenderWindow& getWindow() const;
	bool getMultiJump() const;
	std::shared_ptr<Piece> getBoard(int row, int col);
	void setMultiJump(bool boolean);
	void setBoard(int row, int col, std::shared_ptr<Piece> piece);
	Board(sf::RenderWindow& win);
	void initBoard();
	void drawBoard();
	void drawMoves(const std::string& color, const int& startY, const int& startX);
	bool isWithinBounds(int row, int col);
	void drawSingleJump(const std::string& color, int startRow, int startCol);
	void drawSingleMoves(const std::string& color, const int startRow, const int& startCol);
	void draw();
	void changeTurn(std::string& turn);
	bool findAttacks(const std::string& turn);
	void resetAttackOutline();
	void drawingMove(const int& row, const int& col);
	void drawMultiPath(const int& row, const int& col);
	void countPieces(sf::Text& txt);
	bool hasPlayerLost(const std::string& turn) const;
	bool movesAvailable(const std::string& turn);
	void populateDirections(const std::string& color, const int& row, const int& col, std::vector<std::pair<int, int>>& directions);
	void rotateBoard();
};
