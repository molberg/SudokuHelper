#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>

#include <stdio.h>

#include "sudokutile.h"

const int SudokuTile::sub = 18;

SudokuTile::SudokuTile(QWidget *parent) :
    QWidget(parent), possible(DIM), solved(false), locked(false), value(0)
{
    for (int i = 0; i < DIM; i++) possible[i] = true;
    setMinimumSize(3*sub+2, 3*sub+2);
}

void SudokuTile::setSolved(int solution)
{
    solved = true;
    value = solution;
    repaint();
    update();
    emit valueChanged(m_id, solution);
}

void SudokuTile::restore()
{
    // qInfo() << __FUNCTION__ << m_id << value;
    solved = false;
    possible[value-1] = true;
    value = 0;
    repaint();
    update();
    emit restoreMe(m_id, value);
    // dump();
}

void SudokuTile::lock()
{
    locked = true;
    repaint();
    update();
}

void SudokuTile::unlock()
{
    locked = false;
}

void SudokuTile::reset()
{
    if (!locked) {
        solved = false;
        locked = false;
        value = 0;
        for (int i = 0; i < DIM; i++) possible[i] = true;
    }
}

int SudokuTile::unique()
{
    if (solved) return 0;

    int count = 0, which = 0;
    for (int i = 0; i < DIM; i++) {
        if (possible[i]) {
            count++;
            which = i+1;
        }
    }
    if (count != 1) return 0;
    return which;
}

void SudokuTile::setCell(int value, bool flag)
{
    // qDebug() << __FUNCTION__ << m_id << m_row << m_col << value << flag;
    if (value > 0 && value < 10) {
        possible[value-1] = flag;
        repaint();
        update();
    }
    bool allowed = false;
    for (int i = 0; i < DIM; i++) {
        allowed |= possible[i];
        if (allowed) return;
    }
    emit unsolvable();
}

bool SudokuTile::isPossible(int value) const
{
    if (value < 1 || value > DIM) return false;
    return possible[value-1];
}

void SudokuTile::paintEvent(QPaintEvent *)
{
    QRectF size = QRectF(0, 0, 3*sub, 3*sub);
    QPen pen;

    QPainter painter(this);

    if (solved) {
        if (locked) pen = QPen(Qt::red);
        else        pen = QPen(Qt::black);
        painter.setPen(pen);
        QFont font;
        font.setPixelSize(32);
        painter.setBrush(Qt::white);
        painter.drawRect(size);
        painter.setFont(font);
        painter.drawText(size, Qt::AlignCenter, QString::number(value));
    } else {
        painter.setBrush(Qt::lightGray);
        int number = 1;
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                QRectF cell = QRectF(col*sub, row*sub, sub, sub);
                if (hasFocus()) pen = QPen(Qt::black);
                else            pen = QPen(Qt::darkGray);
                painter.setPen(pen);
                painter.drawRect(cell);
                if (possible[row*3+col]) painter.drawText(cell, Qt::AlignCenter, QString::number(number));
                else                     painter.drawText(cell, Qt::AlignCenter, QString());
                number++;
            }
        }
    }
}

void SudokuTile::mousePressEvent(QMouseEvent *e)
{
    // qInfo() << __FUNCTION__ << e->x() << e->y() << solved << m_id << value << e->button();
    int row = e->y()/sub;
    int col = e->x()/sub;
    if (row < 0 || row > 2 || col < 0 || col > 2) return;
    int subcell = row*3 + col + 1;
    if (e->button() == Qt::LeftButton) {
        if (solved) {
            restore();
        } else {
            if (isPossible(subcell)) {
                setSolved(subcell);
            }
        }
    } else {
        setCell(subcell, false);
    }
}

void SudokuTile::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Tab:
    case Qt::Key_Right:
        emit moveFocus(m_id, 1);
        break;
    case Qt::Key_Backtab:
    case Qt::Key_Left:
        emit moveFocus(m_id, -1);
        break;
    case Qt::Key_Up:
        emit moveFocus(m_id, -DIM);
        break;
    case Qt::Key_Down:
        emit moveFocus(m_id, DIM);
        break;
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        int cell = e->key() - Qt::Key_0;
        if (!solved && isPossible(cell)) {
            setSolved(cell);
            emit moveFocus(m_id, 1);
        }
        break;
    }
}

void SudokuTile::dump()
{
    printf("tile %d [%d %d] %d:", m_id, solved, locked, value);
    for (int i = 0; i < DIM; i++) printf(" %d", possible[i]);
    printf("\n");
}
