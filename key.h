#ifndef KEY_H
#define KEY_H
#include <QGraphicsItem>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QRectF>
#include <QDebug>
class Key : public QGraphicsItem
{
public:
    Key(QGraphicsScene *scene, QChar letter, QRectF rect, int index, QGraphicsItem *parent=nullptr);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    //void createKey();
private:
    QGraphicsScene *scene;
    QChar letter;
    QRectF rect;
    int index;

    bool isPressed=false;
};

#endif // KEY_H
