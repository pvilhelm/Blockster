#ifndef PROGRAM_CONTAINER_H
#define PROGRAM_CONTAINER_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>

#include <memory>

#include "program_scene.h"
#include "b_program_tree.h"
#include "lib_container.h"

class ProgramScene;

class ProgramContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ProgramContainer(QWidget *parent = 0);

    std::shared_ptr<ProgramContainer> ptr_to_self;
    std::weak_ptr<LibContainer> ptr_lib_cont;

    QGraphicsView* root_view;
    ProgramScene* root_scene;

    bster::b_program_tree program_tree;


signals:


public slots:
    void nodeUpdateRequestSlot();

};

#endif // PROGRAM_CONTAINER_H
