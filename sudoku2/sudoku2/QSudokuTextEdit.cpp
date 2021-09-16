#include "QSudokuTextEdit.h"

QSudokuTextEdit::QSudokuTextEdit(QTextEdit *parent) : QTextEdit(parent){
    setAlignment(Qt::AlignCenter);
    setAlignment(Qt::AlignHCenter);
}

void QSudokuTextEdit::keyPressEvent(QKeyEvent *event) {
    if (isReadOnly()){
        return QTextEdit::keyPressEvent(event);
    }
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        setSubmittedCell();
        return;
    }
    else if (event->key() == Qt::Key_Escape) {
        setEntryCell();
        return;
    }
    QTextEdit::keyPressEvent(event);
}

void QSudokuTextEdit::setIntValue(int value){
    setText(QString::number(value));
}

void QSudokuTextEdit::setHintCell(){
    setReadOnly(true);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::lightGray);
    p.setColor(QPalette::Text, Qt::black);
    setPalette(p);
    QTextCursor cursor = textCursor();
    selectAll();
    setAlignment(Qt::AlignCenter);
    setAlignment(Qt::AlignHCenter);
    setFontPointSize(qreal(14.0));
    //setFontUnderline(true);
    setFontWeight(QFont::Bold);
    setTextCursor(cursor);
}

void QSudokuTextEdit::setSubmittedCell(){
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::white);
    p.setColor(QPalette::Text, Qt::black);
    setPalette(p);
    QTextCursor cursor = textCursor();
    selectAll();
    setAlignment(Qt::AlignCenter);
    setAlignment(Qt::AlignHCenter);
    setFontPointSize(qreal(14.0));
    //setFontUnderline(true);
    setFontWeight(QFont::Bold);
    setTextCursor(cursor);
}
void QSudokuTextEdit::setEntryCell(){
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::white);
    p.setColor(QPalette::Text, Qt::blue);
    setPalette(p);
    QTextCursor cursor = textCursor();
    selectAll();
    setAlignment(Qt::AlignLeft);
    setAlignment(Qt::AlignTop);
    setFontPointSize(qreal(12.0));
    //setFontUnderline(false);
    setFontWeight(QFont::Thin);
    setTextCursor(cursor);
}
