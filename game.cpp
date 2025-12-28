#include "game.h"
#include "Menu.h"
#include "player.h"
#include "Directions.h"
#include <Windows.h>
#include <conio.h>
#include "Screen.h"
#include <iostream>
#include "Riddle.h"
#include <algorithm>
#include <queue>

void game::start_game() {
	hideCursor();                            // Hide console cursor during the game
	constexpr char ESC = 27;                // ESC key code
	theScreen.draw();                       // Initial draw of the whole screen

	while (true) {
		theScreen.setTorchInfo(
			p1.getCarryItem() == '!',   // player 1 holds torch?
			p2.getCarryItem() == '!',   // player 2 holds torch?
			p1.getX(), p1.getY(),
			p2.getX(), p2.getY()
		);
		theScreen.draw();                   // Redraw full screen each frame

		for (auto& b : bombs) {             // Update all active bombs
			if (!b.alive) continue;        // Skip bombs that already exploded
			gotoxy(b.x, b.y);              // Move cursor to bomb position
			std::cout << b.timer << std::flush; // Show remaining timer
			Sleep(50);                     // Small delay for visual effect
			if (--b.timer == 0) {          // Decrease timer and check explosion
				explodeAt(b.x, b.y);       // Trigger explosion effect
				b.alive = false;           // Mark bomb as dead
			}
		}

		if (p1.getIsActive()) {
			p1.move();                      // Move player 1 if still active
		}

		if (p2.getIsActive()) {
			p2.move();                      // Move player 2 if still active
		}

		Sleep(20);                          // Control game speed

		if (_kbhit()) {                     // Check if any key was pressed
			char key = _getch();            // Read key

			if ((unsigned char)key == 224 || (unsigned char)key == 0) {
				// Special key (arrows / function keys) – consume extra byte and ignore
				_getch();
				// Do not call keyPressed for arrow keys
			}

			if (key == ESC) {               // Pause menu when ESC is pressed
				int action = pause_menu();
				if (action == 1) {          // Return to main menu
					cls();
					Menu show_main_menu;
					show_main_menu.show_menu();
					break;                  // Exit game loop
				}
				else {                      // Continue game
					theScreen.draw();
				}
			}
			else {
				p1.keyPressed(key);         // Send key to player 1
				p2.keyPressed(key);         // Send key to player 2
			}
		}
	}
	return;                                 // End of start_game
}

void game::initObjectsFromBoard()
{
	riddles.clear();                        // Remove old riddles from previous room
	nextRiddleIndex = 0;                    // Reset riddle index counter

	for (int y = 0; y <= Screen::MAX_Y; ++y) {
		for (int x = 0; x <= Screen::MAX_X; ++x) {
			char ch = theScreen.board[y][x]; // Character at current board cell

			if (ch == 'k') {
				keys.emplace_back(x, y);     // Add key object at this position
			}
			if (ch == '?') {
				int idx = nextRiddleIndex % NUM_RIDDLES;   // Cycle through all riddles
				riddles.emplace_back(x, y, RIDDLES[idx]);  // Create riddle at (x,y)
				++nextRiddleIndex;
			}
			else if (ch == '/' || ch == '\\') {            // Found a switch
				bool is_on = (ch == '/');                  // '/' means ON
				if (is_on) {
					theScreen.setChar(24, 16, 'O');          // Write "ON" indication
					theScreen.setChar(24, 17, 'N');
					gotoxy(16, 24);
					std::cout << "ON" << std::flush;
				}
				else {
					theScreen.setChar(24, 16, 'O');        // Write "OFF" indication
					theScreen.setChar(24, 17, 'F');
					theScreen.setChar(24, 18, 'F');
					gotoxy(16, 24);
					std::cout << "OFF" << std::flush;
				}
				if (currentRoom == 1) {
					switches.emplace_back(x, y, is_on, SwitchType::DOOR); // Switch controls door
				}
				else {
					switches.emplace_back(x, y, is_on, SwitchType::WALL); // Switch controls wall
				}
			}
			else if (ch == '@') {
				bombItem.emplace_back(x, y);               // Bomb item on floor
			}
			else if (ch >= '1' && ch <= '9') {
				doors.emplace_back(x, y, ch);              // Door with required keys
			}
			else if (ch == '!') {
				torches.emplace_back(x, y);   // Add torch at this position
			}
			else if (ch == '#') {
				springs.emplace_back(x, y);  // add spring cell at this position
			}
			else if (ch == '*') {  // מכשול
				// צור מכשול חדש רק אם לא נמצא כבר
				bool alreadyExists = false;
				for (const auto& obs : obstacles) {
					if (obs.contains(x, y)) {
						alreadyExists = true;
						break;
					}
				}
				if (!alreadyExists) {
					obstacles.emplace_back(x, y);  // רק פעם אחת לכל רצף!
				}
			}

		}
	}
}

