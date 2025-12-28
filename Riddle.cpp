#include "Riddle.h"
#include "utils.h"
#include <iostream>
#include <conio.h>


const RiddleData RIDDLES[] = {
    {   // חידה 0
        "What is the output?\n\n#define WHAT(x,y) (x*2) >= y ? (x/2) : (y*2)\n\n void main() {\n  float a;\n  a = WHAT(WHAT(1+4,1+5),1+6);\n  printf(%.0f,a);\n}",
        { "3", "13", "2", "14" },
        1   // התשובה הנכונה היא אפשרות 2: "13"
    },
    {   // חידה 1
        "You are writing a program that consists of several source files\nA global function named: somefunc is fully implemented in two diffrent files\nwhich statements is correct?",
        { "The program will fail to compile in every case.", "The program will fail to link only if the prototypes in the two files are \tdiffrent.", "The program will fail to link only if the function bodies in the two\n\tfiles are diffrent.", "None of the above answers is correct." },
        0   // אפשרות 2
    },
    {   // חידה 2
        "What will be the result of running the following code:\n\n#define X 17\n\n#ifndef X\n    printf('Hello');\n#else\n    printf('World');\n#endif\n\nvoid main(){}",
        { "World", "Hello", "The program does not compile.", "The program will run and will not display any output." },
		2   // אפשרות 3
    }
};

const int NUM_RIDDLES = sizeof(RIDDLES) / sizeof(RIDDLES[0]);  // Total number of riddles


bool Riddle::ask() {
    cls();
                //01234567890123456789012345678901234567890123456789012345678901234567890123456789//
    std::cout << "==================================== Riddle ====================================\n" << std::endl;
    std::cout << question << "\n\n";
    for (int i = 0; i < 4; ++i) {
        std::cout << "   " << (i + 1) << " - " << lines[i] << "\n";
    }
    std::cout << "================================================================================\n";

    int choice = 0;
    while (true) {
        //std::cout << "Choose your answer (1-4): " << std::flush;
        char c = _getch();
        if (c >= '1' && c <= '4') {
            choice = c - '1';
            break;
        }
    }
    return (choice == correctIndex);
}

