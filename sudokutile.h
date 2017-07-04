#ifndef SUDOKUTILE_H
#define SUDOKUTILE_H

#include <QWidget>
#include <QVector>

#define DIM 9

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
    void setCell(int value, bool possible);
    bool isPossible(int value) const;
    QSize sizeHint() const { return QSize(3*sub, 3*sub); };

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);

signals:
    void valueChanged(int id, int value);
    void restoreMe(int id, int value);

private:
    int m_id;
    int m_row, m_col;
    QVector<bool> possible;
    bool solved;
    bool locked;
    int value;
};

#endif // SUDOKUTILE_H