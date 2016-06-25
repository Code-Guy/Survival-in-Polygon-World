#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <phonon>
#include <QTimer>
#include <QList>
#include <QLabel>

#include "scenemanager.h"
#include "pausewidget.h"
#include "scenetitleanim.h"
#include "nextscenedialog.h"
#include "winwidget.h"
#include "losewidget.h"

namespace Ui {
class GameWidget;
}

class GameWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();
    
protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void logic();//进行逻辑处理

    void closeEvent(QCloseEvent *);

private:
    Ui::GameWidget *ui;

    QTimer *tickTimer;
    QPixmap gameBackground;

    int difficulty;//游戏难度 1：简单 2：普通 3：困难 4：深渊
    int gameMode;//游戏模式 1：冒险模式（闯关） 2：生存模式
    int whichPlane;//飞机类型 1：interceptor 2：boomer 3:chopper
    int whichScene;//关卡号 1 2 3 4 5关

    Phonon::MediaObject *gameBGM;
    Phonon::MediaObject *boom;
    Phonon::MediaObject *getItemAudio;
    //静态变量
    Phonon::AudioOutput *audioForBGM;//背景音乐节点
    Phonon::AudioOutput *audioForEffectsBoom;//游戏音效节点(爆炸)
    Phonon::AudioOutput *audioForEffectsItem;//游戏音效节点(得到道具)

    //两个进度条
    Phonon::VolumeSlider *volumeSliderForBGM;
    Phonon::VolumeSlider *volumeSliderForEffects;
    //两个标签
    QLabel *volumeLabelForBGM;
    QLabel *volumeLabelForEffects;

    PauseWidget *pauseWidget;
    NextSceneDialog *nextSceneDialog;

    WinWidget *winWidget;
    LoseWidget *loseWidegt;

    bool isPause;//是否暂停

    SceneManager *sceneManager;//场景管理器
    SceneTitleAnim *sceneTitleAnim;//开场标题动画

    void InitPauseWidget();//初始化暂停界面
    void InitNextSceneDialog();//初始化下一关界面
    void InitWinWidget();//初始化通关界面
    void InitLoseWidget();//初始化失败界面

    bool LoadDatabase();//读数据库中的数据

private slots:
    void Tick();//定时刷新

    void Resume();
    void ReturnToMenu();
    void Restart();

    void Next();

    void ShowNextDialog();//显示下一关界面
    void ShowWinDialog();//显示通关界面
    void ShowLoseDialog();//显示失败界面

    void GameBGMReplay();

signals:
    void ShowMenu();
};

#endif // GAMEWIDGET_H
