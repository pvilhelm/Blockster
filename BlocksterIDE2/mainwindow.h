#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
private:

};

#endif // MAINWINDOW_H
