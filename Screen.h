#pragma once


#include <cstring>  
#include "utils.h"
#include "point.h"


class point;
class Screen
{
public:
	static constexpr int MAX_X = 80;                    // Maximum columns of the screen
	static constexpr int MAX_Y = 24;                    // Maximum rows of the screen
	char board[MAX_Y + 1][MAX_X + 1];                   // 2D array representing the room layout

private:
	static const char* room1[MAX_Y + 1];               // Static map data for room 1
	static const char* room2[MAX_Y + 1];               // Static map data for room 2
	static const char* room3[MAX_Y + 1];               // Static map data for room 3
	bool dark[MAX_Y + 1][MAX_X + 1]{};                 // which cells belong to dark area
	bool p1HasTorch = false;
	bool p2HasTorch = false;
	int  p1x = 0, p1y = 0;
	int  p2x = 0, p2y = 0;


public:
	Screen() {
		loadRoom(0);                                   // Load initial room (room 0) at start
	}

	void loadRoom(int roomIndex);                     // Copy selected room layout into board

	char charAt(const point& p) const {
		return board[p.getY()][p.getX()];             // Get character at point position
	}

	bool isWall(const point& p) const {
		return charAt(p) == 'W';                      // Check if given point is a wall
	}
	bool isobstacle(const point& p) {
		return (charAt(p) == '*');
	}

	bool isobj(const point& p) const {
		return (charAt(p) >= '1' && charAt(p) <= '9'  // Check if point is any object:
			|| charAt(p) == '@'                   // numbers, key, riddles, etc.
			|| charAt(p) == 'k'
			|| charAt(p) == '?'
			|| charAt(p) == '/'
			|| charAt(p) == '\\'
			|| charAt(p) == '!'
			|| charAt(p) == '#'
			|| charAt(p) == '*');
	}

	void explodeAt(int x, int y);                     // Handle explosion centered at (x,y)
	void draw();                                      // Draw entire board to the console

	void setChar(int y, int x, char c) {
		board[y][x] = c;                              // Set specific cell in the board
	}

	void getChar(int y, int x) {
		board[y][x] = ' ';                            // Clear specific cell (set to space)
	}

	bool enableColor = false;                         // Global flag: draw with colors or not

	void drawChar(int x, int y, char ch, Color color);// Draw single char at (x,y) with color

	void setDark(int x, int y, bool value) { dark[y][x] = value; }
	bool isDark(int x, int y) const { return dark[y][x]; }

	void setTorchInfo(bool has1, bool has2,int x1, int y1,int x2, int y2)
	{
		p1HasTorch = has1;
		p2HasTorch = has2;
		p1x = x1; p1y = y1;
		p2x = x2; p2y = y2;
	}
	static int manhattanDist(int x1, int y1, int x2, int y2)
	{
		return std::abs(x1 - x2) + std::abs(y1 - y2);
	}
	bool isCellLitByTorch(int x, int y) const;
};

//perplexity AI.
Screen& getScreen(); // Return reference to the shared Screen instance used by the whole game
