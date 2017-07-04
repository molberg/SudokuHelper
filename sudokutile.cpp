#include <QDebug>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>

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
}

void SudokuTile::lock()
{
    locked = true;
    repaint();
    update();
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

void SudokuTile::setCell(int value, bool flag)
{
    // qDebug() << __FUNCTION__ << m_id << m_row << m_col << value << flag;
    if (value > 0 && value < 10) {
        possible[value-1] = flag;
        repaint();
        update();
    }
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
                if (hasFocus()) pen = QPen(Qt::lightGray);
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
    if (solved) {
        qDebug() << __FUNCTION__ << e->x() << e->y() << solved << m_id << value;
        solved = false;
        emit restoreMe(m_id, value);
        value = 0;
    } else {
        int row = e->y()/sub;
        int col = e->x()/sub;
        if (row < 0 || row > 2 || col < 0 || col > 2) return;
        int cell = row*3 + col + 1;
        if (isPossible(cell)) {
            setSolved(cell);
            emit valueChanged(m_id, cell);
        }
    }
}

void SudokuTile::keyPressEvent(QKeyEvent *e)
{
    char key = char(e->key());
    if (e->key() == Qt::Key_Tab) {
        emit moveFocus(m_id);
        return;
    }

    int cell = key - '0';
    if (cell > 0 && cell < 10) {
        if (!solved && isPossible(cell)) {
            setSolved(cell);
            emit valueChanged(m_id, cell);
            emit moveFocus(m_id);
        }
    }
}