void game::handleStepOnObjects(player& pl) {
	int x = pl.getX();                     // Player current x
	int y = pl.getY();                     // Player current y

	// --- Handle switches ---
	for (size_t i = 0; i < switches.size(); ++i) {
		if (switches[i].getX() == x && switches[i].getY() == y) {

			// Toggle switch state and update display
			switches[i].toggle(theScreen);

			// For room 2: switch that controls door '1'
			if (switches[i].getType() == SwitchType::DOOR && currentRoom == 1) {

				bool doorOpen = false;     // Final decision if door is open

				for (auto& d : doors) {
					if (d.getSymbol() == '1') {          // Find door '1'
						bool enoughKeys = d.isOpen();    // Check keys logic
						bool switchOn = switches[i].isOn();
						doorOpen = (enoughKeys && switchOn); // Both conditions must be true
						break;
					}
				}

				if (doorOpen) {            // Show "YES" near status area
					theScreen.setChar(23, 23, 'Y');
					theScreen.setChar(23, 24, 'E');
					theScreen.setChar(23, 25, 'S');
					theScreen.drawChar(23, 23, 'Y', Color::WHITE_BRIGHT);
					theScreen.drawChar(23, 24, 'E', Color::WHITE_BRIGHT);
					theScreen.drawChar(23, 25, 'S', Color::WHITE_BRIGHT);
					gotoxy(23, 23);
					std::cout << "YES" << std::flush;
				}
				else {                     // Show "NO " if door still closed
					theScreen.setChar(23, 23, 'N');
					theScreen.setChar(23, 24, 'O');
					theScreen.setChar(23, 25, ' ');
					theScreen.drawChar(23, 23, 'N', Color::WHITE_BRIGHT);
					theScreen.drawChar(23, 24, 'O', Color::WHITE_BRIGHT);
					theScreen.drawChar(23, 25, ' ', Color::WHITE_BRIGHT);
					gotoxy(23, 23);
					std::cout << "NO " << std::flush;
				}
			}

			// Apply wall effect for room 0
			if (switches[i].getType() == SwitchType::WALL && currentRoom == 0) {
				update_Internal_Wall_By_Switch(switches[i]);
			}
			break;                         // Only one switch per tile
		}
	}

	// --- Handle riddles ---
	for (size_t i = 0; i < riddles.size(); ++i) {
		if (riddles[i].getX() == x && riddles[i].getY() == y) {

			bool solved = riddles[i].ask();   // Show riddle and get answer

			if (solved) {
				std::cout << "Correct answer!" << std::flush;
				Sleep(1000);
				// Correct answer: remove riddle mark from screen and vector
				theScreen.setChar(y, x, ' ');
				riddles.erase(riddles.begin() + i);
			}
			else {
				std::cout << "Wrong answer! Returning to start position." << std::flush;
				Sleep(1000);
				// Wrong answer: reset player to starting position in this room
				pl.setPosition(pl.get_startpointX(), pl.get_startpointY());
			}
			theScreen.draw();              // Redraw room after change
		}
	}

	// --- Handle doors ---
	for (size_t i = 0; i < doors.size(); ++i) {
		if (doors[i].getX() == x && doors[i].getY() == y) {

			// If player holds a key (represented as 'k'), give it to the door
			if (pl.iscarrying_item() && pl.getCarryItem() == 'k') {
				pl.useKey();               // Remove key from player
				doors[i].addKey();        // Increase door key counter
			}
			bool canopen = doors[i].isOpen(); // Door open according to its internal logic
			bool hasSwitchPermission = false;

			for (const auto& sw : switches) {
				if (currentRoom == 1) {
					if (sw.getType() == SwitchType::DOOR && sw.isOn()) {
						hasSwitchPermission = true;   // Switch allows opening
						break;
					}
					canopen = canopen && hasSwitchPermission; // Need both: keys + switch
				}
			}

			if (canopen) {
				theScreen.setChar(23, 23, 'Y');
				theScreen.setChar(23, 24, 'E');
				theScreen.setChar(23, 25, 'S');
				gotoxy(23, 23);
				std::cout << "YES" << std::flush;
				pl.deactivate();          // Player finished this room
				playerPassedDoor(pl);     // Handle room exit logic
			}

			// Door remains on map and vector; just stop further checks
			return;
		}
	}

	// --- Handle picking up items (keys, bombs, torches) ---
	if (!pl.iscarrying_item()) {
		char before = theScreen.board[y][x]; // What was on this tile
		if (pl.pickupIfPossible()) {         // Player successfully picked up item

			if (before == 'k') {             // Remove key from keys vector
				for (size_t i = 0; i < keys.size(); ++i) {
					if (keys[i].getX() == x && keys[i].getY() == y) {
						keys.erase(keys.begin() + i);
						break;
					}
				}
			}
			else if (before == '@') {        // Remove bomb item from bombItem vector
				for (size_t i = 0; i < bombItem.size(); ++i) {
					if (bombItem[i].getX() == x && bombItem[i].getY() == y) {
						bombItem.erase(bombItem.begin() + i);
						break;
					}
				}
			}
			else if (before == '!') {        // Remove torch item from torch vector
				for (size_t i = 0; i < torches.size(); ++i) {
					if (torches[i].getX() == x && torches[i].getY() == y) {
						torches.erase(torches.begin() + i);
						break;
					}
				}
			}
		}
	}
	activateSpringIfNeeded(pl);
}

