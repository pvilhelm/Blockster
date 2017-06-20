#ifndef BLOCK_ITEM_H
#define BLOCK_ITEM_H

#include <string>
#include <memory>
#include <QGraphicsItem>

#include "b_node.h"


class block_item : public QGraphicsItem
{
    Q_OBJECT
public:
    block_item(QGraphicsItem* parent, std::shared_ptr<bster::b_node> node);

    std::shared_ptr<bster::b_node> this_node;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    QRectF boundingRect();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

signals:
    void nodeMoved();

};

#endif // BLOCK_ITEM_H
