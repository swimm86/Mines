#include "Field.hpp"

#include "Cell.hpp"

Field::Field() : QObject()
{
}

void Field::setSize(int width, int height)
{
    m_width = width;
    m_height = height;
    m_time = 0;

    for (int y = 0; y < height; ++y) {

        for (int x = 0; x < width; ++x) {
            Cell *cell = new Cell(this, x, y);
            connect(cell, SIGNAL(checkWin()), this, SLOT(checkWin()));
            m_cells.append(cell);
        }
    }
}

void Field::setNumberOfMines(int number)
{
    m_numberOfMines = number;
}

void Field::generate(int x, int y)
{
    qsrand(10);
    int minesToPlace = m_numberOfMines;
    m_minesLeft = m_numberOfMines;
    m_virtualMines = 0;
    m_generated = true;
    m_defusedMines = 0;
    emit startTimer();

    while (minesToPlace > 0) {
        Cell *cell = m_cells.at(qrand() % m_cells.count());

        Cell *banned = cellAt(x,y);
        QVector<Cell*> bannedCells = banned->getNeighbors();
        bannedCells.append(banned);

        if (bannedCells.contains(cell)) {
            continue;
        }

        if (cell->haveMine()) {
            continue;
        }

        cell->setHaveMine(true);
        --minesToPlace;
    }
}

Cell *Field::cellAt(int x, int y) const
{
    if (m_wrapped) {
        if (x < 0) {
            x += m_width;
        } else if (x > m_width - 1) {
            x -= m_width;
        }

        if (y < 0) {
            y += m_height;
        } else if (y > m_height - 1) {
            y -= m_height;
        }
    }

    if (x < 0 || x >= m_width) {
        return 0;
    }

    if (y < 0 || y >= m_height) {
        return 0;
    }

    return m_cells.at(x + y * m_width);
}

void Field::prepare()
{
    m_generated = false;

    for (Cell *cell : m_cells) {
        cell->reset();
    }

    m_minesLeft = m_numberOfMines;
    m_virtualMines = 0;
    m_lose = false;
    m_gameStarted = 0;
}

void Field::plusSec() { ++m_time; }

void Field::plusMine()
{
    if (m_virtualMines != 0) {
        m_virtualMines--;
    } else {
        m_minesLeft++;
    }

    emit changedNumberOfMines();
}

void Field::minusMine()
{
    if (minesLeft() == 0) {
        m_virtualMines++;
    } else {
        m_minesLeft--;
    }

    emit changedNumberOfMines();
}

void Field::timeReset() { m_time = 0; }

void Field::win()
{
    for (Cell *cell : m_cells) {
        if (cell->haveMine() & !cell->isOpen()) {
            cell->setDefused(true);
        }

        cell->setOpened(true);
    }

    emit stopTimer();
    emit winGame();
}

void Field::lose()
{
    m_lose = true;

    for (Cell *cell : m_cells) {
        if (cell->isMined() & cell->haveMine()) {
            cell->setDefused(true);
        }
        cell->minedGameOver();
        cell->setOpened(true);
    }

    emit stopTimer();
}

void Field::checkWin()
{
    m_numberOfUnexplosedMines = 0;

    for (Cell *cell : m_cells) {
        if (!cell->isOpen()) {
            m_numberOfUnexplosedMines++;
        }
    }

    if (m_numberOfUnexplosedMines == m_numberOfMines) {
        win();
    }
}

void Field::setWrapped(bool wrapped)
{
    m_wrapped = wrapped;
}