void game::goToNextRoom() {
	++currentRoom;                              // Move to next room index
	if (currentRoom >= 2) {
		// For rooms beyond last: just load and draw final room
		theScreen.loadRoom(currentRoom);
		theScreen.draw();
		return;
	}

	theScreen.loadRoom(currentRoom);            // Replace board with new room
	p1.getIsActiveback();                       // Reactivate player 1
	p2.getIsActiveback();                       // Reactivate player 2
	keys.clear();                               // Clear all old objects
	doors.clear();
	bombItem.clear();
	switches.clear();
	obstacles.clear();                          //clear all old objects
	initObjectsFromBoard();                     // Scan new board for objects

	// Reset player positions for new room
	setposition_player1(2, 3);
	p1.setStartPosition(2, 3);                  // New starting position for player 1
	setposition_player2(77, 1);
	p2.setStartPosition(77, 1);                 // New starting position for player 2
	theScreen.draw();                           // Draw whole new room
	p1.draw('$');                               // Draw player 1 character
	p2.draw('&');                               // Draw player 2 character
}

int game::pause_menu()
{
	cls();                                      // Clear screen and show pause menu
	std::cout << "============ PAUSED ============" << std::endl;
	std::cout << "      ESC - continue game       " << std::endl;
	std::cout << "       H  - return to main menu " << std::endl;
	std::cout << "       I - view instructions    " << std::endl;
	std::cout << "================================" << std::endl;

	while (true) {
		int ch = _getch();                     // Wait for user choice
		if (ch == 27) {                        // ESC -> continue game
			return 0;
		}
		else if (ch == 'h' || ch == 'H') {     // h/H -> go back to main menu
			return 1;
		}
		else if (ch == 'i' || ch == 'I') {     // i/I -> show instructions
			cls();
			std::cout << "=============================== Game Instructions ==============================" << std::endl;
			std::cout << "|                                                                              |" << std::endl;
			std::cout << "|1.There are two players participating in the game world.                      |" << std::endl;
			std::cout << "|2.The goal of each player is to reach the end of the level and solve all      |" << std::endl;
			std::cout << "|  challenges.                                                                 |" << std::endl;
			std::cout << "|3.Players can move using the following keys:                                  |" << std::endl;
			std::cout << "|     Player 1: W (Up), A (Left), S (Stay), D (Right), X (Down), E (Dispose)   |" << std::endl;
			std::cout << "|     Player 2: I (Up), J (Left), K (Stay), L (Right), M (Down), O (Dispose)   |" << std::endl;
			std::cout << "|4.You will encounter obstacles, doors, keys, springs,bombs,and other game     |" << std::endl;
			std::cout << "|  elements.                                                                   |" << std::endl;
			std::cout << "|5.Collaboration is required to overcome obstacles and open new paths.         |" << std::endl;
			std::cout << "|6.Good luck!                                                                  |" << std::endl;
			std::cout << "|                                                                              |" << std::endl;
			std::cout << "| *for back press b*                                                           |" << std::endl;
			std::cout << "================================================================================" << std::endl;

			char press = 'b';                  // Wait until user presses 'b'
			while (press != _getch()) {};
			cls();                             // Redraw pause menu after returning
			std::cout << "============ PAUSED ============" << std::endl;
			std::cout << "      ESC - continue game       " << std::endl;
			std::cout << "       H  - return to main menu " << std::endl;
			std::cout << "       I - view instructions    " << std::endl;
			std::cout << "================================" << std::endl;
		}
	}
}

