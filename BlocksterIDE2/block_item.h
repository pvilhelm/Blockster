#ifndef BLOCK_ITEM_H
#define BLOCK_ITEM_H

#include <string>
#include <memory>

#include <QGraphicsItem>
#include <QObject>


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

public slots:
    void nameBoxChanged(std::string new_name);

};

class BlockTextItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    BlockTextItem(BlockItem* parent);

signals:
    void nameChangedSignal(std::string new_name);

public slots:
    void nameChanged();

};


#endif // BLOCK_ITEM_H
