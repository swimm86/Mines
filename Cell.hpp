#ifndef CELL_HPP
#define CELL_HPP

#include <QVector>
#include <QMainWindow>
#include <QObject>

class Cell : public QObject
{
    Q_OBJECT

public:
    Cell(int x = 0, int y = 0);

public:
    int x() const { return m_x; }
    int y() const { return m_y; }

    int minesAround() const;

    void setHaveMine(bool haveMine);
    void setSuspect(bool suspected);
    void setMined(bool mined);
    void setDefused(bool defused);
    void setOpened(bool opened);
    void setNeighbors(const QVector<Cell*> &neighbors);

    bool isOpen() const { return m_open; }
    bool isSuspect() const { return m_suspect; }
    bool isMined() const { return m_mined; }
    bool isDefused() const { return m_defused; }
    bool haveMine() const { return m_haveMine; }

    void open();
    void smartOpen();
    void reset();
    void minedGameOver();


    QVector<Cell*> getNeighbors() const;

private:
    int m_x;
    int m_y;

    bool m_haveMine;
    bool m_open;
    bool m_suspect;
    bool m_mined;
    bool m_defused;

    QVector<Cell*> m_neighbors;

signals:
    void checkWin();
    void generate(int x, int y);
    void lose();
    void minusMine();
    void plusMine();

};

#endif // CELL_HPP
