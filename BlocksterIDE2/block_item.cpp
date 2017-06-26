#include "block_item.h"
#include "port_item.h"
#include <QPainter>

BlockItem::BlockItem(QGraphicsItem* parent, std::shared_ptr<bster::b_node> node) : QGraphicsItem(parent), this_node(node)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    //setFlag(QGraphicsItem::ItemIsSelectable);
    //setFlag(QGraphicsItem::ItemIsFocusable);
    setPos(this_node->node_pos.x,this_node->node_pos.y);
    setRotation(this_node->node_pos.rot);

    int n_inports = this_node->v_inports.size();
    for(int i = 0; i < n_inports; i++){
        InportItem* ii = new InportItem(this,i);
        ii->setPos(0,(i+1.)*this_node->node_height/(n_inports+1));
    }
    int n_outports = this_node->v_outports.size();
    for(int i = 0; i < n_outports; i++){
        OutportItem* ii = new OutportItem(this,i);
        ii->setPos(this_node->node_width,(i+1.)*this_node->node_height/(n_outports+1));
    }
}

QVariant BlockItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene()) {
      emit nodeMoved();
    }
    return QGraphicsItem::itemChange(change, value);
}

QRectF BlockItem::boundingRect() const
{
    constexpr double line_padding = 2.;//assume a line with 2px thickness for some margin
    return QRectF(-line_padding/2.,
                  -line_padding/2.,
                  this_node->node_width+line_padding,
                  this_node->node_height+line_padding);
}

void BlockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRect(0.,0.,this_node->node_width,this_node->node_height);
}

void BlockItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("User clicked block");
    QGraphicsItem::mousePressEvent(event);
}
