#include "setdialog.h"
#include "ui_setdialog.h"
#include <QPainter>

SetDialog::SetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Set");
    this->setFixedSize(450, 320);

    this->parent = parent;

    backgroundLace = QPixmap(":/images/set_lace.png");
    connect(ui->simpleRadioBtn, SIGNAL(clicked(bool)), this, SLOT(radioChange()));
    connect(ui->commonRadioBtn, SIGNAL(clicked(bool)), this, SLOT(radioChange()));
    connect(ui->hardRadioBtn, SIGNAL(clicked(bool)), this, SLOT(radioChange()));
    connect(ui->superhardRadioBtn, SIGNAL(clicked(bool)), this, SLOT(radioChange()));

    ui->commonRadioBtn->setChecked(true);
    difficulty = 2;
}

SetDialog::~SetDialog()
{
    delete ui;
}

void SetDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, backgroundLace);
}

void SetDialog::on_okBtn_clicked()
{
    this->hide();
    emit UpdateDifficulty(difficulty);
}

void SetDialog::on_cancelBtn_clicked()
{
    this->hide();
}

void SetDialog::radioChange()
{
    if(sender() == ui->simpleRadioBtn)
        difficulty = 1;
    else if(sender() == ui->commonRadioBtn)
        difficulty = 2;
    else if(sender() == ui->hardRadioBtn)
        difficulty = 3;
    else if(sender() == ui->superhardRadioBtn)
        difficulty = 4;
}
