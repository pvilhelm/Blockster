#ifndef LIBBROWSER_H
#define LIBBROWSER_H

#include <QDockWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QList>

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

    void ParseBlocks();

    QGraphicsView* root_view;
    LibScene* root_scene;
    QString lib_path;
    QList<QString> libPathsList;

private:
    Ui::LibBrowser *ui;
};

#endif // LIBBROWSER_H
