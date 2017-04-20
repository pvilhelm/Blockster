#ifndef BLOCK_H
#define BLOCK_H

#include <QGraphicsItem>
#include <QtXml>
#include <QtWidgets>
#include <QDomDocument>

#include "inport.h"
#include "outport.h"

class Block : public QGraphicsItem
{
public:
    Block(int w, int h);

    QString block_name;
    QString block_id;
    QString block_type;
    QString lib_path ="";

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

    void addInport(int n);
    void addOutport(int n);
    void setLibPath(QString lib_path);
    void parseXML();

    QColor color = Qt::white;
    int w = 0;
    int h = 0;

    QDomDocument* xml_root;



};

#endif // BLOCK_H
