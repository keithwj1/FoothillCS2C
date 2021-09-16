/**
 * @file QSudokuTextEdit.h
 *
 * @brief Contains the header for the QTextEdit Class Wrapper
 *
 * @ingroup Sudoku
 * (Note: this needs exactly one @defgroup somewhere)
 *
 * @author Keith Jones
 * Contact: keithwj1@gmail.com
 *
 */

#ifndef QSUDOKUTEXTEDIT_H
#define QSUDOKUTEXTEDIT_H

#include <QObject>
#include <QKeyEvent>
#include <QTextEdit>
//class QSudokuTextEdit : public QObject

/**
 * Implementation of a custom functions for visual formatting
 * Extendes the QTextEdit class
 *
 *
 */
class QSudokuTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    //explicit QSudokuTextEdit(QObject *parent = nullptr); origina declaration
    explicit QSudokuTextEdit(QTextEdit *parent = nullptr);
    /** Handels the event if a key is pressed while the box is selected
    *
    *
    * @param event event that has occurred
    */
    void keyPressEvent(QKeyEvent *event);
    /** Formats the cell as a hint cell
    *
    */
    void setHintCell();
    /** Sets the value of the cell
    *
    * If no verbosity is specified, inherit from parent
    *
    * @param name component of interest
    * @param verbosity desired trace verbosity
    */
    void setIntValue(int value);
    void setSubmittedCell();
    void setEntryCell();

signals:

};

#endif // QSUDOKUTEXTEDIT_H
