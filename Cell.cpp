#include "Cell.hpp"

#include "Field.hpp"

Cell::Cell(Field *field, int x, int y)
{
    m_field = field;

    m_x = x;
    m_y = y;

    m_haveMine = false;
    m_open = false;
    m_suspect = false;
    m_mined = false;
    m_defused = false;
}

int Cell::minesAround() const
{
    int mines = 0;

    for (Cell *cell : getNeighbors()) {
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
    if(!m_field->isGenerated()) {
        m_field->generate(m_x, m_y);
    }

    if (isOpen()) { return; }

    else if (!isSuspect() & !isMined()) {

        if(minesAround() != 0) {
            m_open = true;
        } else {
            m_open = true;

            for (Cell *cell : getNeighbors()) {
                if (!cell->isMined() & !cell->isSuspect() &cell->minesAround() == 0) {
                    cell->open();
                } else if (!cell->isMined() & !cell->isSuspect()) {
                    cell->m_open = true;
                }
            }
        }
        m_field->checkWin();
    }

    if(this->haveMine() & !m_field->isLose() & !isSuspect() & !isMined()) {
        m_field->lose();
    }
}

void Cell::smartOpen()
{
    for (Cell *cell : getNeighbors()) {
        if (!cell->isMined()) {
            cell->setOpened(true);
            m_field->checkWin();
        }

        if (cell->isOpen() & cell->haveMine()) {
            m_field->lose();
        }
    }
}

void maybeAddCell(QVector<Cell*> *vector, Cell *cell)
{
    if (cell) {
        vector->append(cell);
    }
}

QVector<Cell *> Cell::getNeighbors() const
{
    QVector<Cell*> neighbors;

    for (int x = m_x - 1; x <= m_x + 1; ++x) {
        maybeAddCell(&neighbors, m_field->cellAt(x, m_y + 1));
        maybeAddCell(&neighbors, m_field->cellAt(x, m_y - 1));
    }

    maybeAddCell(&neighbors, m_field->cellAt(m_x + 1, m_y));
    maybeAddCell(&neighbors, m_field->cellAt(m_x - 1, m_y));

    return neighbors;
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
        m_field->minusMine();
    } else {
        m_field->plusMine();
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

void Cell::minedGameOver()
{
    m_mined = false;
    m_suspect = false;
}
