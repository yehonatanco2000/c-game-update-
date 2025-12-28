#pragma once
#include "player.h"
#include "Screen.h"
#include "game.h"
#include <cctype>
#include <iostream>






void player::disposeItem() {
	if (carry_item == ' ') {                         // If hand is empty, nothing to dispose
		return;
	}

	int dropX = p.getX();                           // Start drop position at player x
	int dropY = p.getY();                           // Start drop position at player y

	Direction d = p.getDir();                       // Current movement direction

	if (d.dx() != 0 || d.dy() != 0) {
		// Throw item behind the player relative to movement direction
		dropX -= d.dx();
		dropY -= d.dy();
	}
	else {
		// Player is not moving: search for a free nearby tile
		const int offsets[][2] = {
		{ 1,  0},  // Right
		{-1,  0},  // Left
		{ 0, -1},  // Up
		{ 0,  1},  // Down
		{ 1, -1},  // Right-Up
		{ 1,  1},  // Right-Down
		{-1, -1},  // Left-Up
		{-1,  1}   // Left-Down
		};

		bool found = false;                         // True if a free tile is found
		for (auto& off : offsets) {
			int nx = dropX + off[0];               // Candidate x
			int ny = dropY + off[1];               // Candidate y

			point candidate(nx, ny, { 0,0 }, ' '); // Temporary point for checks

			if (!screen.isWall(candidate) && !screen.isobj(candidate)) {
				dropX = nx;                        // Use this free tile
				dropY = ny;
				found = true;
				break;
			}
		}

		// If no free tile is found around the player, do not drop the item
		if (!found) {
			return;
		}
	}

	point target(dropX, dropY, { 0,0 }, ' ');       // Final drop position
	if (screen.isWall(target) || screen.isobj(target)) {
		return;                                     // Do not drop on wall/object
	}

	// Place the item on the dynamic board and draw it
	screen.setChar(dropY, dropX, carry_item);       // Update board cell with item
	screen.drawChar(dropX, dropY, carry_item,       // Draw with color by item type
		(carry_item == '@') ? Color::RED :
		(carry_item == 'k') ? Color::YELLOW :
		(carry_item == '!') ? Color::BLUE :
		Color::WHITE_BRIGHT);

	if (carry_item == '@') {                        // If dropped item is a bomb
		if (gameptr)
			gameptr->addBomb(dropX, dropY);        // Register active bomb in game
	}
	carry_item = ' ';                               // Clear player's hand
	screen.setChar(21 + (p.c == '$' ? 0 : 1), 34, carry_item); // Clear HUD item slot
}

bool player::pickupIfPossible() {
	char object = screen.charAt(p);                 // Character on current tile

	if (!iscarrying_item() && object != '?' && object != '/' && object != '\\' && object != ' ' && object != '#' && object != '*') {
		// Hand is empty and tile is a pickup item (not riddle/switch/empty)
		carry_item = object;                        // Take the item
		screen.setChar(p.getY(), p.getX(), ' ');    // Clear tile on board
		screen.drawChar(p.getX(), p.getY(), ' ', Color::WHITE_BRIGHT); // Erase from screen
		return true;                                // Now holding an item
	}

	// Hand already holds key/bomb – swap or drop according to design
	if (carry_item == '@' || carry_item == 'k' || carry_item == '!') {
		screen.setChar(p.getY(), p.getX(), object); // Put previous object back on floor
		if (object == 'k') {
			screen.drawChar(p.getX(), p.getY(), object, Color::YELLOW); // Draw key in yellow
		}
		else if (object == '@') {
			screen.drawChar(p.getX(), p.getY(), object, Color::RED);    // Draw bomb in red
		}
		else if( object == '!') {
			screen.drawChar(p.getX(), p.getY(), object, Color::BLUE);   // Draw torch in blue
		}
		else {
			screen.drawChar(p.getX(), p.getY(), object, Color::WHITE_BRIGHT); // Default color
		}
		return true;
	}

	return false;                                   // No change (cannot pick up)
}

