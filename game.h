#pragma once
#include "Menu.h"
#include <iostream>
#include <vector>
#include "player.h"
#include "Screen.h"
#include "keys.h"
#include "Doors.h"
#include "Bomb.h"
#include "Riddle.h"
#include "Switch.h"
#include "Torch.h"
#include "Spring.h"
#include "Obstacle.h"

struct ActiveBomb {
	int x, y;     // Bomb position on the board
	int timer;    // How many game cycles left before explosion
	bool alive;   // True if bomb has not exploded yet
};

class game
{
	Screen theScreen;                 // The game screen (map and drawing)
	player p1;                        // First player
	player p2;                        // Second player
	std::vector<ActiveBomb> bombs;    // List of active bombs counting down
	std::vector<keys>      keys;      // All key items in the current room
	std::vector<Doors>     doors;     // All doors in the current room
	std::vector<Bomb> bombItem;       // Bomb items lying on the floor (collectable)
	std::vector<Riddle> riddles;      // All riddles in the current room
	std::vector<Switch> switches;     // All switches in the current room
	std::vector<Torch> torches;      // All torches in the current room
	std::vector<Spring> springs;     // All springs in the current room
	std::vector<Obstacle> obstacles;  // רשימת כל המכשולים בחדר
	int currentRoom = 0;              // Index of the current room
	int playersExitedThisRoom = 0;    // How many players already exited this room
	int doorTargetRoom = 1;           // Room index to go to when passing the door
	int nextRiddleIndex = 0;          // Index of the next riddle to activate/check
	friend Menu;                      // Allow Menu class to access private members
public:
	game()                            // Game constructor
		: theScreen(getScreen()),     // Use shared Screen instance
		p1(point(3, 9, { 0,0 }, '$'), "dxaws", theScreen, ' '),   // Init player 1
		p2(point(68, 9, { 0,0 }, '&'), "lmjik", theScreen, ' ')  // Init player 2
	{
		p1.setGamePtr(this);          // Give player 1 pointer back to game
		p2.setGamePtr(this);          // Give player 2 pointer back to game
		initObjectsFromBoard();       // Scan board and create keys, doors, etc.
	}
	void start_game();                // Main game loop entry point
	void addBomb(int x, int y) {
		bombs.push_back({ x, y, 5, true }); // Add new active bomb at (x,y) with 5 cycles
	}
	bool isPlayerAt(int x, int y, const player* ignore)const; // Check if any player is at (x,y)
	void handleStepOnObjects(player& pl);                     // Handle player stepping on items
	void goToNextRoom();                                      // Move game to the next room
	bool isLastActivePlayer(const player& pl) const;          // Check if this is the last active player
	void playerPassedDoor(player& pl);                        // Logic when player passes a door
	void explodeAt(int x, int y);                             // Trigger explosion at (x,y)
	void update_Internal_Wall_By_Switch(const Switch& sw);    // Update walls affected by a switch
	void setposition_player1(int x, int y) {
		p1.setPosition(x, y);          // Set position of player 1
	}
	void setposition_player2(int x, int y) {
		p2.setPosition(x, y);          // Set position of player 2
	}
	player* getOtherPlayer(const player* pl);
	bool tryPushObstacle(point& target, Direction dir);
	void mergeObstaclesOnBoard();



	//int getPushForce(Direction dir ,Screen& screen) const;

private:
	void initObjectsFromBoard();      // Read screen board and create all objects
	int pause_menu();                 // Show pause menu and return user choice
	int countSpringCompressed(const player& pl) const;
	void activateSpringIfNeeded(player& pl);


};

