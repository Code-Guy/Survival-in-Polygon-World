#include "startdialog.h"
#include "ui_startdialog.h"
#include <QBitmap>

StartDialog::StartDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartDialog)
{
    ui->setupUi(this);

    this->parent = parent;
    this->setWindowTitle("Start Set");
    this->setFixedSize(682, 481);

    plane1 = QPixmap(":images/player1.png");
    plane2 = QPixmap(":images/player2.png");
    plane3 = QPixmap(":images/player3.png");
    planeMask = QPixmap(":images/plane_mask.png").scaled(ui->plane1Label->size());

    ui->plane1Label->setPixmap(plane1.scaled(ui->plane1Label->size()));
    ui->plane2Label->setPixmap(plane2.scaled(ui->plane1Label->size()));
    ui->plane3Label->setPixmap(plane3.scaled(ui->plane1Label->size()));
    ui->plane1Label->setMask(planeMask.mask());

    connect(ui->plane1Radio, SIGNAL(clicked(bool)), this, SLOT(planeRadioChange()));
    connect(ui->plane2Radio, SIGNAL(clicked(bool)), this, SLOT(planeRadioChange()));
    connect(ui->plane3Radio, SIGNAL(clicked(bool)), this, SLOT(planeRadioChange()));

    connect(ui->adventureRadio, SIGNAL(clicked(bool)), this, SLOT(modeRadioChange()));
    connect(ui->surviveRadio, SIGNAL(clicked(bool)), this, SLOT(modeRadioChange()));

    ui->adventureRadio->setChecked(true);
    ui->plane1Radio->setChecked(true);

    whichPlane = 1;
    gameMode = 1;

    levelDialog = new LevelDialog(this);
    connect(levelDialog, SIGNAL(UpdateWhichScene(int)), this, SIGNAL(UpdateWhichScene(int)));
    connect(levelDialog, SIGNAL(ShowMenu()), this, SIGNAL(ShowMenu()));
    connect(levelDialog, SIGNAL(StopMenuBGM()), this, SIGNAL(StopMenuBGM()));
}

StartDialog::~StartDialog()
{
    delete ui;
}

void StartDialog::on_okBtn_clicked()
{
    this->hide();
    emit UpdateGameMode(gameMode);
    emit UpdateWhichPlane(whichPlane);

    levelDialog->UpdateWinSceneIndex();
    levelDialog->show();
}

void StartDialog::on_cancelBtn_clicked()
{
    this->hide();
    parent->show();
}

void StartDialog::planeRadioChange()
{
    if(sender() == ui->plane1Radio)
    {
        SetUnchecked(whichPlane);
        ui->plane1Label->setMask(planeMask.mask());
        whichPlane = 1;
    }
    else if(sender() == ui->plane2Radio)
    {
        SetUnchecked(whichPlane);
        ui->plane2Label->setMask(planeMask.mask());
        whichPlane = 2;
    }
    else if(sender() == ui->plane3Radio)
    {
        SetUnchecked(whichPlane);
        ui->plane3Label->setMask(planeMask.mask());
        whichPlane = 3;
    }
}

void StartDialog::modeRadioChange()
{
    if(sender() == ui->adventureRadio)
        gameMode = 1;
    else if(sender() == ui->surviveRadio)
        gameMode = 2;
}

void StartDialog::SetUnchecked(int index)
{
    switch(index)
    {
    case 1:
        ui->plane1Label->clearMask();
        break;
    case 2:
        ui->plane2Label->clearMask();
        break;
    case 3:
        ui->plane3Label->clearMask();
        break;
    }
}
