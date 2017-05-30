#include "block.h"
#include "signalline.h"
#include <QDebug>

#include <sstream>
//#include <blockstersession.h>
#include "outport.h"
#include "inport.h"



Block::Block(QString template_path)
{
    setPos(0,0);
    setFlags(ItemIsSelectable | ItemIsMovable |  ItemSendsGeometryChanges );

    w = 100.;
    h = 100.;

    ProcessXMLtemplate(template_path);

    name_text_item.setTextInteractionFlags(Qt::TextEditable);
    name_text_item.setParentItem(this);
    this->setName(this->block_name);
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
    Q_UNUSED(option);
    painter->setBrush(QBrush(Qt::white,Qt::SolidPattern));

    painter->drawRect(QRectF(0, 0, w, h));

    return;
}

void Block::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug() << "User clicked block" << "id" << this->block_id << "name" << this->block_name << __LINE__ << __FILE__;
    if(event->button()==Qt::RightButton){
        QMenu* menu = new QMenu();
        menu->addAction("Execute this first");
        this->scene()->addWidget(menu);
    }
    else
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

//Used in ProcessXML template
#define EXISTS_OR_THROW(x, str_what) {if( (x).isNull()){ \
    qWarning() << "XML parsing error element doesn't exist: " << (str_what) << template_path << __LINE__<<__FILE__; \
    throw std::runtime_error("XML parsing error"); \
    }} \

