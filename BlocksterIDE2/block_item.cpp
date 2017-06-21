#include "block_item.h"

#include <QPainter>

block_item::block_item(QGraphicsItem* parent, std::shared_ptr<bster::b_node> node) : QGraphicsObject(parent), this_node(node)
{

}

QVariant block_item::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene()) {
      emit nodeMoved();
    }
    return QGraphicsItem::itemChange(change, value);
}

QRectF block_item::boundingRect()
{
    constexpr double line_padding = 2;//assume a line with 2px thickness for some margin
    return QRectF(-line_padding/2,-line_padding/2,this_node->node_width,this_node->node_height);
}

void block_item::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRect(0,0,this_node->node_width,this_node->node_height);
}
