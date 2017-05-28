#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "libbrowser.h"
#include "blockstersession.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    LibBrowser* libBrowser;



private slots:
    void on_actionNew_program_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
