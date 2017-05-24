#include "libbrowser.h"
#include "ui_libbrowser.h"
#include <QDrag>
#include "block.h"

LibBrowser::LibBrowser(QWidget *parent, QString lib_path) :
    QDockWidget(parent),
    ui(new Ui::LibBrowser),
    lib_path(lib_path)
{
    ui->setupUi(this);
    setWindowTitle("Library browser");
    root_scene = new LibScene(this);
    root_view = new QGraphicsView(root_scene,this);

    this->ui->horizontalLayout->addWidget(root_view);
    root_scene->addBlock("");

    root_view->setObjectName(QStringLiteral("rootView"));

}

LibBrowser::~LibBrowser()
{
    delete ui;
}