//perplexity AI
bool game::isPlayerAt(int x, int y, const player* ignore) const {
	const player* other = nullptr;             // Pointer to the other player

	if (ignore == &p1)
		other = &p2;
	else if (ignore == &p2)
		other = &p1;
	else
		return false;                           // ignore is not p1 or p2

	return other->getIsActive() &&             // Other player must be active
		other->getX() == x &&                  // Same x
		other->getY() == y;                    // Same y
}

bool game::isLastActivePlayer(const player& pl) const {
	int activeCount = 0;                       // Count how many players are active
	if (p1.getIsActive()) ++activeCount;
	if (p2.getIsActive()) ++activeCount;

	return (activeCount == 1) && pl.getIsActive(); // True if only this player is active
}

void game::playerPassedDoor(player& pl) {
	// Hide player from current position on screen
	pl.draw(' ');

	++playersExitedThisRoom;                   // Increment counter of players who left

	// If first player exited, keep room; play continues with remaining player
	if (playersExitedThisRoom == 1) {
		return;
	}

	// If second player also exited, load next room
	if (playersExitedThisRoom == 2) {
		goToNextRoom();
		playersExitedThisRoom = 0;             // Reset counter for the new room
	}
}

void game::explodeAt(int x, int y) {
	// First handle visual and board effects
	theScreen.explodeAt(x, y);

	// Then remove riddles within distance 3 from explosion center
	for (int dy = -3; dy <= 3; ++dy) {
		for (int dx = -3; dx <= 3; ++dx) {
			int nx = x + dx;
			int ny = y + dy;

			if (nx < 0 || nx > Screen::MAX_X || ny < 0 || ny > Screen::MAX_Y)
				continue;                       // Skip out-of-bounds cells

			for (size_t i = 0; i < riddles.size(); ++i) {
				if (riddles[i].getX() == nx && riddles[i].getY() == ny) {
					riddles.erase(riddles.begin() + i); // Remove riddle in blast radius
					break;
				}
			}
			for (size_t i = 0; i < doors.size(); ++i) {
				if (doors[i].getX() == nx && doors[i].getY() == ny) {
					doors.erase(doors.begin() + i); // Remove door in blast radius
					break;
				}
			}

			for (size_t i = 0; i < obstacles.size(); ++i) {
				if (obstacles[i].contains(nx, ny)) {
					obstacles.erase(obstacles.begin() + i);
					break;
				}
			}

		}
	}
	const int explosionRadius = 3;    
	// For player 1
	int dx1 = p1.getX() - x;
	int dy1 = p1.getY() - y;
	int life_p1 = p1.getLife();
	if (abs(dx1) + abs(dy1) <= explosionRadius) {
		theScreen.setChar(21, 54, life_p1 - 1 + '0');
		theScreen.drawChar(54, 21, life_p1 - 1 + '0', Color::WHITE_BRIGHT);
		p1.setLife(life_p1 - 1);
		life_p1 = p1.getLife();
		if (life_p1 <= 0)
			p1.deactivate();
	}

	// For player 2
	int dx2 = p2.getX() - x;
	int dy2 = p2.getY() - y;
	int life_p2 = p2.getLife();
	if (abs(dx2) + abs(dy2) <= explosionRadius) {
		theScreen.setChar(22, 54, life_p2 - 1 + '0');
		theScreen.drawChar(54, 22, life_p2 - 1 + '0', Color::WHITE_BRIGHT);
		p2.setLife(life_p2 - 1);
		life_p2 = p2.getLife();
		if (life_p2 <= 0)
			p2.deactivate();
	}
}