void Block::ProcessXMLtemplate(QString template_path)
{
    QDomDocument xml_root;
    QFile file(template_path);
    if (!file.open(QIODevice::ReadOnly)){
        qWarning() << "Could not open template path:" << template_path << __LINE__ << __FILE__;
        throw std::runtime_error("Could not open template path "+template_path.toStdString());
    }

    this->template_name = template_path.split("/").last();

    if (!xml_root.setContent(&file)) {
        file.close();
        qWarning() <<"Could not set XML DOM for:" << template_path << __LINE__ << __FILE__;
        throw std::runtime_error("Could not set XML DOM for "+template_path.toStdString());
    }
    file.close();

    QDomElement root_el = xml_root.documentElement();

    //Parse id

    this->block_id = root_el.attribute("id");

    //Parse Node_visualisation
    QDomElement vis_el = root_el.firstChildElement("Node_visualisation");
    EXISTS_OR_THROW(vis_el,"Node_visualisation")

    QDomElement shape_el = vis_el.firstChildElement("Node_shape");
    EXISTS_OR_THROW(shape_el,"Node_shap")

    QDomAttr attr_shape = shape_el.attributes().namedItem("shape").toAttr();
    EXISTS_OR_THROW(attr_shape,"Node_visualisation@shape")
    if(attr_shape.value() == "rectangle"){
        this->block_shape_type = RECTANGLE;
    }
    else{
        qWarning() << "XML parsing error element doesn't exist: " << "Node_visualisation" << template_path << __LINE__<<__FILE__;
        throw std::runtime_error("XML parsing error");
    }

    QDomElement h_el = shape_el.firstChildElement("Node_height");
    EXISTS_OR_THROW(h_el,"Node_height")

    {
        QString height = h_el.text();
        bool ok;
        float tmp = height.toFloat(&ok);
        if(ok){
            h = tmp;
        }
        else{
            qWarning() << "XML parsing error height not valid float: " << "Node_height" << template_path << __LINE__<<__FILE__;
            throw std::runtime_error("XML parsing error");
        }
    }

    QDomElement w_el = shape_el.firstChildElement("Node_width");
    EXISTS_OR_THROW(w_el,"Node_width")
    {
        QString width = w_el.text();
        bool ok;
        float tmp = width.toFloat(&ok);
        if(ok){
            w = tmp;
        }
        else{
            qWarning() << "XML parsing error width not valid float: " << "Node_width" << template_path << __LINE__<<__FILE__;
            throw std::runtime_error("XML parsing error");
        }
    }

    //Parse node lib path
    QDomElement node_type_el = root_el.firstChildElement("Node_type");
    EXISTS_OR_THROW(node_type_el,"Node_type")
    QDomElement lib_path_el = node_type_el.firstChildElement("Lib_path");
    EXISTS_OR_THROW(lib_path_el,"Lib_path")

    this->lib_path = lib_path_el.text().simplified();
    if(lib_path.length()<1){
        qWarning() << "XML parsing error Lib_path length < 1: "<< template_path << __LINE__<<__FILE__;
        throw std::runtime_error("XML parsing error");
    }

    //Parse node members
    QDomElement node_init_el = root_el.firstChildElement("Node_init");
    EXISTS_OR_THROW(node_init_el,"Node_init")

    QDomElement members_el = node_init_el.firstChildElement("Members");
    EXISTS_OR_THROW(members_el,"Members")

    //parse all members member:s
    QDomNodeList l = members_el.childNodes();
    for(int i = 0; i<l.length();i++){
        QDomNode n = l.at(i);
        auto e = n.toElement();
        if(e.isNull())
            continue;
        Member m;
        QDomElement member_value_el = e.firstChildElement("Member_value");
        EXISTS_OR_THROW(member_value_el,"Member_value");
        m.member_value = member_value_el.text().simplified();
        QDomElement member_name_el = e.firstChildElement("Member_name");
        EXISTS_OR_THROW(member_name_el,"Member_name");
        m.member_name = member_name_el.text().simplified();
        QDomElement member_type_el = e.firstChildElement("Member_type");
        EXISTS_OR_THROW(member_type_el,"Member_type");
        m.member_type = member_type_el.text().simplified();
        QDomElement member_allowed_types_el = e.firstChildElement("Member_allowed_types");
        EXISTS_OR_THROW(member_allowed_types_el,"Member_allowed_types");
        m.member_allowed_types = member_allowed_types_el.text().simplified().split(" ");
        QDomElement member_tunable_el = e.firstChildElement("Member_tunable");
        EXISTS_OR_THROW(member_tunable_el,"Member_tunable");
        m.member_tunable = member_tunable_el.text().simplified();
        this->member_list.append(m);
    }


    //Parse node name
    QDomElement node_name_el = root_el.firstChildElement("Node_name");
    EXISTS_OR_THROW(node_name_el,"Node_name")

    this->block_name = node_name_el.text().simplified();

    //Parse ports
    QDomElement ports_el = root_el.firstChildElement("Ports");
    EXISTS_OR_THROW(ports_el,"Ports")

    QDomElement in_ports_el = ports_el.firstChildElement("Inports");
    EXISTS_OR_THROW(in_ports_el,"Inports")

    QDomElement out_ports_el = ports_el.firstChildElement("Outports");
    EXISTS_OR_THROW(out_ports_el,"Outports")

    {
        int n_outports = 0;
        QDomNodeList node_list = out_ports_el.childNodes();
        QList<QDomElement> outports_el_list;
        for(int j = 0;j<node_list.length();j++){
            QDomNode i = node_list.item(j);
            if(!i.isElement())
                continue;
            QDomElement e = i.toElement();
            if(e.tagName()!="Outport")
                continue;
            outports_el_list.append(e);
            n_outports++;
        }
        if(n_outports){
            addOutport(n_outports);
        }
        for(int i =0; i< outports_el_list.length();i++){
            QDomElement e = outports_el_list[i];
            Outport* op = this->outportptr_list[0];

            QString o_id = e.attribute("id");

            {
                bool tmp = false;
                o_id.toInt(&tmp);
                if(!tmp){
                    qDebug() << "XML Parsing error: Port id not valid int"<<__LINE__<<__FILE__;
                    throw std::runtime_error("XML Parsing error: Port id not valid int");
                }
            }
            op->port_id = o_id;

            QDomElement o_type_el = e.firstChildElement("Outport_type");
            EXISTS_OR_THROW(o_type_el,"Outport_type")
            op->signal_type = o_type_el.text();

            QDomElement o_validtypes_el = e.firstChildElement("Outport_allowed_types");
            EXISTS_OR_THROW(o_validtypes_el,"Outport_allowed_types")
            op->valid_signal_types = o_validtypes_el.text().split(" ");


        }
    }

    {
        int n_inports = 0;
        QDomNodeList node_list = in_ports_el.childNodes();
        QList<QDomElement> inports_el_list;

        for(int j = 0;j<node_list.length();j++){
            QDomNode i = node_list.item(j);
            if(!i.isElement())
                continue;
            QDomElement e = i.toElement();
            if(e.tagName()!="Inport")
                continue;
            n_inports++;
            inports_el_list.append(e);
        }
        if(n_inports){
            addInport(n_inports);
        }
        for(int i =0; i< inports_el_list.length();i++){
            QDomElement e = inports_el_list[i];
            Inport* op = this->inportptr_list[0];
            QString o_id = e.attribute("id");

            {
                bool tmp = false;
                o_id.toInt(&tmp);
                if(!tmp){
                    qDebug() << "XML Parsing error: Port id not valid int"<<__LINE__<<__FILE__;
                    throw std::runtime_error("XML Parsing error: Port id not valid int");
                }
            }
            op->port_id = o_id;

            QDomElement o_type_el = e.firstChildElement("Inport_type");
            EXISTS_OR_THROW(o_type_el,"Inport_type")
            op->signal_type = o_type_el.text();

            QDomElement o_validtypes_el = e.firstChildElement("Inport_allowed_types");
            EXISTS_OR_THROW(o_validtypes_el,"Inport_allowed_types")
            op->valid_signal_types = o_validtypes_el.text().split(" ");

        }
    }
}

