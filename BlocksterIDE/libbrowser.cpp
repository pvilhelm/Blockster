#include "libbrowser.h"
#include "ui_libbrowser.h"

LibBrowser::LibBrowser(QWidget *parent, QString lib_path) :
    QDockWidget(parent),
    ui(new Ui::LibBrowser),
    lib_path(lib_path)
{
    ui->setupUi(this);

}

LibBrowser::~LibBrowser()
{
    delete ui;
}