void player::move() {
	if (!isactive) {
		return;                                        // Inactive player does not move
	}
	// 1) Try spring-based movement first
	if (moveWithSpring()) {
		if (gameptr) {
			updateHUD();
		}
		return;        // Do not perform normal move when spring handled it
	}
	// Compute desired next position based on current direction
	int nextX = p.getX();
	int nextY = p.getY();
	Direction d = p.getDir();
	nextX += d.dx();
	nextY += d.dy();
	point nextPoint(nextX, nextY, { 0,0 }, p.c);

	// If next tile is occupied by the other player – do not move
	if (gameptr && gameptr->isPlayerAt(nextX, nextY, this)) {
		p.dir = Direction::directions[Direction::STAY]; // Stop moving
		return;
	}
	if (screen.isWall(nextPoint)) {
		p.dir = Direction::directions[Direction::STAY];
		return;
	}

	// מכשול – ניסיון דחיפה כשהשחקן עדיין לפני המכשול
	if (screen.isobstacle(nextPoint)) {
		if (!(gameptr && gameptr->tryPushObstacle(nextPoint, d))) {
			// לא הצליח לדחוף – נשאר במקום
			p.x = nextX - d.dx();  // חזרה למיקום הקודם
			p.y = nextY - d.dy();
			p.draw();
			return;
		}
		// אם הצליח – nextPoint עכשיו ריק
	}

	// עכשיו מזיזים את השחקן בדיוק כמו קודם
	p.draw(' ');
	p.move();
	p.draw();

	if (gameptr) {
		gameptr->handleStepOnObjects(*this);          // React to items, doors, switches, etc.
		updateHUD();                                 // Update HUD to reflect any changes
	}
}

void player::keyPressed(char ch) {
	size_t index = 0;                                  // Index into Moves array
	for (char move : Moves) {                          // Scan all movement keys
		if (std::tolower(move) == std::tolower(ch)) {  // Match ignoring case
			p.changeDir(Direction::directions[index]); // Change direction accordingly
			break;
		}
		++index;
	}
	// Dispose item – depends on which player this is
	if ((p.c == '$' && (ch == 'e')) ||                 // Player 1 uses 'e'
		(p.c == '&' && (ch == 'o'))) {                 // Player 2 uses 'o'
		disposeItem();                                 // Try to drop/throw carried item
	}
}

void player::setStartPosition(int x, int y) {
	p.x = x;                                          // Update internal point x
	p.y = y;                                          // Update internal point y
	startpointX = x;                                  // Store new starting X
	startpointY = y;                                  // Store new starting Y
}

bool player::moveWithSpring()
{
	if (springTicksLeft <= 0)
		return false;  // not under spring effect

	// Base velocity from spring
	int vx = springDir.dx() * springSpeed;
	int vy = springDir.dy() * springSpeed;

	// Allow sideways movement (player input)
	Direction inputDir = p.getDir();

	int sidex = inputDir.dx();
	int sidey = inputDir.dy();

	// Forbid movement opposite to spring direction
	if (sidex == -springDir.dx() && sidex != 0)
		sidex = 0;
	if (sidey == -springDir.dy() && sidey != 0)
		sidey = 0;

	vx += sidex;
	vy += sidey;

	// Number of small steps to take this frame
	int ax = std::abs(vx);
	int ay = std::abs(vy);
	int steps = (ax > ay) ? ax : ay;   // max

	int stepX = (steps > 0) ? vx / steps : 0;
	int stepY = (steps > 0) ? vy / steps : 0;


	for (int i = 0; i < steps; ++i) {
		int nextX = p.getX() + stepX;
		int nextY = p.getY() + stepY;

		// Check other player
		if (gameptr && gameptr->isPlayerAt(nextX, nextY, this)) {
			// Collision under spring effect: push the other player
			player* other = gameptr->getOtherPlayer(this);

			// Give other same spring effect
			other->setSpringEffect(springSpeed, springTicksLeft, springDir);

			// We can either stop here or continue;
			clearSpringEffect();
			p.dir = Direction::directions[Direction::STAY];
			return true;
		}

		point nextPoint(nextX, nextY, { 0,0 }, p.c);
		point old_pos = p;
		p.draw(' ');

		// 2. קודם בודקים קיר
		if (screen.isWall(nextPoint)) {
			// נעצרים לפני הקיר
			p = old_pos;
			p.draw();
			clearSpringEffect();
			if (gameptr)
				gameptr->handleStepOnObjects(*this);
			return true;
		}

		// 3. עכשיו בודקים מכשול – דחיפה כמו ב-move הרגיל
		if (screen.isobstacle(nextPoint)) {
			// ניסיון דחיפה של המכשול בכיוון springDir
			if (gameptr && gameptr->tryPushObstacle(nextPoint, springDir)) {
				// המכשול זז, התא הבא עכשיו ריק – השחקן מתקדם לשם
				p = old_pos;
				p.x = nextX;
				p.y = nextY;
				p.draw();
				if (gameptr)
					gameptr->handleStepOnObjects(*this);
				continue;   // ממשיכים לשאר צעדי הקפיץ
			}
			else {
				// לא הצליח לדחוף – נעצרים לפני המכשול
				p = old_pos;
				p.draw();
				clearSpringEffect();
				if (gameptr)
					gameptr->handleStepOnObjects(*this);
				return true;
			}
		}

		// 4. אין קיר ואין מכשול – מתקדמים כרגיל
		p.x = nextX;
		p.y = nextY;
		p.draw();
		if (gameptr)
			gameptr->handleStepOnObjects(*this);
	}

	// decrease remaining spring ticks
	--springTicksLeft;
	if (springTicksLeft == 0) {
		clearSpringEffect();
	}

	return true;   // we handled movement this frame
}

