#ifndef SUDOKUHELPER_H
#define SUDOKUHELPER_H

#include <QMainWindow>
#include <QFrame>
#include "sudokutile.h"

namespace Ui {
class SudokuHelper;
}

class SudokuHelper : public QMainWindow
{
    Q_OBJECT

public:
    explicit SudokuHelper(QMainWindow *parent = 0);
    ~SudokuHelper();

private slots:
    void updateField(int cell, int value);
    void restoreField(int cell, int value);
    void moveFocus(int cell);

    void on_actionAbout_triggered();

    void on_actionQuit_triggered();

    void on_actionLock_triggered();

    void on_actionClear_triggered();

private:
    Ui::SudokuHelper *ui;
    QFrame frames[DIM];
    SudokuTile tiles[DIM*DIM];
    int rowNumber(int cell) { return cell/DIM; };
    int colNumber(int cell) { return cell%DIM; };
    int cellNumber(int row, int col) { return row*DIM+col; };
};

#endif // SUDOKUHELPER_H