void game::update_Internal_Wall_By_Switch(const Switch& sw) {
	const int y = 15;                          // Fixed row of internal wall
	const int xStart = 29;                     // Start x of wall segment
	const int xEnd = 32;                       // End x of wall segment

	if (currentRoom != 0)
		return;                                 // Safety: only in room 0

	if (sw.isOn()) {
		// ON -> remove wall (replace with spaces)
		for (int x = xStart; x <= xEnd; ++x) {
			theScreen.setChar(y, x, ' ');
			gotoxy(x, y);
			std::cout << ' ' << std::flush;
		}
	}
	else {
		// OFF -> restore wall (character 'W')
		for (int x = xStart; x <= xEnd; ++x) {
			theScreen.setChar(y, x, 'W');
			theScreen.drawChar(x, y, 'W', Color::LIGHT_CYAN);
		}
	}
}

player* game::getOtherPlayer(const player* pl) {
	if (pl == &p1) return &p2;
	if (pl == &p2) return &p1;
	return nullptr;
}

int game::countSpringCompressed(const player& pl) const
{
	Direction d = pl.getPoint().getDir();
	int dx = d.dx();
	int dy = d.dy();

	if (dx == 0 && dy == 0)
		return 0;

	int x = pl.getX();
	int y = pl.getY();

	int count = 1;

	while (true) {
		x -= dx;
		y -= dy;

		if (x < 0 || x >= Screen::MAX_X ||
			y < 0 || y >= Screen::MAX_Y)
			break;

		char ch = theScreen.board[y][x];

		if (ch == '#') {
			++count;        
		}
		else {
			break;          
		}
	}

	return count;
}


void game::activateSpringIfNeeded(player& pl)
{
	if (pl.isUnderSpringEffect())
		return;

	int x = pl.getX();
	int y = pl.getY();

	// 1. must stand on spring cell
	if (theScreen.board[y][x] != '#')
		return;

	Direction inDir = pl.getPoint().getDir();
	int dx = inDir.dx();
	int dy = inDir.dy();

	if (dx == 0 && dy == 0)
		return; // not moving into spring

	int nextX = x + dx;
	int nextY = y + dy;

	// 2. if next cell is also '#', we are still compressing -> do nothing yet
	if (nextX >= 0 && nextX <= Screen::MAX_X &&
		nextY >= 0 && nextY <= Screen::MAX_Y &&
		theScreen.board[nextY][nextX] == '#')
	{
		return;
	}
	if (theScreen.board[nextY][nextX] == 'W') { 
		int k = countSpringCompressed(pl);      // נשאר לטפל במצב של שינוי כיוון בזמן השיגור(לא אחרי)
		if (k <= 0)
			return;

		// release direction = opposite to compression
		int outDx = -dx;
		int outDy = -dy;
		Direction outDir(outDx, outDy);

		int speed = k;
		int ticks = k * k;


		pl.setSpringEffect(speed, ticks, outDir);
		pl.setDirection(outDir);
	}

}

