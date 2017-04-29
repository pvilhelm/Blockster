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

