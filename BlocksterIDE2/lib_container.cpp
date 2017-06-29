#include "lib_container.h"

#include <memory>
#include <QHBoxLayout>

LibContainer::LibContainer(QWidget *parent) : QWidget(parent)
{
    this->resize(300, 600);

    QHBoxLayout* ptr_layout = new QHBoxLayout(this);

    root_scene = new LibScene(R"(C:\Repos\Blockster\Blockster\blocks)");
    root_scene->setObjectName("lib_scene");
    root_view = new QGraphicsView(root_scene);
    root_view->show();
    ptr_layout->addWidget(root_view);

    //connects signals
    //connect(root_scene,SIGNAL(nodeUpdateRequest()),this,SLOT(nodeUpdateRequestSlot()));

}

/*void LibContainer::nodeUpdateRequestSlot()
{
    qDebug("Print: nodeUpdateRequestSlot(); SLOT WORKED =)");
}*/
