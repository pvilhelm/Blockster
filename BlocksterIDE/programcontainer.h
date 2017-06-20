#ifndef PROGRAMCONTAINER_H
#define PROGRAMCONTAINER_H

#include <QDockWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "programscene.h"

#include "b_program_tree.h"

namespace Ui {
class ProgramContainer;
}

class ProgramContainer : public QDockWidget
{
    Q_OBJECT

public:
    explicit ProgramContainer(QWidget *parent = 0, QString program_name = tr("Defualt program name"));
    ~ProgramContainer();
    QGraphicsView* root_view;
    ProgramScene* root_scene;
    QString prg_name;

    bster::b_program_tree program_tree;



private:
    Ui::ProgramContainer *ui;
};

#endif // PROGRAMCONTAINER_H
