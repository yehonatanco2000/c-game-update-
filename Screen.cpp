#pragma once
#include "Screen.h"
#include "utils.h"
#include <iostream>
#include "point.h"



const char* Screen::room1[Screen::MAX_Y + 1] = {
          //   01234567890123456789012345678901234567890123456789012345678901234567890123456789
              "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",  // 0
              "W          WWWWWWWWWWWWWWWWWWW  k        WWWWWWWWWWWWWWWWWWWW      @           W",  // 1
              "W          W                 WW         WW                  W                  W",  // 2
              "W          W                 WW         WW                  W                  W",  // 3
              "W          W                 WW  \\      WW       WWWWWWWWWWWW                  W",  // 4
              "W          W        WWWWWWWWWWWWWWWW?WWWWW        W                            W",  // 5
              "W          W        W                    W        W                            W",  // 6
              "W          W        W                    W   k    W      @                     W",  // 7
              "W          ?        W                    W        W                            W",  // 8
              "W          W        W   *                W        W                            W",  // 9
              "W          W        W               **   W        W                            W",  // 10
              "W          W        W                    W        W                            W",  // 11
              "W          W        W                    W        W       @     *             #W",  // 12
              "W          W        W                    W        W                            W",  // 13
              "W          W        W                    W        W                  * **    ##W",  // 14
              "W          W        W        WWWW        W    @   W                            W",  // 15
              "W          W        W        W  W        W        W       @                    W",  // 16
              "W          W     @  W        W2 W        W  WWWWWWW            *   k ***  #####W",  // 17
              "W          W        W        WWWW        W WW                                  W",  // 18
              "W     !    W        W                    WW         @                 @        W",  // 19  
              "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",  // 20
                "The player($) is carrying an item:    ||  life remand:5                       ",  // 21
                "The player(&) is carrying an item:    ||  life remand:5                       ",  // 22
              "DOOR OPEN (YES || NO): NO                                                       ",  //23
              "Switch is now:                                                                  "   //24
};

const char* Screen::room2[Screen::MAX_Y + 1] = {
    //           01234567890123456789012345678901234567890123456789012345678901234567890123456789
                "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW", // 0
                "W                    W    W      W            W       W       W       W   W    W",  // 1
                "W                    W    W      W            W   W   W   W   W   W   W   ?    W",  // 2
                "W                    W  1 W      W            W   W   W   W   W   W   W   WWWWWW",  // 3
                "W                    W    W      W            W   W   W   W   W   W   W        W",  // 4
                "W                    W    W      W       @    W   W   W   W   W   W   W        W",  // 5
                "W                    WWWWWW      W            W   W   W   W   W   W   W        W",  // 6
                "W                                W       @    W   W   W   W   W   W   W        W",  // 7
                "W                                W            W   W   W   W   W   W   W        W",  // 8
                "W                     W          W            W   W   W   W   W   W   W        W",  // 9
                "W                     W          WWWWWWWW     W   W   W   W   W   W   W        W",  // 10
                "W                     W          W    W  W        W       W       W            W",  // 11
                "W                     W          W    W WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW\\WWWW",  // 12
                "W                     W          W    W                                        W",  // 13
                "W                     W          W  k W                                        W",  // 14
                "W                     W          W    W                                        W",  // 15
                "W                     W          WWWWWW                                        W",  // 16
                "W                     W                                                        W",  // 17
                "W                     W           W                                            W",  // 18
                "W                     W           W                                            W",  // 19  
                "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",  // 20
                "The player($) is carrying an item:    ||  life remand:5                         ",  // 21
                "The player(&) is carrying an item:    ||  life remand:5                         ",  // 22
                "DOOR OPEN (YES || NO): NO                                                       ",  //23
                "Switch is now:                                                                  "   //24
};

const char* Screen::room3[Screen::MAX_Y + 1] = {
    //            01234567890123456789012345678901234567890123456789012345678901234567890123456789
				 "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",// 0
				 "W                                                                              W",// 1
				 "W                                                                              W",// 2
				 "W                      YY    YY    OOOOOO    UUU   UUU                         W",// 3
				 "W                      YY    YY   OO    OO   UUU   UUU                         W",// 4
				 "W                       YY  YY    OO    OO   UUU   UUU                         W",// 5
				 "W                        YYYY     OO    OO   UUU   UUU                         W",// 6
				 "W                         YY      OO    OO   UUU   UUU                         W",// 7
				 "W                         YY      OO    OO   UUU   UUU                         W",// 8
				 "W                         YY      OO    OO   UUU   UUU                         W",// 9
				 "W                         YY       OOOOOO     UUUUUU                           W",// 10
				 "W                                                                              W",//11
				 "W                                                                              W",//12
				 "W                                                                              W",//13
                 "W                       ww        ww   IIIII   NNN   NN                        W",//14
				 "W                       ww   ww   ww    III    NNNN  NN                        W",//15
				 "W                       ww   ww   ww    III    NN NN NN                        W",//16
				 "W                       ww   ww   ww    III    NN  NNNN                        W",//17
				 "W                       ww   ww   ww    III    NN   NNN                        W",//18
				 "W                         ww ww ww     IIIII   NN    NN                        W",//19
				 "W                                                                              W",//20
				 "W                                                                              W",//21
				 "W                                                                              W",//22
                 "W                                                                              W",//23
                 "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",//24
				//01234567890123456789012345678901234567890123456789012345678901234567890123456789
       
};


