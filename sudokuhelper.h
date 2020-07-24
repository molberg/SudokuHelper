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
    void test();
    bool solve();
    void check();

private slots:
    void updateField(int cell, int value);
    void restoreField(int cell, int value);
    void moveFocus(int cell, int steps);
    void unsolvable() { m_solvable = false; }

    void on_actionAbout_triggered();
    void on_actionQuit_triggered();
    void on_actionLock_triggered();
    void on_actionClear_triggered();
    void on_action_Hint_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSolve_triggered();
    void on_actionNew_triggered();

private:
    Ui::SudokuHelper *ui;
    QFrame frames[DIM];
    SudokuTile tiles[DIM*DIM];
    bool m_solvable;
    int rowNumber(int cell) { return cell/DIM; };
    int colNumber(int cell) { return cell%DIM; };
    int cellNumber(int row, int col) { return row*DIM+col; };
    int subNumber(int row, int col) { return (row/SUB)*SUB + col/SUB; };
    int possibleRow(int row, int value);
    int possibleCol(int col, int value);
    int possibleSub(int sub, int value);
    bool possible(int row, int col, int value);
    bool inRow(int row, int value);
    bool inCol(int col, int value);
    bool inSub(int sub, int value);
};

#endif // SUDOKUHELPER_H
