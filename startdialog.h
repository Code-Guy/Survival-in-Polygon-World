#ifndef STARTDIALOG_H
#define STARTDIALOG_H

#include <QDialog>
#include "leveldialog.h"

namespace Ui {
class StartDialog;
}

class StartDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit StartDialog(QWidget *parent = 0);
    ~StartDialog();
    
private slots:
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();
    void planeRadioChange();
    void modeRadioChange();

private:
    Ui::StartDialog *ui;

    QWidget *parent;
    LevelDialog *levelDialog;

    QPixmap plane1, plane2, plane3, planeMask;
    int whichPlane;
    int gameMode;

    void SetUnchecked(int index);

signals:
    void UpdateGameMode(int gameMode);//更新游戏模式
    void UpdateWhichPlane(int whichPlane);//更新飞机类型
    void UpdateWhichScene(int whichScene);//更新关卡号
    void StopMenuBGM();
    void ShowMenu();
};

#endif // STARTDIALOG_H
