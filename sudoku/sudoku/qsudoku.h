#ifndef QSUDOKU_H
#define QSUDOKU_H

#include <QWidget>
#include "sudokuclass.h"
#include <QTextEdit>
#include <QGridLayout>
#include <QLine>
#include <QPushButton>
#include <QButtonGroup>
class QSudoku : public QWidget
{
    Q_OBJECT
public:
    int base = 3;
    int board_size = base*base;
    SudokuGame game;
    QGridLayout *mainLayout;
    QGridLayout *gridLayout;
    std::map<int,std::map<int,QTextEdit*>> display_grid;
    QGridLayout *buttonLayout;
    QPushButton *button_submit;
    QPushButton *button_solve;
    explicit QSudoku(QWidget *parent = nullptr);
    ~QSudoku();
    void on_button_submit_clicked();
    void on_button_solve_clicked();
    void SetMapValues(std::map<int,std::map<int,int>> &map);
signals:

};

#endif // QSUDOKU_H
