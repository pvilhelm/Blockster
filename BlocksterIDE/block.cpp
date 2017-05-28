#include "block.h"
#include "signalline.h"

Block::Block(float w, float h): Block::Block(0,0,w,h)
{
}

Block::Block(float x, float y, float w, float h) : w(w), h(h)
{
    setPos(x,y);
    setFlags(ItemIsSelectable | ItemIsMovable |  ItemSendsGeometryChanges );
    nameTextItem.setTextInteractionFlags(Qt::TextEditable);
    nameTextItem.setParentItem(this);
    this->setName("");
}

Block::Block(QString template_path)
{
    setPos(0,0);
    setFlags(ItemIsSelectable | ItemIsMovable |  ItemSendsGeometryChanges );
    nameTextItem.setTextInteractionFlags(Qt::TextEditable);
    nameTextItem.setParentItem(this);
    this->setName("");
    w = 100.;
    h = 100.;

    ProcessXMLtemplate(template_path);
}

QRectF Block::boundingRect() const
{
    return QRectF(0, 0, w, h);
}

QPainterPath Block::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, w, h);
    return path;
}

void Block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->drawRect(QRectF(0, 0, w, h));

    return;
}

void Block::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

QVariant Block::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionHasChanged){
        //QPointF newPos = value.toPointF();
        for( QGraphicsItem* i : this->childItems()){
            Outport* o = dynamic_cast<Outport*>(i);
            if(o){
                SignalLine* sl = o->outSignalLine;
                if(sl){
                    SignalSegment* ss = sl->vec_signalnodes.first();
                    ss->changePos(o->scenePos());
                }
                continue;
            }
            Inport* in = dynamic_cast<Inport*>(i);
            if(in){
                SignalLine* sl = in->inSignalLine;
                if(sl){
                    SignalSegment* ss = sl->vec_signalnodes.last();
                    ss->changePos(ss->scenePos(),in->scenePos());
                }
                continue;
            }
        }
    }
    return value;
}

void Block::ProcessXMLtemplate(QString template_path)
{
    xml_root = new QDomDocument();
    QFile file(template_path);
    if (!file.open(QIODevice::ReadOnly)){
        throw std::runtime_error("Could not open template path "+template_path.toStdString());
    }

    if (!xml_root->setContent(&file)) {
        file.close();
        throw std::runtime_error("Could not set XML DOM for "+template_path.toStdString());
    }
    file.close();

    QDomElement root_el = xml_root->documentElement();

    QDomNodeList  node_vis_list = root_el.elementsByTagName("Node_visualisation");
    if(!node_vis_list.isEmpty() || node_vis_list.count()!=1){
        throw std::runtime_error("XML parsing error "+template_path.toStdString());
    }
    QDomNode vis_el = node_vis_list.item(0);
    QDomNode shape_el = vis_el.elementsByTagName("Node_shape").item(0);

    if(shape_el.isEmpty() || node_vis_list.count()!=1){
        QString error = "XML parsing error "+template_path+__LINE__+__FILE__;
        throw std::runtime_error(error.toStdString());
    }

}

void Block::addInport(int n)
{
    for(float i = 1; i<=n; i++){
        Inport* newInport = new Inport(0,i*h/(n+1));
        newInport->setParentItem(this);
    }
}

void Block::addOutport(int n)
{
    for(float i = 1; i<=n; i++){
        Outport* newOutport = new Outport(w,i*h/(n+1));
        newOutport->setParentItem(this);
    }
}

void Block::setName(QString name)
{
    nameTextItem.setPlainText(name);
    nameTextItem.adjustSize();
    if(nameTextItem.textWidth()<40){
        nameTextItem.setTextWidth(40);
    }
    nameTextItem.setPos(this->w/2-nameTextItem.textWidth()/2,h+10);
}

QMimeData *Block::getMime()
{
    QMimeData* mime = new QMimeData();
    mime->setText(this->lib_path);
    return mime;
}


