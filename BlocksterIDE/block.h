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
    enum BlockShapeType{
        RECTANGLE
    };
    class Member{
    public:
        QString member_value = "";
        QString member_type = "";
        QString member_name = "";
        QStringList member_allowed_types;
        QString member_tunable = "no";
    };

    Block(QString template_path);

    QString block_name = "";
    QString block_id = "";
    QString block_type = "";
    QString lib_path = "";
    QString task_id = "";
    QString execution_order = "";
    QString template_name = "";

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void ProcessXMLtemplate(QString template_path);
    void addInport(int n);
    void addOutport(int n);
    void setLibPath(QString lib_path);
    void parseXML();
    void setName(QString name);
    QMimeData* getMime();
    QString getAsXML();

    QColor color = Qt::white;
    float w = 0;
    float h = 0;

    BlockShapeType block_shape_type = RECTANGLE;
    QList<Inport*> inportptr_list;
    QList<Outport*> outportptr_list;
    QList<Member> member_list;

    QGraphicsTextItem name_text_item;
};

#endif // BLOCK_H
