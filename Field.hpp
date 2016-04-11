#ifndef FIELD_HPP
#define FIELD_HPP

#include <QVector>
#include <QMainWindow>
#include <QObject>

class Cell;

class Field : public QObject
{
    Q_OBJECT

public:
    Field();

    void setSize(int width, int height);
    void setNumberOfMines(int number);

    void prepare();
    void plusSec();
    void reset();
    void timeReset();
    void disableText();
    void restartTime();
    void win();

    void setWrapped(bool wrapped);

    int width() const { return m_width; }
    int height() const { return m_height; }
    int getNumberOfMines () const { return m_numberOfMines;}
    int getNumberOfMinesLeft () const { return m_minesLeft; }
    int time() const { return m_time;}
    int minesLeft() const { return m_minesLeft;}

    bool isGenerated() const { return m_generated;}
    bool isLose() const { return m_lose; }
    bool isGameStarted() const { return m_gameStarted; }

    Cell *cellAt(int x, int y) const;

    QVector<Cell*> getNeighbors(Cell *cell) const;

private:
    QVector<Cell*> m_cells;

    int m_width;
    int m_height;
    int m_numberOfMines;
    int m_time;
    int m_minesLeft;
    int m_lose;
    int m_virtualMines;
    int m_defusedMines;
    int m_numberOfUnexplosedMines;

    bool m_generated;
    bool m_smartMined;
    bool m_gameStarted;
    bool m_wrapped;

signals:
    void changedNumberOfMines();
    void stopTimer();
    void startTimer();
    void winGame();

private slots:
    void checkWin();
    void generate(int x, int y);
    void lose();
    void plusMine();
    void minusMine();
};

#endif // FIELD_HPP
