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
    Block(float w, float h);
    Block(float x, float y, float w, float h);

    QString block_name;
    QString block_id;
    QString block_type;
    QString lib_path ="";

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void addInport(int n);
    void addOutport(int n);
    void setLibPath(QString lib_path);
    void parseXML();
    void setName(QString name);

    QColor color = Qt::white;
    float w = 0;
    float h = 0;
    QString name;

    QDomDocument* xml_root;

    QGraphicsTextItem nameTextItem;


};

#endif // BLOCK_H
