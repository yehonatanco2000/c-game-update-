#include "Switch.h"


void Switch::draw(Screen& screen) const {
    char ch = getSymbol();              // Choose symbol based on state
    screen.setChar(getY(), getX(), ch); // Update board array
    gotoxy(getX(), getY());             // Move console cursor to switch position
    std::cout << ch << std::flush;      // Print symbol to screen
}

void Switch::toggle(Screen& screen) {
    on = !on;                           // Flip ON/OFF state
    char ch = getSymbol();              // Get new symbol after toggle
    screen.setChar(getY(), getX(), ch); // Update board cell
    screen.drawChar(getX(), getY(), ch, Color::CYAN); // Draw switch in cyan

    if (on) {                           // If now ON, write "ON" text on status line
        screen.setChar(24, 16, 'O');
        screen.setChar(24, 17, 'N');
        screen.setChar(24, 18, ' ');
        screen.drawChar(getX(), getY(), ch, Color::CYAN);
    }
    else {                              // If now OFF, write "OFF" text on status line
        screen.setChar(24, 16, 'O');
        screen.setChar(24, 17, 'F');
        screen.setChar(24, 18, 'F');
        screen.drawChar(getX(), getY(), ch, Color::CYAN);
    }

    // Draw "ON"/"OFF" indicator at fixed position (x=16..18, y=24)
    screen.drawChar(16, 24, on ? 'O' : 'O', Color::WHITE_BRIGHT);
    screen.drawChar(17, 24, on ? 'N' : 'F', Color::WHITE_BRIGHT);
    screen.drawChar(18, 24, on ? ' ' : 'F', Color::WHITE_BRIGHT);
}