void player::updateHUD()
{
	if (p.c == '$') {
		screen.setChar(21, 34, carry_item);
		if (carry_item == '@')
			screen.drawChar(34, 21, '@', Color::RED);
		else if (carry_item == 'k')
			screen.drawChar(34, 21, 'k', Color::YELLOW);
		else if (carry_item == '!')
			screen.drawChar(34, 21, '!', Color::BLUE);
		else
			screen.drawChar(34, 21, ' ', Color::WHITE_BRIGHT);
	}
	else if (p.c == '&') {
		screen.setChar(22, 34, carry_item);
		if (carry_item == '@')
			screen.drawChar(34, 22, '@', Color::RED);
		else if (carry_item == 'k')
			screen.drawChar(34, 22, 'k', Color::YELLOW);
		else if (carry_item == '!')
			screen.drawChar(34, 22, '!', Color::BLUE);
		else
			screen.drawChar(34, 22, ' ', Color::WHITE_BRIGHT);
	}
}



//bool player::moveWithSpring()
//{
//	if (springTicksLeft <= 0)
//		return false;  // not under spring effect
//
//	// Base velocity from spring
//	int vx = springDir.dx() * springSpeed;
//	int vy = springDir.dy() * springSpeed;
//
//	// Allow sideways movement (player input)
//	Direction inputDir = p.getDir();
//
//	int sidex = inputDir.dx();
//	int sidey = inputDir.dy();
//
//	// Forbid movement opposite to spring direction
//	if (sidex == -springDir.dx() && sidex != 0)
//		sidex = 0;
//	if (sidey == -springDir.dy() && sidey != 0)
//		sidey = 0;
//
//	vx += sidex;
//	vy += sidey;
//
//	// Number of small steps to take this frame
//	int ax = std::abs(vx);
//	int ay = std::abs(vy);
//	int steps = (ax > ay) ? ax : ay;   // max
//
//	int stepX = (steps > 0) ? vx / steps : 0;
//	int stepY = (steps > 0) ? vy / steps : 0;
//
//
//	for (int i = 0; i < steps; ++i) {
//		int nextX = p.getX() + stepX;
//		int nextY = p.getY() + stepY;
//
//		// Check other player
//		if (gameptr && gameptr->isPlayerAt(nextX, nextY, this)) {
//			// Collision under spring effect: push the other player
//			player* other = gameptr->getOtherPlayer(this);
//
//			// Give other same spring effect
//			other->setSpringEffect(springSpeed, springTicksLeft, springDir);
//
//			// We can either stop here or continue;
//			clearSpringEffect();
//			p.dir = Direction::directions[Direction::STAY];
//			return true;
//		}
//
//		point old_pos = p;
//		p.draw(' ');
//		p.x = nextX;
//		p.y = nextY;
//
//		if (screen.isWall(p)) {
//			// Hit wall: cancel spring and stay before wall
//			p = old_pos;
//			clearSpringEffect();
//			p.draw();
//			if (gameptr)
//				gameptr->handleStepOnObjects(*this);
//			return true;
//		}
//
//		p.draw();
//		if (gameptr)
//			gameptr->handleStepOnObjects(*this);
//	}
//
//	// decrease remaining spring ticks
//	--springTicksLeft;
//	if (springTicksLeft == 0) {
//		clearSpringEffect();
//	}
//
//	return true;   // we handled movement this frame
//}