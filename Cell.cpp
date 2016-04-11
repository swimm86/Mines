#include "Cell.hpp"

Cell::Cell(int x, int y) : QObject()
{
    m_x = x;
    m_y = y;

    m_haveMine = false;
    m_open = false;
    m_suspect = false;
    m_mined = false;
    m_defused = false;

    QVector<Cell*> m_neighbors;
}

int Cell::minesAround() const
{
    int mines = 0;

    for (Cell *cell : m_neighbors) {
        if (cell->haveMine()) {
            ++mines;
        }
    }

    return mines;
}

void Cell::setHaveMine(bool haveMine)
{
    m_haveMine = haveMine;
}

void Cell::open()
{
    emit generate(m_x, m_y);

    if (isOpen()) {
        return;
    } else if (!isSuspect() & !isMined()) {

        if(minesAround() != 0) {
            m_open = true;
        } else {
            m_open = true;

            for (Cell *cell : m_neighbors) {
                if (!cell->isMined() & !cell->isSuspect() &cell->minesAround() == 0) {
                    cell->open();
                } else if (!cell->isMined() & !cell->isSuspect()) {
                    cell->m_open = true;
                }
            }
        }
        emit checkWin();
    }

    if(this->haveMine() & !isSuspect() & !isMined()) {
        emit lose();
    }
}

void Cell::smartOpen()
{
    int minedCells = 0;
    for (Cell *cell : m_neighbors) {
        if (cell->isMined()) {
            minedCells++;
        }
    }
    if (minedCells == minesAround()) {
        for (Cell *cell : m_neighbors) {
            if (!cell->isMined()) {
                cell->setOpened(true);
                emit checkWin();
            }

            if (cell->isOpen() & cell->haveMine()) {
                emit lose();
            }
        }
    }
}

QVector<Cell *> Cell::getNeighbors() const
{
    return m_neighbors;
}

void Cell::reset()
{
    m_haveMine = false;
    m_open = false;
    m_mined = false;
    m_suspect = false;
    m_defused = false;
}

void Cell::setSuspect(bool suspected)
{
    m_suspect = suspected;
}

void Cell::setMined(bool mined)
{
    m_mined = mined;

    if (m_mined) {
        emit minusMine();
    } else {
        emit plusMine();
    }
}

void Cell::setDefused(bool defused)
{
    m_defused = defused;
}

void Cell::setOpened(bool opened)
{
    m_open = opened;
}

void Cell::setNeighbors(const QVector<Cell *> &neighbors)
{
    m_neighbors = neighbors;
}

void Cell::minedGameOver()
{
    m_mined = false;
    m_suspect = false;
}
