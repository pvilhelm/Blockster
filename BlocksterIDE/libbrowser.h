#ifndef LIBBROWSER_H
#define LIBBROWSER_H

#include <QDockWidget>

namespace Ui {
class LibBrowser;
}

class LibBrowser : public QDockWidget
{
    Q_OBJECT

public:
    explicit LibBrowser(QWidget *parent = 0, QString lib_path = "");
    ~LibBrowser();

    QString lib_path;

private:
    Ui::LibBrowser *ui;
};

#endif // LIBBROWSER_H
