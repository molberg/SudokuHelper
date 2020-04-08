#ifndef SUDOKUTILE_H
#define SUDOKUTILE_H

#include <QWidget>
#include <QVector>

#define DIM 9
#define SUB 3

class SudokuTile : public QWidget
{
    Q_OBJECT

public:
    static const int sub;
    explicit SudokuTile(QWidget *parent = 0);

    void setId(int n) { m_id = n; m_row = n/DIM; m_col = n%DIM; };
    void setSolved(int solution);
    void lock();
    void reset();
    bool isSolved() const { return solved; };
    bool isLocked() const { return locked; };
    int getValue()  const { return value;  };
    int unique();
    void restore();
    void setCell(int value, bool possible);
    bool isPossible(int value) const;
    QSize sizeHint() const { return QSize(SUB*sub, SUB*sub); };
    void dump();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);

signals:
    void valueChanged(int id, int value);
    void restoreMe(int id, int value);
    void moveFocus(int id, int steps);
    void unsolvable();

private:
    int m_id;
    int m_row, m_col;
    QVector<bool> possible;
    bool solved;
    bool locked;
    int value;
};

#endif // SUDOKUTILE_H
