#include <QDebug>
#include <QMessageBox>
#include <QGridLayout>
#include <QShortcut>
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QTextStream>
#include <QRegExp>

#include "sudokuhelper.h"
#include "ui_sudokuhelper.h"

SudokuHelper::SudokuHelper(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::SudokuHelper)
{
    ui->setupUi(this);
    QGridLayout *grid = new QGridLayout;

    int i = 0;
    for (int row = 0; row < SUB; row++) {
        for (int col = 0; col < SUB; col++) {
            QGridLayout *frameLayout = new QGridLayout;
            frames[i].setFrameStyle(QFrame::Panel | QFrame::Raised);
            grid->addWidget(&frames[i], row, col);
            for (int k = 0; k < SUB; k++) {
                for (int l = 0; l < SUB; l++) {
                    int ic = cellNumber(row*SUB+k, col*SUB+l);
                    frameLayout->addWidget(&tiles[ic], k, l);
                }
            }
            frames[i].setLayout(frameLayout);
            frameLayout->setHorizontalSpacing(1);
            frameLayout->setVerticalSpacing(1);
            frameLayout->setContentsMargins(2,2,2,2);
            i++;
        }
    }

    i = 0;
    for (int row = 0; row < DIM; row++) {
        for (int col = 0; col < DIM; col++) {
            tiles[i].setId(i);
            connect(&(tiles[i]), SIGNAL(valueChanged(int,int)), this, SLOT(updateField(int,int)));
            connect(&(tiles[i]), SIGNAL(restoreMe(int,int)), this, SLOT(restoreField(int,int)));
            connect(&(tiles[i]), SIGNAL(moveFocus(int, int)), this, SLOT(moveFocus(int, int)));
            i++;
        }
    }

    QWidget *window = new QWidget();
    window->setLayout(grid);
    grid->setHorizontalSpacing(1);
    grid->setVerticalSpacing(1);

    // Set QWidget as the central layout of the main window
    setCentralWidget(window);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    tiles[0].setFocus();
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close()));
}

SudokuHelper::~SudokuHelper()
{
    delete ui;
}

void SudokuHelper::updateField(int cell, int value)
{
    int row = rowNumber(cell);
    int col = colNumber(cell);

    // update row
    for (int i = 0; i < DIM; i++) {
        int ic = cellNumber(row, i);
        if (ic != cell) tiles[ic].setCell(value, false);
    }
    // update column
    for (int i = 0; i < DIM; i++) {
        int ic = cellNumber(i, col);
        if (ic != cell) tiles[ic].setCell(value, false);
    }
    // update subframe
    row = SUB*(row/SUB);
    col = SUB*(col/SUB);
    for (int k = 0; k < SUB; k++) {
        for (int l = 0; l < SUB; l++) {
            int ic = cellNumber(row+k, col+l);
            if (ic != cell) tiles[ic].setCell(value, false);
        }
    }
}

void SudokuHelper::on_actionAbout_triggered()
{
    QMessageBox::about(this, "About sudoku helper", "Hello Sudoku!");
}

void SudokuHelper::on_actionQuit_triggered()
{
    close();
}

void SudokuHelper::on_actionLock_triggered()
{
    for (int ic = 0; ic < DIM*DIM; ic++) {
        if (tiles[ic].isSolved()) tiles[ic].lock();
    }
}

void SudokuHelper::on_actionClear_triggered()
{
    for (int ic = 0; ic < DIM*DIM; ic++) {
        tiles[ic].reset();
    }
    for (int ic = 0; ic < DIM*DIM; ic++) {
        if (tiles[ic].isLocked()) {
            updateField(ic, tiles[ic].getValue());
        }
    }

}

void SudokuHelper::restoreField(int cell, int value)
{
    int row = rowNumber(cell);
    int col = colNumber(cell);
    // update row
    for (int i = 0; i < DIM; i++) {
        int ic = cellNumber(row, i);
        tiles[ic].setCell(value, true);
    }
    // update column
    for (int i = 0; i < DIM; i++) {
        int ic = cellNumber(i, col);
        tiles[ic].setCell(value, true);
    }
    // update subframe
    row = SUB*(row/SUB);
    col = SUB*(col/SUB);
    for (int k = 0; k < SUB; k++) {
        for (int l = 0; l < SUB; l++) {
            int ic = cellNumber(row+k, col+l);
            tiles[ic].setCell(value, true);
        }
    }
}

void SudokuHelper::moveFocus(int cell, int steps)
{
    cell += steps;
    if (cell < 0 || cell >= DIM*DIM) setFocus();
    else                 tiles[cell].setFocus();
}

