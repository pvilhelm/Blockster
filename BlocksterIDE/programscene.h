#ifndef PROGRAMSCENE_H
#define PROGRAMSCENE_H

#include <QGraphicsScene>
#include <QtWidgets>

#include "block.h"
#include "signalline.h"
#include "signalsegment.h"



class ProgramScene : public QGraphicsScene
{

    Q_OBJECT

public:
    enum Mode { None, InsertLine};
    SignalSegment* lastSignalSegment = 0;

    Mode mode = None;

    ProgramScene(QObject *parent);


    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event);

    void addBlock(QString template_path);
    void addBlock(QString template_path, QPointF scene_pos);


    QHash<QString,int> block_totn_hashtable;
    QList<Block*> block_ptr_list;
};

#endif // PROGRAMSCENE_H
