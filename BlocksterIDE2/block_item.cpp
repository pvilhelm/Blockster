#include "block_item.h"


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
