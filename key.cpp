#include "key.h"

Key::Key(QGraphicsScene *scene, QChar letter, QRectF rect, int index, QGraphicsItem *parent) : QGraphicsItem(parent)
{
    this->scene=scene;
    this->letter=letter;
    this->rect=rect;
    this->index=index;
}

void Key::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);
    painter->setPen(Qt::black);
    int x=index;
    switch (x) {
    case 0:
    case 4:
        painter->setBrush(QColor(255, 209, 220));
        break;
    case 1:
    case 5:
        painter->setBrush(QColor(255, 218, 185));
        break;
    case 2:
    case 7:
        painter->setBrush(QColor(185, 231, 255));
        break;
    default:
        painter->setBrush(QColor(220, 208, 255));
        break;

    }

    if(isPressed) painter->setBrush(QColor(205, 87, 51));
    painter->drawRect(boundingRect());
    painter->setPen(Qt::black);
    painter->drawText(boundingRect(), Qt::AlignCenter, QString(letter));
}

QRectF Key::boundingRect() const
{
    return this->rect;
}
