#ifndef PROGRAM_SCENE_H
#define PROGRAM_SCENE_H

#include <QGraphicsScene>
#include <QtWidgets>

#include <memory>
#include <vector>
#include <string>

#include "program_container.h"
#include "b_node.h"
#include "block_item.h"

class ProgramContainer;

/**
 * @brief The ProgramScene class is the graphical representation of the /ref b_program_tree
 *
 * It also processes graphical user input.
 */

class ProgramScene : public QGraphicsScene
{
    Q_OBJECT
public:
    ProgramScene(QObject *parent);

    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event);

signals:
    void nodeUpdateRequest();
    void nodeAddRequest(std::string str_xml);

};


#endif // PROGRAM_SCENE_H
