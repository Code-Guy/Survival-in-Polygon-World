#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
#include <QList>
#include <QPoint>
#include <QTimer>
#include <phonon>
#include <QLabel>

#include "setdialog.h"
#include "startdialog.h"
#include "helpwidget.h"
#include "statswidget.h"

namespace Ui {
class MenuWidget;
}

class MenuWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MenuWidget(QWidget *parent = 0);
    ~MenuWidget();

protected:
    void paintEvent(QPaintEvent *event);
    void closeEvent(QCloseEvent *);

private:
    Ui::MenuWidget *ui;
    QTimer *tickTimer;
    QPixmap bullet;
    QPixmap player;
    QPixmap title;
    QPixmap menuBackground;
    bool isRight;//主角是否向右（左）移动

    //跳转的界面
    SetDialog *setDialog;
    StartDialog *startDialog;
    HelpWidget *helpWidget;
    StatsWidget *statsWidget;

    QList<QPoint> bulletPos;
    QPoint playerPos;

    Phonon::MediaObject *menuBGM;
    //静态变量
    Phonon::AudioOutput *audioForBGM;//背景音乐节点
    Phonon::AudioOutput *audioForEffects;//游戏音效节点

    //两个进度条
    Phonon::VolumeSlider *volumeSliderForBGM;
    Phonon::VolumeSlider *volumeSliderForEffects;
    //两个标签
    QLabel *volumeLabelForBGM;
    QLabel *volumeLabelForEffects;

    int difficulty;//游戏难度 1：简单 2：普通 3：困难 4：深渊
    int gameMode;//游戏模式 1：冒险模式（闯关） 2：生存模式
    int whichPlane;//飞机类型 1：interceptor 2：boomer 3:chopper
    int whichScene;//关卡号 1 2 3 4 5关
    void UpdateScene();//更新主角和子弹的位置

    void InitSetDialog();//初始化参数设置界面
    void InitStartDialog();//初始化开始设置界面

private slots:
    void Tick();//定时刷新
    void UpdateDifficulty(int difficulty);//更新游戏难度
    void UpdateGameMode(int gameMode);//更新游戏模式
    void UpdateWhichPlane(int whichPlane);//更新飞机类型
    void UpdateWhichScene(int whichScene);//更新关卡号
    void StopMenuBGM();
    void ShowMenu();

    void MenuBGMReplay();

    void on_helpBtn_clicked();
    void on_startBtn_clicked();
    void on_setBtn_clicked();
    void on_aboutBtn_clicked();
    void on_exitBtn_clicked();
};

#endif // MENUWIDGET_H
