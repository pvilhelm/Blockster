#ifndef PROGRAMSCENE_H
#define PROGRAMSCENE_H

#include <QGraphicsScene>
#include <QtWidgets>

#include "block.h"

class ProgramScene : public QGraphicsScene
{

    Q_OBJECT

public:
    enum Mode { None, InsertLine};

    Mode mode = None;

    ProgramScene(QObject *parent);

    Block* test = new Block(100,120);

};

#endif // PROGRAMSCENE_H