#undef EXISTS_OR_THROW

void Block::addInport(int n)
{
    for(float i = 1; i<=n; i++){
        Inport* newInport = new Inport(0,i*h/(n+1));
        inportptr_list.append(newInport);
        newInport->setParentItem(this);
    }
}

void Block::addOutport(int n)
{
    for(float i = 1; i<=n; i++){
        Outport* newOutport = new Outport(w,i*h/(n+1));
        outportptr_list.append(newOutport);
        newOutport->setParentItem(this);
    }
}

void Block::setName(QString name)
{
    name_text_item.setPlainText(name);
    name_text_item.adjustSize();
    if(name_text_item.textWidth()<40){
        name_text_item.setTextWidth(40);
    }
    name_text_item.setPos(this->w/2-name_text_item.textWidth()/2,h+10);
}

QMimeData* Block::getMime()
{
    QMimeData* mime = new QMimeData();
    mime->setText(this->lib_path);
    return mime;
}

QString Block::getAsXML()
{
    QString out;
    QXmlStreamWriter ss(&out);
    ss.setAutoFormatting(true);
    ss.writeStartDocument();

    ss.writeStartElement("Node");
    ss.writeAttribute("id",this->block_id);
    {
        ss.writeStartElement("Node_name");
        {
            ss.writeCharacters(this->block_name);
        }
        ss.writeEndElement();

        ss.writeStartElement("Node_type");
        ss.writeAttribute("type","cpp");
        {
            ss.writeStartElement("Lib_path");
            {
                ss.writeCharacters(this->lib_path);
            }
            ss.writeEndElement();
        }
        ss.writeEndElement();

        ss.writeStartElement("Node_init");
        {
            ss.writeStartElement("Members");
            {
                for(auto m: this->member_list){
                    ss.writeStartElement("Member");
                    ss.writeTextElement("Member_value",m.member_value);
                    ss.writeTextElement("Member_name",m.member_name);
                    ss.writeTextElement("Member_type",m.member_type);
                    ss.writeTextElement("Member_tunable",m.member_tunable);
                    ss.writeEndElement();
                }
            }
            ss.writeEndElement();
        }
        ss.writeEndElement();

        ss.writeStartElement("Ports");
        {
            ss.writeStartElement("Inports");
            {
                for(auto e : this->inportptr_list){
                    ss.writeStartElement("Inport");
                    ss.writeAttribute("id",e->port_id);
                    ss.writeTextElement("Inport_type",e->signal_type);
                    ss.writeTextElement("Inport_source_node_id",e->source_node_id);
                    ss.writeTextElement("Inport_source_node_Outport_id",e->source_node_outport_id);
                    ss.writeEndElement();
                }
            }
            ss.writeEndElement();

            ss.writeStartElement("Outports");
            {
                for(auto e : this->outportptr_list){
                    ss.writeStartElement("Outport");
                    ss.writeAttribute("id",e->port_id);
                    ss.writeTextElement("Outport_type",e->signal_type);
                    ss.writeEndElement();
                }
            }
            ss.writeEndElement();
        }
        ss.writeEndElement();

        ss.writeStartElement("Node_settings");
        ss.writeEndElement();

        ss.writeStartElement("Node_execution");
        {
            ss.writeTextElement("Node_task",this->task_id);
            ss.writeTextElement("Node_execution_order",this->execution_order);
        }
        ss.writeEndElement();
    }
    ss.writeEndElement();
    ss.writeEndDocument();

    return out;
}


