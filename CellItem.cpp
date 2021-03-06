#include "CellItem.hpp"

#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSimpleTextItem>
#include <QPainter>

#include "Cell.hpp"

const int cellSize = 32;

CellItem::CellItem(Cell *cell) :
    QGraphicsItem()
{
    m_cell = cell;
    m_text = new QGraphicsSimpleTextItem(this);

    QFont f = m_text->font();
    f.setPixelSize(cellSize - 4);
    m_text->setFont(f);

    setPos(m_cell->x() * cellSize, m_cell->y() * cellSize);

    setAcceptedMouseButtons(Qt::RightButton | Qt::LeftButton);
}

QRectF CellItem::boundingRect() const
{
    return QRectF(0, 0, cellSize, cellSize);
}

void CellItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setRenderHint(QPainter::HighQualityAntialiasing);

    painter->drawRect(0, 0, cellSize, cellSize);

    static const int border = 4;
    if (m_cell->isOpen()) {

        m_text->setText("");

        if (m_cell->haveMine()) {
            if (!m_cell->isDefused()) { painter->fillRect(0, 0, cellSize, cellSize, Qt::red); }
            else { painter->fillRect(0, 0, cellSize, cellSize, Qt::green); }
            m_text->setText("");
            m_text->setText("+");
        } else if (!m_cell->haveMine() & m_cell->minesAround() != 0) {
            m_text->setText(QString::number(m_cell->minesAround()));
        }
    } else {
        painter->fillRect(border, border, cellSize - border * 2, cellSize - border * 2, Qt::lightGray);
        m_text->setText("");

        if (m_cell->isSuspect()) {
            m_text->setText("?");
        } else if (m_cell->isMined()) {
            m_text->setText("!");
        }
    }

    m_text->setPos((cellSize - m_text->boundingRect().width()) / 2, (cellSize - m_text->boundingRect().height()) / 2);
}

void CellItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

void CellItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_cell->open();
    } else if (event->button() == Qt::RightButton) {

        if (m_cell->isOpen()) {
            m_cell->smartOpen();
        } else if (!m_cell->isMined() & !m_cell->isSuspect()) {
            m_cell->setMined(true);
        } else if (m_cell->isMined()) {
            m_cell->setSuspect(true);
            m_cell->setMined(false);
        } else {
            m_cell->setSuspect(false);
        }
    }

    update();
}

