#include "signalline.h"



#define WIDTH 1

SignalLine::SignalLine(QPointF start,Outport* op)
{
    addStartSegment(start,start+QPointF(20,0),op);
}

SignalLine::~SignalLine()
{
    for(SignalSegment* s: vec_signalnodes){
        if(s->type & END_SEGMENT){
            Inport* ip = s->end_port;

            if(ip){
                ip->inSignalLine = 0;
            }
        }
        else if(s->type & START_SEGMENT){
            Outport* op = s->start_port;

            if(op){
                op->outSignalLine = 0;
            }
        }
    }
}

QRectF SignalLine::boundingRect() const
{
    return QRectF(0,0,0,0);
}



void SignalLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{

}

void SignalLine::addSegment(QPointF startPoint, QPointF endPoint, SegmentType type)
{
    SignalSegment* ss = new SignalSegment(this,startPoint.x(),startPoint.y(),endPoint.x(),endPoint.y());
    ss->type = type;
    this->vec_signalnodes.append(ss);
}

void SignalLine::addEndSegment(QPointF startPoint, QPointF endPoint, Inport* ip){

    SignalSegment* ss = new SignalSegment(this,startPoint.x(),startPoint.y(),endPoint.x(),endPoint.y());
    ss->type = END_SEGMENT;
    ss->end_port = ip;
    ip->source_node_id = this->getSourcePortBlocksId();
    ip->source_node_outport_id = this->getSourcePortId();
    if(ip->signal_type == "inherit"){
        ip->signal_type = this->getSourcePortType();
    }

    this->vec_signalnodes.append(ss);

    this->end_port_list.append(ip);
    ip->inSignalLine = this;
}

void SignalLine::addStartSegment(QPointF startPoint, QPointF endPoint, Outport *op)
{
    SignalSegment* ss = new SignalSegment(this,startPoint.x(),startPoint.y(),endPoint.x(),endPoint.y());
    ss->type = START_SEGMENT;
    ss->start_port = op;

    this->vec_signalnodes.append(ss);

    this->start_port = op;
    op->outSignalLine = this;
}

QString SignalLine::getSourcePortType()
{
    Outport* op = this->start_port;
    if(!op){
        return "";
    }
    else{
        return op->signal_type;
    }
}

QString SignalLine::getSourcePortBlocksId()
{
    Outport* op = this->start_port;
    if(!op){
        return "";
    }
    else{
        Block* b = dynamic_cast<Block*>(op->parentItem());
        if(b){
            return b->block_id;
        }
    }
}

QString SignalLine::getSourcePortId()
{
    Outport* op = this->start_port;
    if(!op){
        return "";
    }
    else{
        return op->port_id;
    }
}



void SignalLine::removeSegment(SignalSegment *ss)
{
    Q_ASSERT(ss);
    if(!ss)
        return;

    if(ss->type & END_SEGMENT){
        Inport* ip = ss->end_port;
        if(ip){
            ip->inSignalLine = 0;
        }
    }
    else if(ss->type & START_SEGMENT){
        Outport* op = ss->start_port;
        if(op){
            op->outSignalLine = 0;
        }
    }

    this->vec_signalnodes.removeOne(ss);
    delete ss;
}

