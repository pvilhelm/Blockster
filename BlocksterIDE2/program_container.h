#ifndef PROGRAM_CONTAINER_H
#define PROGRAM_CONTAINER_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVector>

#include <memory>
#include <string>

#include "program_scene.h"
#include "b_program_tree.h"
#include "lib_container.h"

class ProgramScene;

/**
 * \brief Container for a Blockster program.
 *
 * A ProgramContainer contains the b_program_tree program tree, that contains all the program in a tree structure. The
 * ProgramContainer also contains the root QGraphicsView for the QGraphicsScenes that visualizes the program.
 *
 */

class ProgramContainer : public QWidget
{
    Q_OBJECT
public:
    explicit ProgramContainer(QWidget *parent = 0);
    explicit ProgramContainer(std::string path_to_xml_file);

    /**
     * @brief Pointer to the root view object.
     *
     * The ProgramContainer will automatically free the object on destruction.
     */
    QGraphicsView* root_view;

    /**
     * @brief The program tree.
     *
     * The program tree for the Blockster program contained in the ProgramContainer. The program tree is
     * either set to a stored program tree from a xml-file a construction or is set to an empty one, depending
     * on the constructor used.
     */
    bster::b_program_tree program_tree;


signals:


public slots:
    void nodeUpdateRequestSlot();

};

#endif // PROGRAM_CONTAINER_H