void Screen::draw() {
    for (int y = 0; y <= MAX_Y; ++y) {                       // Go over all rows
        for (int x = 0; x <= MAX_X; ++x) {                   // Go over all columns
            char ch = board[y][x];                           // Character from board at (x,y)

            Color col = Color::WHITE_BRIGHT;                 // Default text color

            if (!isCellLitByTorch(x, y)) {
                ch = ' ';
            }

            // Choose color based on the character type
            if (ch == 'W') col = Color::LIGHT_CYAN;         // Walls
            else if (ch == '$' || ch == '&') col = Color::WHITE_BRIGHT; // Players
            else if (ch == '@') col = Color::RED;           // Bomb item
            else if (ch == 'k') col = Color::YELLOW;        // Key
            else if (ch == '?') col = Color::GREEN;         // Riddle
            else if (ch == '/' || ch == '\\') col = Color::CYAN; // Switch
            else if (ch >= '1' && ch <= '9') col = Color::MAGENTA; // Doors (numbers)
			else if (ch == '!') col = Color::BLUE;          // Special item
            else if (ch == '*') col = Color::LIGHT_GREEN;  // obstacle
			else if (ch == '#') col = Color::BROWN;         // Spring


            drawChar(x, y, ch, col);                         // Draw single cell with color
        }
    }
}

//perplexity AI.
void Screen::loadRoom(int roomIndex) {
    const char* const* src = nullptr;                        // Pointer to source room data

    switch (roomIndex) {                                     // Select which room to load
    case 0: src = room1; break;
    case 1: src = room2; break;
    case 2: src = room3; break;
    default: src = room1; break;                             // Fallback to room1
    }

    for (int y = 0; y <= MAX_Y; ++y) {                       // Copy each row into board
        std::memcpy(board[y], src[y], std::strlen(src[y]) + 1);
        board[y][MAX_X] = '\0';                              // Ensure null-terminated line
    }
    for (int y = 0; y <= MAX_Y; ++y)
        for (int x = 0; x <= MAX_X; ++x)
            dark[y][x] = false;

    if (roomIndex == 0) {
        // example dark area in room 1
        for (int y = 5; y <= 15; ++y)
            for (int x = 10; x <= 27; ++x)
                dark[y][x] = true;
        for(int y = 6; y <= 19; ++y)
            for (int x =41; x <= 50; ++x)
				dark[y][x] = true;
    }
    if(roomIndex == 1) {
        // example dark area in room 2
        for (int y = 10; y <= 16; ++y)
            for (int x = 33; x <= 39; ++x)
                dark[y][x] = true;
	}
}

void Screen::explodeAt(int x, int y) {
    // Remove the bomb character itself
    setChar(y, x, ' ');
    drawChar(x, y, ' ', Color::WHITE_BRIGHT);

    // Radius 3 explosion in all directions
    for (int dy = -3; dy <= 3; ++dy) {
        for (int dx = -3; dx <= 3; ++dx) {
            int nx = x + dx;                                 // Neighbor x
            int ny = y + dy;                                 // Neighbor y

            if (nx < 0 || nx > MAX_X || ny < 0 || ny > MAX_Y)
                continue;                                    // Skip out-of-bounds

            char ch = board[ny][nx];                         // Character at neighbor

            if (ch != ' ') {
                // Do not erase outer frame walls (top, bottom, left, right borders)
                if ((ny == 0 && nx >= 0 && nx <= MAX_X) ||
                    (ny >= 20 && nx >= 0 && nx <= MAX_X) ||
                    (nx == 0 && ny >= 0 && ny <= MAX_Y) ||
                    ((nx == (MAX_X - 1) || nx == MAX_X) && ny >= 0 && ny <= MAX_Y)) {
                    continue;
                }
                board[ny][nx] = ' ';                         // Clear cell in board
                gotoxy(nx, ny);                              // Move cursor to cell
                drawChar(nx, ny, ' ', Color::WHITE_BRIGHT);  // Erase visually
            }
        }
    }
}

//perplexity AI.
void Screen::drawChar(int x, int y, char ch, Color color) {
    gotoxy(x, y);                                           // Move cursor to (x,y)
    if (!enableColor) {                                     // If colors disabled
        std::cout << ch << std::flush;                      // Just print plain char
        return;
    }

    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);             // Get console handle
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(h, &info);                   // Save current attributes
    WORD oldAttr = info.wAttributes;

    SetConsoleTextAttribute(h, static_cast<WORD>(color));   // Set new text color
    std::cout << ch << std::flush;                          // Print character
    SetConsoleTextAttribute(h, oldAttr);                    // Restore old attributes
}

bool Screen::isCellLitByTorch(int x, int y) const
{
    // Cell is not in a dark area
    if (!dark[y][x])
        return true;

    // Torch item itself is always visible
    if (board[y][x] == '!')
        return true;

    const int TORCH_RADIUS = 13;

    // Check player 1
    if (p1HasTorch &&
        manhattanDist(x, y, p1x, p1y) <= TORCH_RADIUS)
        return true;

    // Check player 2
    if (p2HasTorch &&
        manhattanDist(x, y, p2x, p2y) <= TORCH_RADIUS)
        return true;

    // Dark cell with no torch in range
    return false;
}

Screen& getScreen() {
    static Screen s;                                        // Single shared Screen instance
    return s;                                               // Return reference to it
}