bool game::tryPushObstacle(point& target, Direction dir) {
    // מוצאים את המכשול שהשחקן הראשון פגע בו
    Obstacle* found = nullptr;
    for (auto& obs : obstacles) {
        if (obs.contains(target.getX(), target.getY())) {
            found = &obs;
            break;
        }
    }
    if (!found)
        return false;

    Obstacle& obs = *found;
    int dx = dir.dx();
    int dy = dir.dy();

    int totalForce = 0;

    // ===== פונקציה פנימית: האם שחקן דוחף את המכשול הזה? =====
    auto contributes = [&](player& pl) -> int {
        if (!pl.getIsActive())
            return 0;

        // חייב לדחוף באותו כיוון
        if (pl.getPoint().getDir().dx() != dx ||
            pl.getPoint().getDir().dy() != dy)
            return 0;

        // התא "לפני" השחקן (הצעד הבא שלו)
        int fx = pl.getX() + dx;
        int fy = pl.getY() + dy;

        // אם התא הזה חלק מהמכשול – השחקן דוחף אותו
        if (!obs.contains(fx, fy))
            return 0;

        // כוחו של השחקן
        return pl.isUnderSpringEffect() ? pl.getSpringSpeed() : 1;
    };

    totalForce += contributes(p1);
    totalForce += contributes(p2);

    // אין מי שדוחף
    if (totalForce <= 0)
        return false;


    // מספיק כוח לשנע את כל המכשול צעד אחד
    if (totalForce >= obs.getSize() && obs.canPush(dir, theScreen)) {
        obs.push(dir, theScreen);
		mergeObstaclesOnBoard();
        return true;
    }

    return false;
}


void game::mergeObstaclesOnBoard()
{
	// ניקוי הרשימה ובנייה מחדש על סמך המסך
	obstacles.clear();

	// מטריצה שמסמנת אם כבר ביקרנו בתא
	bool visited[Screen::MAX_Y + 1][Screen::MAX_X + 1] = { false };

	for (int y = 0; y <= Screen::MAX_Y; ++y) {
		for (int x = 0; x <= Screen::MAX_X; ++x) {
			if (theScreen.board[y][x] == '*' && !visited[y][x]) {
				// BFS על מקבץ מכשול אחד
				std::vector<point> cluster;
				std::queue<std::pair<int, int>> q;
				q.push({ x,y });
				visited[y][x] = true;

				while (!q.empty()) {
					auto [cx, cy] = q.front();
					q.pop();
					cluster.emplace_back(cx, cy, Direction::directions[Direction::STAY], '*');

					const int dx[4] = { 1,-1,0,0 };
					const int dy[4] = { 0,0,1,-1 };

					for (int k = 0; k < 4; ++k) {
						int nx = cx + dx[k];
						int ny = cy + dy[k];
						if (nx < 0 || nx > Screen::MAX_X || ny < 0 || ny > Screen::MAX_Y)
							continue;
						if (theScreen.board[ny][nx] == '*' && !visited[ny][nx]) {
							visited[ny][nx] = true;
							q.push({ nx, ny });
						}
					}
				}

				// יצרנו cluster שלם – עכשיו בונים Obstacle חדש ממנו
				Obstacle newObs(0, 0);       // נשתמש בבנאי קיים ואז נדרוס אותו
				newObs.getCells().clear();
				newObs.setSize(0);

				for (auto& p : cluster) {
					newObs.getCells().push_back(p);
				}
				newObs.setSize((int)cluster.size());

				obstacles.push_back(newObs);
			}
		}
	}
}




//int game::getPushForce(Direction dir ,Screen& screen) const {
//	int force = 0;
//
//	int dx = dir.dx();
//	int dy = dir.dy();
//
//	int px1 = p1.getX();
//	int py1 = p1.getY();
//	point front1(px1,py1, { 0,0 }, ' ');
//	bool p1TouchesObstacle = screen.isobstacle(front1);
//
//	// שחקן 1
//	if (p1.getIsActive() &&
//		p1.getPoint().getDir().dx() == dx &&
//		p1.getPoint().getDir().dy() == dy && p1TouchesObstacle) {
//
//		force += p1.isUnderSpringEffect() ? p1.getSpringSpeed() : 1;
//	}
//	int px2 = p2.getX();
//	int py2 = p2.getY();
//	point front2(px2, py2, { 0,0 }, ' ');
//	bool p2TouchesObstacle = screen.isobstacle(front2);
//
//	// שחקן 2
//	if (p2.getIsActive() &&
//		p2.getPoint().getDir().dx() == dx &&
//		p2.getPoint().getDir().dy() == dy && p2TouchesObstacle) {
//
//		force += p2.isUnderSpringEffect() ? p2.getSpringSpeed() : 1;
//	}
//
//	return force;
//}
//





