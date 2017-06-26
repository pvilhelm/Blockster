#ifndef BLOCK_ITEM_H
#define BLOCK_ITEM_H

#include <string>
#include <memory>
#include <QGraphicsItem>

#include "b_node.h"


class BlockItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    BlockItem(QGraphicsItem* parent, std::shared_ptr<bster::b_node> node);

    std::shared_ptr<bster::b_node> this_node;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    void nodeMoved();

};

#endif // BLOCK_ITEM_H
