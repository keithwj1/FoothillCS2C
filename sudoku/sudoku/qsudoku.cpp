#include "qsudoku.h"
#include "sudokuclass.h"
#include <QTextEdit>
#include <QGridLayout>
#include <QLine>
#include <QPushButton>
#include <QButtonGroup>
QSudoku::QSudoku(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QGridLayout;
    gridLayout = new QGridLayout;
    base = 3;
    board_size = base*base;
    game = SudokuGame(base);
    game.setup_map();
    for (int row = 0; row < board_size; row++){
        for (int col =0; col < board_size; col++){
            display_grid[row][col] = new QTextEdit();
            if (game.map_board[row][col] != 0){
                display_grid[row][col]->setText(QString::number(game.map_board[row][col]));
                display_grid[row][col]->setReadOnly(true);
            }
            gridLayout->addWidget(display_grid[row][col],row,col,1,1);
        }
    }
    buttonLayout = new QGridLayout;
    button_submit = new QPushButton("Submit");
    button_solve = new QPushButton("Solve");
    buttonLayout->addWidget(button_submit,0,0);
    buttonLayout->addWidget(button_solve,0,1);
    mainLayout->addLayout(gridLayout,0,0);
    mainLayout->addLayout(buttonLayout,1,0);
    setWindowTitle("Sudoku");
}
QSudoku::~QSudoku(){
    delete mainLayout;
    delete gridLayout;
    delete buttonLayout;
    delete button_submit;
    delete button_solve;
    for (int row = 0; row < board_size; row++){
        for (int col =0; col < board_size; col++){
            delete display_grid[row][col];
        }
    }
}
void QSudoku::on_button_submit_clicked()
{
    game.solve();
}

void QSudoku::on_button_solve_clicked()
{
    game.solve(game.map_board);
    SetMapValues(game.map_board);
}
void QSudoku::SetMapValues(std::map<int,std::map<int,int>> &map){
    for (int row = 0; row < board_size; row++){
        for (int col = 0; col < board_size; col++){
            if (map[row][col] != 0){
                display_grid[row][col]->setText(QString::number(map[row][col]));
            }
        }
    }
}
