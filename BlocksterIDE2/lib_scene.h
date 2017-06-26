#ifndef LIB_SCENE_H
#define LIB_SCENE_H

#include <QObject>
#include <QGraphicsScene>

#include "b_lib_tree.h"
#include <string>
class LibScene : public QGraphicsScene
{
    Q_OBJECT
public:
    LibScene(std::string lib_path);

    bster::b_lib_tree lib;

public slots:
    void reparseLib();

};

#endif // LIB_SCENE_H
