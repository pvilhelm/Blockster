#ifndef LIBSCENE_H
#define LIBSCENE_H

#include <QObject>
#include <QGraphicsScene>

class LibScene : public QGraphicsScene
{
    Q_OBJECT

public:
    LibScene(QObject *parent);
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void addBlock(QString path);

    float lastYPos = 0;
};

#endif // LIBSCENE_H
