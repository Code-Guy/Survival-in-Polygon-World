#include "winwidget.h"
#include "ui_winwidget.h"

WinWidget::WinWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("Win");
    this->setFixedSize(543, 321);
    helpWidget = new HelpWidget;

    connect(helpWidget, SIGNAL(Closed()), this, SLOT(show()));
}

WinWidget::~WinWidget()
{
    delete ui;

    delete helpWidget;
}

void WinWidget::on_restartBtn_clicked()
{
    emit Restart();
    this->hide();
}

void WinWidget::on_helpBtn_clicked()
{
    this->hide();
    helpWidget->show();
}

void WinWidget::on_menuBtn_clicked()
{
    emit ReturnToMenu();
    this->hide();
}
