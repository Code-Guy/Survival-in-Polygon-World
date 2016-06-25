#include "leveldialog.h"
#include "ui_leveldialog.h"

#include <QDebug>
#include "globals.h"

LevelDialog::LevelDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LevelDialog)
{
    ui->setupUi(this);
    this->parent = parent;
    this->setWindowTitle("Select Scene");
    this->setFixedSize(375, 435);

    ui->scene1Btn->setDisabled(true);
    ui->scene2Btn->setDisabled(true);
    ui->scene3Btn->setDisabled(true);
    ui->scene4Btn->setDisabled(true);
    ui->scene5Btn->setDisabled(true);

    whichScene = 1;
}

LevelDialog::~LevelDialog()
{
    delete ui;
}

void LevelDialog::on_scene1Btn_clicked()
{
    whichScene = 1;
    if(whichScene != prevWhichScene)
        SetUnchecked(prevWhichScene);
    prevWhichScene = whichScene;
}

void LevelDialog::on_scene2Btn_clicked()
{
    whichScene = 2;
    if(whichScene != prevWhichScene)
        SetUnchecked(prevWhichScene);
    prevWhichScene = whichScene;
}

void LevelDialog::on_scene3Btn_clicked()
{
    whichScene = 3;
    if(whichScene != prevWhichScene)
        SetUnchecked(prevWhichScene);
    prevWhichScene = whichScene;
}

void LevelDialog::on_scene4Btn_clicked()
{
    whichScene = 4;
    if(whichScene != prevWhichScene)
        SetUnchecked(prevWhichScene);
    prevWhichScene = whichScene;
}

void LevelDialog::on_scene5Btn_clicked()
{
    whichScene = 5;
    if(whichScene != prevWhichScene)
        SetUnchecked(prevWhichScene);
    prevWhichScene = whichScene;
}

void LevelDialog::on_okBtn_clicked()
{
    this->hide();
    emit UpdateWhichScene(whichScene);
    emit StopMenuBGM();

    gameWidget = new GameWidget;

    connect(gameWidget, SIGNAL(ShowMenu()), this, SIGNAL(ShowMenu()));
    gameWidget->show();
}

void LevelDialog::on_cancelBtn_clicked()
{
    this->hide();
    parent->show();
}

void LevelDialog::SetUnchecked(int index)
{
    switch(index)
    {
    case 1:
        ui->scene1Btn->setChecked(false);
        break;
    case 2:
        ui->scene2Btn->setChecked(false);
        break;
    case 3:
        ui->scene3Btn->setChecked(false);
        break;
    case 4:
        ui->scene4Btn->setChecked(false);
        break;
    case 5:
        ui->scene5Btn->setChecked(false);
        break;
    }
}

void LevelDialog::UpdateWinSceneIndex()
{
    ReadWinSceneIndex();
    switch(winSceneIndex)
    {
    case 0:
    case 1:
        ui->scene1Btn->setEnabled(true);
        break;
    case 2:
        ui->scene1Btn->setEnabled(true);
        ui->scene2Btn->setEnabled(true);
        break;
    case 3:
        ui->scene1Btn->setEnabled(true);
        ui->scene2Btn->setEnabled(true);
        ui->scene3Btn->setEnabled(true);
        break;
    case 4:
        ui->scene1Btn->setEnabled(true);
        ui->scene2Btn->setEnabled(true);
        ui->scene3Btn->setEnabled(true);
        ui->scene4Btn->setEnabled(true);
        break;
    case 5:
        ui->scene1Btn->setEnabled(true);
        ui->scene2Btn->setEnabled(true);
        ui->scene3Btn->setEnabled(true);
        ui->scene4Btn->setEnabled(true);
        ui->scene5Btn->setEnabled(true);
        break;
    }
}
