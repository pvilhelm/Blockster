#include "signalline.h"



#define WIDTH 1

SignalLine::SignalLine(QPointF start)
{
    float x = start.x();
    float y = start.y();
    vec_signalnodes.append(new SignalSegment(this,x,y,x+20,y));


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

void SignalLine::removeSegment(SignalSegment *ss)
{
    if(!ss)
        return;

    if(ss == this->vec_signalnodes.last()){
        this->vec_signalnodes.removeLast();
        delete ss;
    }
    else
        return;

}