void SudokuHelper::on_action_Hint_triggered()
{
    int value, ic;

    for (int ic = 0; ic < DIM*DIM; ic++) {
        if ((value = tiles[ic].unique()) > 0) {
            // tiles[ic].setSolved(value);
            tiles[ic].setFocus();
            qDebug() << "unique value" << rowNumber(ic) << colNumber(ic) << value;
            return;
        }
    }

    for (value = 1; value <= 9; value++) {
        for (int i = 0; i < DIM; i++) {
            if ((ic = possibleRow(i, value)) != -1) {
                // tiles[ic].setSolved(value);
                tiles[ic].setFocus();
                qDebug() << "possible row" << rowNumber(ic) << colNumber(ic) << value;
                return;
            }
            if ((ic = possibleCol(i, value)) != -1) {
                // tiles[ic].setSolved(value);
                tiles[ic].setFocus();
                qDebug() << "possible col" << rowNumber(ic) << colNumber(ic) << value;
                return;
            }
            if ((ic = possibleSub(i, value)) != -1) {
                // tiles[ic].setSolved(value);
                tiles[ic].setFocus();
                qDebug() << "possible sub" << rowNumber(ic) << colNumber(ic) << value;
                return;
            }
        }
    }
}

int SudokuHelper::possibleRow(int row, int value)
{
    int count = 0, which = -1;
    for (int col = 0; col < DIM; col++) {
        int ic = cellNumber(row, col);
        if (!tiles[ic].isSolved() && tiles[ic].isPossible(value)) {
            count++;
            which = ic;
        }
    }
    if (count == 1) return which;
    return -1;
}

int SudokuHelper::possibleCol(int col, int value)
{
    int count = 0, which = -1;
    for (int row = 0; row < DIM; row++) {
        int ic = cellNumber(row, col);
        if (!tiles[ic].isSolved() && tiles[ic].isPossible(value)) {
            count++;
            which = ic;
        }
    }
    if (count == 1) return which;
    return -1;
}

int SudokuHelper::possibleSub(int sub, int value)
{
    int count = 0, which = -1;
    int row = SUB*(sub/SUB);
    int col = SUB*(sub%SUB);
    for (int k = 0; k < SUB; k++) {
        for (int l = 0; l < SUB; l++) {
            int ic = cellNumber(row+k, col+l);
            if (!tiles[ic].isSolved() && tiles[ic].isPossible(value)) {
                count++;
                which = ic;
            }
        }
    }
    if (count == 1) return which;
    return -1;
}

void SudokuHelper::test()
{
    tiles[ 4].setSolved(8);
    tiles[ 8].setSolved(4);
    tiles[ 9].setSolved(1);
    tiles[13].setSolved(3);
    tiles[14].setSolved(7);
    tiles[15].setSolved(9);
    tiles[18].setSolved(4);
    tiles[19].setSolved(3);
    tiles[20].setSolved(9);
    tiles[21].setSolved(2);
    tiles[24].setSolved(1);
    tiles[27].setSolved(9);
    tiles[29].setSolved(2);
    tiles[30].setSolved(5);
    tiles[31].setSolved(1);
    tiles[37].setSolved(8);
    tiles[39].setSolved(9);
    tiles[42].setSolved(5);
    tiles[47].setSolved(1);
    tiles[49].setSolved(2);
    tiles[51].setSolved(7);
    tiles[53].setSolved(3);
    tiles[54].setSolved(8);
    tiles[58].setSolved(5);
    tiles[59].setSolved(1);
    tiles[60].setSolved(3);
    tiles[62].setSolved(9);
    tiles[65].setSolved(3);
    tiles[70].setSolved(5);
    tiles[71].setSolved(1);
    tiles[72].setSolved(7);
    tiles[74].setSolved(5);
    tiles[77].setSolved(2);
    tiles[78].setSolved(4);
    tiles[80].setSolved(8);
    setFocus();
}

void SudokuHelper::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open sudoku", QDir::homePath()+"C/QT/SudokuHelper", "");
    if (!fileName.isNull()) {
        QFile sudoku(fileName);
        if (sudoku.open(QIODevice::ReadOnly)) {
            QTextStream in(&sudoku);
            QString line;
            while (!in.atEnd()) {
                line += in.readLine();
            }
            line.remove(QRegExp("[^.0123456789]"));
            // qDebug() << line << line.length();
            if (line.length() == DIM*DIM) {
                for (int cell = 0; cell < DIM*DIM; cell++) {
                    QChar next = line.at(cell);
                    if ((next == '.') || (next == '0')) continue;
                    int value = next.toLatin1() - '0';
                    // qDebug() << cell << value;
                    tiles[cell].setSolved(value);
                    tiles[cell].lock();
                }
            }
        }
    }
}
