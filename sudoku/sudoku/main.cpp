#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "sudokuclass.h"
#include <QTextEdit>
#include <QGridLayout>
#include <QLine>
#include <QPushButton>
#include "qsudoku.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "sudoku_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    // Create a widget
    QWidget *w = new QSudoku();
    // Window title
    w->setWindowTitle("Sudoku");
    // Display
    w->show();

    // Event loop
    return a.exec();
}

int mainOld(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "sudoku_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    QGridLayout *mainLayout = new QGridLayout;
    QGridLayout *gridLayout = new QGridLayout;
    int base = 3;
    int board_size = base*base;
    SudokuGame game = SudokuGame(base);
    game.setup_map();
    std::map<int,std::map<int,QTextEdit*>> display_grid;
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
    QGridLayout *buttonLayout = new QGridLayout;
    QPushButton *button_submit = new QPushButton("Submit");
    QPushButton *button_solve = new QPushButton("Solve");
    buttonLayout->addWidget(button_submit,0,0);
    buttonLayout->addWidget(button_solve,0,1);
    mainLayout->addLayout(gridLayout,0,0);
    mainLayout->addLayout(buttonLayout,1,0);
    game.print_map();

    // Create a widget
    QWidget *w = new QWidget();

    // Set the grid layout as a main layout
    w->setLayout(mainLayout);

    // Window title
    w->setWindowTitle("Sudoku");
    // Display
    w->show();

    // Event loop
    return a.exec();


    MainWindow mw;
    mw.show();
    return a.exec();
}
