#include "gamewidget.h"
#include "ui_gamewidget.h"
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>
#include "globals.h"

GameWidget::GameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("Game");

    tickTimer = new QTimer(this);
    connect(tickTimer, SIGNAL(timeout()), this, SLOT(Tick()));
    tickTimer->start(30);

    gameBackground = QPixmap(":/images/game_background.png");

    LoadDatabase();//读数据库中的数据

    boom = new Phonon::MediaObject(this);//敌机爆炸音效
    gameBGM = new Phonon::MediaObject(this);//主菜单背景音乐

    getItemAudio = new Phonon::MediaObject(this);//得到道具的音效

    audioForBGM = new Phonon::AudioOutput(Phonon::MusicCategory, this);//背景音乐节点
    audioForEffectsBoom = new Phonon::AudioOutput(Phonon::MusicCategory, this);//音效节点
    audioForEffectsItem = new Phonon::AudioOutput(Phonon::MusicCategory, this);//音效节点
    Phonon::createPath(gameBGM, audioForBGM);//创建路径
    Phonon::createPath(boom, audioForEffectsBoom);
    Phonon::createPath(getItemAudio, audioForEffectsItem);
    gameBGM->setCurrentSource(Phonon::MediaSource("musics/game.mp3"));
    gameBGM->play();

    boom->setCurrentSource(Phonon::MediaSource("musics/boom.mp3"));
    getItemAudio->setCurrentSource(Phonon::MediaSource("musics/item.mp3"));

    connect(gameBGM, SIGNAL(aboutToFinish()), this, SLOT(GameBGMReplay()));

    isPause = false;//先暂停界面，播放一段动画

    sceneManager = new SceneManager(whichScene, whichPlane, boom, getItemAudio);

    connect(sceneManager, SIGNAL(ShowNextDialog()), this, SLOT(ShowNextDialog()));
    connect(sceneManager, SIGNAL(ShowWinDialog()), this, SLOT(ShowWinDialog()));
    connect(sceneManager, SIGNAL(ShowLoseDialog()), this, SLOT(ShowLoseDialog()));

    sceneTitleAnim = new SceneTitleAnim(whichScene);

    InitPauseWidget();//初始化暂停界面
    InitNextSceneDialog();//初始化下一关界面
    InitWinWidget();//初始化通关界面
    InitLoseWidget();//初始化失败界面
}

GameWidget::~GameWidget()
{
    delete ui;

    delete tickTimer;
    delete pauseWidget;
    delete nextSceneDialog;
    delete winWidget;
    delete loseWidegt;

    delete volumeLabelForBGM;
    delete volumeLabelForEffects;

    delete sceneManager;//场景管理器
    delete sceneTitleAnim;//开场标题动画
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    if(!isPause)
        logic();
    QPainter painter(this);
    QPen pen(Qt::white);
    QBrush brush(Qt::white);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(rect());

    painter.drawPixmap(0, 0, gameBackground);

    if(!sceneTitleAnim->IsPlayEnd())
    {
        sceneTitleAnim->Draw(&painter);
    }

    sceneManager->Draw(&painter);
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    sceneManager->HandlePressKey(event);
    if(event->key() == Qt::Key_Escape)
    {
        isPause = true;
        pauseWidget->show();
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    sceneManager->HandleReleaseKey(event);
}

void GameWidget::logic()
{
    if(!sceneTitleAnim->IsPlayEnd())
    {
        sceneTitleAnim->Logic();
    }
    else
    {
        sceneManager->Logic();
    }
}

void GameWidget::Tick()//定时刷新
{
    update();
}

void GameWidget::InitPauseWidget()//初始化暂停界面
{
    pauseWidget = new PauseWidget;
    volumeSliderForBGM = new Phonon::VolumeSlider(audioForBGM, pauseWidget);
    volumeSliderForEffects = new Phonon::VolumeSlider(audioForEffectsBoom, pauseWidget);
    //volumeSliderForEffects->setAudioOutput(audioForEffectsBoom);

    volumeLabelForBGM = new QLabel(tr("music："), pauseWidget);
    volumeLabelForEffects = new QLabel(tr("effect："), pauseWidget);

    volumeSliderForBGM->move(130, 170);
    volumeSliderForEffects->move(130, 270);
    volumeLabelForBGM->move(90, 175);
    volumeLabelForEffects->move(90, 275);

    connect(pauseWidget, SIGNAL(Resume()), this, SLOT(Resume()));
    connect(pauseWidget, SIGNAL(ReturnToMenu()), this, SLOT(ReturnToMenu()));
    connect(pauseWidget, SIGNAL(Restart()), this, SLOT(Restart()));
}

void GameWidget::InitNextSceneDialog()//初始化下一关界面
{
    nextSceneDialog = new NextSceneDialog;

    connect(nextSceneDialog, SIGNAL(Next()), this, SLOT(Next()));
    connect(nextSceneDialog, SIGNAL(ReturnToMenu()), this, SLOT(ReturnToMenu()));
    connect(nextSceneDialog, SIGNAL(Restart()), this, SLOT(Restart()));
}

void GameWidget::InitWinWidget()//初始化通关界面
{
    winWidget = new WinWidget;

    connect(winWidget, SIGNAL(ReturnToMenu()), this, SLOT(ReturnToMenu()));
    connect(winWidget, SIGNAL(Restart()), this, SLOT(Restart()));
}

void GameWidget::InitLoseWidget()//初始化失败界面
{
    loseWidegt = new LoseWidget;

    connect(loseWidegt, SIGNAL(ReturnToMenu()), this, SLOT(ReturnToMenu()));
    connect(loseWidegt, SIGNAL(Restart()), this, SLOT(Restart()));
}

void GameWidget::Resume()
{
    isPause = false;
}

void GameWidget::ReturnToMenu()
{
    //更新数据统计
    UpdateDatabase();

    gameBGM->stop();
    this->close();
    emit ShowMenu();
}

void GameWidget::Restart()
{
    sceneManager->Restart();
    isPause = false;
}

bool GameWidget::LoadDatabase()//读数据库中的数据
{
    if(!query.exec("select * from gameparam"))
    {
        qDebug() << "selet wrong!";
        return false;
    }
    //访问数据库中的数据
    while(query.next())
    {
        difficulty = query.value(0).toInt();//游戏难度 1：简单 2：普通 3：困难 4：深渊
        gameMode = query.value(1).toInt();//游戏模式 1：冒险模式（闯关） 2：生存模式
        whichPlane = query.value(2).toInt();//飞机类型 1：interceptor 2：boomer 3:chopper
        whichScene = query.value(3).toInt();//关卡号 1 2 3 4 5关
    }

    return true;
}

void GameWidget::Next()
{
    whichScene++;
    sceneManager->SetWhichScene(sceneManager->GetWhichScene()+1);
    sceneManager->Restart();
    sceneTitleAnim->SetSceneIndex(whichScene);
    isPause = false;
    sceneTitleAnim->Play();
}

void GameWidget::ShowNextDialog()
{
    isPause = true;
    nextSceneDialog->show();
}

void GameWidget::GameBGMReplay()
{
    qDebug() << "replay";
    gameBGM->stop();
    gameBGM->play();
}

void GameWidget::ShowWinDialog()//显示通关界面
{
    isPause = true;
    winWidget->show();
}

void GameWidget::ShowLoseDialog()//显示失败界面
{
    isPause = true;
    loseWidegt->show();
}

void GameWidget::closeEvent(QCloseEvent *)
{
    UpdateDatabase();
}
