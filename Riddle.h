#pragma once

#include <string>

struct RiddleData {
    std::string question;          // Full text of the riddle question
    std::string options[4];        // Four possible answers to choose from
    int correctIndex;              // Index (0-3) of the correct answer
};

extern const RiddleData RIDDLES[]; // Global array with all riddle data
extern const int NUM_RIDDLES;      // Number of riddles in the array

class Riddle {
    int x, y;                      // Position of the riddle trigger on the board
    std::string question;          // Stored question text for this riddle
    std::string lines[4];          // Stored answer options for this riddle
    int correctIndex;              // Index of the correct answer

public:
    Riddle(int x, int y, const RiddleData& data) // Constructor: build riddle from RiddleData
        : x(x), y(y),
        question(data.question),
        correctIndex(data.correctIndex)
    {
        for (int i = 0; i < 4; ++i)
            lines[i] = data.options[i];         // Copy all answer options
    }
    int getX() const { return x; }              // Get riddle x-coordinate
    int getY() const { return y; }              // Get riddle y-coordinate
    bool ask();                                 // Show riddle to player and return true if correct
};



