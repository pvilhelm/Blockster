#include "terminalwidget.h"
#include "ui_terminalwidget.h"

TerminalWidget::TerminalWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::TerminalWidget)
{
    ui->setupUi(this);
}

TerminalWidget::~TerminalWidget()
{
    delete ui;
}
