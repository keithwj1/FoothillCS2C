#ifndef QSUDOKU_H
#define QSUDOKU_H

#include <QWidget>
#include "SudokuClass.h"
#include <SudokuClass.h>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QGroupBox>
#include <QSudokuTextEdit.h>
#include <QInputDialog>
#include <QApplication>
class QSudoku : public QWidget
{
    Q_OBJECT

public:
    QSudoku(QWidget *parent = nullptr);
    ~QSudoku();
    QScrollArea *scrollArea;


    int base = 3;
    int board_size = base*base;
    SudokuGame game;
    std::vector<QLine*> lines;
    std::vector<QGroupBox*> boxes;
    QGridLayout *mainLayout;
    QGridLayout *gridLayout;
    std::vector<QGridLayout*> box_grid_layouts;
    std::map<int,std::map<int,QSudokuTextEdit*>> display_grid;
    QGridLayout *buttonLayout;
    QPushButton *button_submit;
    QPushButton *button_solve;
    QPushButton *button_visual_solve;
    QPushButton *button_hint;
    bool board_read_only();
    void SetHintCell(QTextEdit * cell, int value);
    void on_button_submit_clicked();
    void on_button_solve_clicked();
    void on_button_hint_clicked();
    void on_button_visual_solve_clicked();
    Board get_user_map();
    bool visual_solve_smart(Board &map, int row, int col);
    bool visual_solve(Board &map,int row, int col);
    void SetMapValues(Board &map);
};
#endif // QSUDOKU_H
