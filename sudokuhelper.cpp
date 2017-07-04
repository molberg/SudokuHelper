// #include <QDebug>
#include <QMessageBox>
#include <QGridLayout>

#include "sudokuhelper.h"
#include "ui_sudokuhelper.h"

SudokuHelper::SudokuHelper(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::SudokuHelper)
{
    ui->setupUi(this);
    QGridLayout *grid = new QGridLayout;

    int i = 0;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            QGridLayout *frameLayout = new QGridLayout;
            frames[i].setFrameStyle(QFrame::Panel | QFrame::Raised);
            grid->addWidget(&frames[i], row, col);
            for (int k = 0; k < 3; k++) {
                for (int l = 0; l < 3; l++) {
                    int ic = (row*3+k)*DIM + (col*3+l);
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
            i++;
        }
    }
    QWidget *window = new QWidget();
    window->setLayout(grid);
    // setLayout(grid);
    grid->setHorizontalSpacing(1);
    grid->setVerticalSpacing(1);
    // grid->setSizeConstraint(QLayout::SetFixedSize);

    // Set QWidget as the central layout of the main window
    setCentralWidget(window);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

SudokuHelper::~SudokuHelper()
{
    delete ui;
}

void SudokuHelper::updateField(int cell, int value)
{
    int row = cell/DIM;
    int col = cell%DIM;
    // qDebug() << __FUNCTION__ << cell << value << row << col;
    // update row
    for (int i = 0; i < DIM; i++) {
        int ic = row*DIM+i;
        if (ic != cell) tiles[ic].setCell(value, false);
    }
    // update column
    for (int i = 0; i < DIM; i++) {
        int ic = i*DIM+col;
        if (ic != cell) tiles[ic].setCell(value, false);
    }
    // update subframe
    for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
            row = 3*(row/3);
            col = 3*(col/3);
            int ic = (row+k)*DIM + col+l;
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
    int row = cell/DIM;
    int col = cell%DIM;
    // qDebug() << __FUNCTION__ << cell << value << row << col;
    // update row
    for (int i = 0; i < DIM; i++) {
        int ic = row*DIM+i;
        tiles[ic].setCell(value, true);
    }
    // update column
    for (int i = 0; i < DIM; i++) {
        int ic = i*DIM+col;
        tiles[ic].setCell(value, true);
    }
    // update subframe
    for (int k = 0; k < 3; k++) {
        for (int l = 0; l < 3; l++) {
            row = 3*(row/3);
            col = 3*(col/3);
            int ic = (row+k)*DIM + col+l;
            tiles[ic].setCell(value, true);
        }
    }
}
