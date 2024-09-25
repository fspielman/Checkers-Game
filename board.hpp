#pragma once
#include <SFML\Graphics.hpp>
#include <memory>
#include "piece.hpp"

class Board {
private:
	sf::RenderWindow& window;
	std::vector<std::vector<std::shared_ptr<Piece>>> board;
	//std::vector<std::pair<int, int>> visitedSquares;
	bool multiJump;
	std::map<std::string, int> pieceCnt;
	bool found;
	bool available;
public:
	Board(sf::RenderWindow& win);
	void initBoard();
	void drawBoard();
	void drawMoves(const std::string& color, const int& startY, const int& startX);
	bool isWithinBounds(int row, int col);
	void drawSingleJump(const std::string& color, int startRow, int startCol);
	void drawSingleMoves(const std::string& color, const int startRow, const int& startCol);
	void draw();
	std::shared_ptr<Piece> getBoard(int row, int col);
	void setBoard(int row, int col, std::shared_ptr<Piece> piece);
	// void updateBoard(int row, int col, std::shared_ptr<Piece> piece);
	void changeTurn(std::string& turn);
	bool findAttacks(const std::string& turn);
	void resetAttackOutline();
	void drawingMove(const int& row, const int& col);
	sf::RenderWindow& getWindow() const;
//	void resetJumpMoves();
//	std::vector<std::pair<int, int>> getVisitedSquares() const;
	void setMultiJump(bool boolean);
	bool getMultiJump() const;
	void testing(const int& row, const int& col);
	void countPieces(sf::Text& txt);
	bool hasPlayerLost(const std::string& turn) const;
	bool movesAvailable(const std::string& turn);
	void populateDirections(const std::string& color, const int& row, const int& col, std::vector<std::pair<int, int>>& directions);
	bool getAvailable() const;
	void rotateBoard();
};
