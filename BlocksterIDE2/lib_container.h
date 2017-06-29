#ifndef LIB_CONTAINER_H
#define LIB_CONTAINER_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>

#include <memory>

#include "lib_scene.h"
#include "b_program_tree.h"

class LibScene;

class LibContainer : public QWidget
{
    Q_OBJECT
public:
    explicit LibContainer(QWidget *parent = 0);

    std::shared_ptr<LibContainer> ptr_to_self;
    QGraphicsView* root_view;
    LibScene* root_scene;


signals:


public slots:

};

#endif // LIB_CONTAINER_H
