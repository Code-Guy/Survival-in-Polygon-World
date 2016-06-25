#include "pausewidget.h"
#include "ui_pausewidget.h"

PauseWidget::PauseWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PauseWidget)
{
    ui->setupUi(this);
    this->setWindowTitle("Pause");
    this->setFixedSize(569, 414);
    helpWidget = new HelpWidget;

    connect(helpWidget, SIGNAL(Closed()), this, SLOT(show()));
}

PauseWidget::~PauseWidget()
{
    delete ui;

    delete helpWidget;
}

void PauseWidget::on_resumeBtn_clicked()
{
    this->hide();
    emit Resume();
}

void PauseWidget::on_helpBtn_clicked()
{
    this->hide();
    helpWidget->show();
}

void PauseWidget::on_menuBtn_clicked()
{
    this->hide();
    emit ReturnToMenu();
}

void PauseWidget::on_restartBtn_clicked()
{
    this->hide();
    emit Restart();
}
