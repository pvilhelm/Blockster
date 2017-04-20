#ifndef TERMINALWIDGET_H
#define TERMINALWIDGET_H

#include <QDockWidget>

namespace Ui {
class TerminalWidget;
}

class TerminalWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit TerminalWidget(QWidget *parent = 0);
    ~TerminalWidget();

private:
    Ui::TerminalWidget *ui;
};

#endif // TERMINALWIDGET_H
