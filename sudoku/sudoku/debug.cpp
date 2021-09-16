#include <iostream>
#include "sudokuclass.h"

int main(int argc, char *argv[])
{
    SudokuGame game = SudokuGame();
    game.setup();
    game.print();
}