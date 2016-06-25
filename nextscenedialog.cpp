#include "nextscenedialog.h"
#include "ui_nextscenedialog.h"
#include "globals.h"

NextSceneDialog::NextSceneDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NextSceneDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Next");
    this->setFixedSize(353, 374);

    helpWidget = new HelpWidget;

    connect(helpWidget, SIGNAL(Closed()), this, SLOT(show()));
}

NextSceneDialog::~NextSceneDialog()
{
    delete ui;

    delete helpWidget;
}

void NextSceneDialog::on_nextBtn_clicked()
{
    emit Next();
    this->hide();
}

void NextSceneDialog::on_restartBtn_clicked()
{
    emit Restart();
    this->hide();
}

void NextSceneDialog::on_helpBtn_clicked()
{
    this->hide();
    helpWidget->show();
}

void NextSceneDialog::on_menuBtn_clicked()
{
    emit ReturnToMenu();
    this->hide();
}
