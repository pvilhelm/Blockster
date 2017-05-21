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

    Block* test = new Block(100,120);

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

};

#endif // PROGRAMSCENE_H
