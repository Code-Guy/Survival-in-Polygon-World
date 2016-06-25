#include "losewidget.h"
#include "ui_losewidget.h"

LoseWidget::LoseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoseWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("Lose");
    this->setFixedSize(513, 323);

    helpWidget = new HelpWidget;

    connect(helpWidget, SIGNAL(Closed()), this, SLOT(show()));
}

LoseWidget::~LoseWidget()
{
    delete ui;
}

void LoseWidget::on_restartBtn_clicked()
{
    emit Restart();
    this->hide();
}

void LoseWidget::on_helpBtn_clicked()
{
    this->hide();
    helpWidget->show();
}

void LoseWidget::on_menuBtn_clicked()
{
    emit ReturnToMenu();
    this->hide();
}
