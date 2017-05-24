#ifndef LIBBROWSER_H
#define LIBBROWSER_H

#include <QDockWidget>
#include <QGraphicsScene>
#include <QGraphicsView>

#include "libscene.h"

namespace Ui {
class LibBrowser;
}

class LibBrowser : public QDockWidget
{
    Q_OBJECT

public:
    explicit LibBrowser(QWidget *parent = 0, QString lib_path = "");
    ~LibBrowser();


    QGraphicsView* root_view;
    LibScene* root_scene;

    QString lib_path;

private:
    Ui::LibBrowser *ui;
};

#endif // LIBBROWSER_H
