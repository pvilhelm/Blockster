#include "block_item.h"
#include "port_item.h"
#include <QPainter>
#include <QTextOption>
#include <QTextDocument>

BlockItem::BlockItem(QGraphicsItem* parent, std::shared_ptr<bster::b_node> node) : QGraphicsItem(parent), this_node(node)
{
    setFlag(QGraphicsItem::ItemIsMovable);
    //setFlag(QGraphicsItem::ItemIsSelectable);
    //setFlag(QGraphicsItem::ItemIsFocusable);
    setPos(this_node->node_pos.x,this_node->node_pos.y);
    setRotation(this_node->node_pos.rot);

    size_t n_inports = this_node->v_inports.size();
    for(int i = 0; i < n_inports; i++){
        InportItem* ii = new InportItem(this,i);
        ii->setPos(-1-this_node->node_width/2,-this_node->node_height/2+(i+1.)*this_node->node_height/(n_inports+1));
    }
    size_t n_outports = this_node->v_outports.size();
    for(int i = 0; i < n_outports; i++){
        OutportItem* ii = new OutportItem(this,i);
        ii->setPos(1+this_node->node_width/2,-this_node->node_height/2+(i+1.)*this_node->node_height/(n_outports+1));
    }

    BlockTextItem* bti = new BlockTextItem(this);
    bti->setPlainText(this_node->node_name.c_str());
    bti->setTextWidth(-1);
    connect(bti,SIGNAL(nameChangedSignal(std::string)),this,SLOT(nameBoxChanged(std::string)));

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
    return QRectF(-line_padding/2.-this_node->node_width/2,
                  -line_padding/2.-this_node->node_height/2,
                  this_node->node_width+line_padding,
                  this_node->node_height+line_padding);
}

void BlockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawRect(-this_node->node_width/2, -this_node->node_height/2, this_node->node_width,this_node->node_height);
}

void BlockItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("User clicked block");
    QGraphicsItem::mousePressEvent(event);
}

void BlockItem::nameBoxChanged(std::string new_name)
{
    this_node->node_name = new_name;
}

BlockTextItem::BlockTextItem(BlockItem *parent) : QGraphicsTextItem(parent)
{

    this->setTextInteractionFlags(Qt::TextEditable);
    auto doc = this->document();
    connect(doc, SIGNAL( contentsChanged(void)),this,SLOT(nameChanged(void)));
    this->setPos(QPointF(0.-this->boundingRect().width()/2., parent->this_node->node_height/2.+3));
}

void BlockTextItem::nameChanged()
{
    std::string new_name = this->toPlainText().toStdString();
    this->setPos(QPointF(2.-this->boundingRect().width()/2.,
                         ((BlockItem*) this->parentItem())->this_node->node_height/2.+3));
    emit nameChangedSignal(new_name);
}
