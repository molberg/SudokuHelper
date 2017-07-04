#include <QMessageBox>
#include <QGridLayout>
#include <QShortcut>

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
            connect(&(tiles[i]), SIGNAL(moveFocus(int)), this, SLOT(moveFocus(int)));
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
    new QShortcut(QKeySequence(Qt::Key_F3), this, SLOT(on_actionLock_triggered()));
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

void SudokuHelper::moveFocus(int cell)
{
    cell += 1;
    if (cell >= DIM*DIM) setFocus();
    else                 tiles[cell].setFocus();
}
