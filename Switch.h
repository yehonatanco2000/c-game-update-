#pragma once
#include "point.h"
#include "Screen.h"
#include "utils.h"
#include <iostream>

enum class SwitchType {
    DOOR,      // Switch controls a door
    WALL       // Switch controls an internal wall
};

class Switch
{
    int x, y;              // Switch position on the board
    bool on;               // Current state: true = ON, false = OFF
    SwitchType type;       // What this switch affects (door or wall)

public:
    Switch(int x, int y, bool is_on, SwitchType t)
        : x(x), y(y), on(is_on), type(t)
    {
    }                     // Constructor: set position, initial state and type

    int getX() const { return x; }          // Get switch x-coordinate
    int getY() const { return y; }          // Get switch y-coordinate
    bool isOn() const { return on; }        // Check if switch is currently ON
    SwitchType getType() const { return type; } // Get what this switch controls

    char getSymbol() const {
        return on ? '/' : '\\';             // Character to draw: '/' for ON, '\' for OFF
    }

    void draw(Screen& screen) const;

    void toggle(Screen& screen); // Toggle switch state and update display

};


