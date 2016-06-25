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

    LoadDatabase();//�����ݿ��е�����

    boom = new Phonon::MediaObject(this);//�л���ը��Ч
    gameBGM = new Phonon::MediaObject(this);//���˵���������

    getItemAudio = new Phonon::MediaObject(this);//�õ����ߵ���Ч

    audioForBGM = new Phonon::AudioOutput(Phonon::MusicCategory, this);//�������ֽڵ�
    audioForEffectsBoom = new Phonon::AudioOutput(Phonon::MusicCategory, this);//��Ч�ڵ�
    audioForEffectsItem = new Phonon::AudioOutput(Phonon::MusicCategory, this);//��Ч�ڵ�
    Phonon::createPath(gameBGM, audioForBGM);//����·��
    Phonon::createPath(boom, audioForEffectsBoom);
    Phonon::createPath(getItemAudio, audioForEffectsItem);
    gameBGM->setCurrentSource(Phonon::MediaSource("musics/game.mp3"));
    gameBGM->play();

    boom->setCurrentSource(Phonon::MediaSource("musics/boom.mp3"));
    getItemAudio->setCurrentSource(Phonon::MediaSource("musics/item.mp3"));

    connect(gameBGM, SIGNAL(aboutToFinish()), this, SLOT(GameBGMReplay()));

    isPause = false;//����ͣ���棬����һ�ζ���

    sceneManager = new SceneManager(whichScene, whichPlane, boom, getItemAudio);

    connect(sceneManager, SIGNAL(ShowNextDialog()), this, SLOT(ShowNextDialog()));
    connect(sceneManager, SIGNAL(ShowWinDialog()), this, SLOT(ShowWinDialog()));
    connect(sceneManager, SIGNAL(ShowLoseDialog()), this, SLOT(ShowLoseDialog()));

    sceneTitleAnim = new SceneTitleAnim(whichScene);

    InitPauseWidget();//��ʼ����ͣ����
    InitNextSceneDialog();//��ʼ����һ�ؽ���
    InitWinWidget();//��ʼ��ͨ�ؽ���
    InitLoseWidget();//��ʼ��ʧ�ܽ���
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

    delete sceneManager;//����������
    delete sceneTitleAnim;//�������⶯��
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

void GameWidget::Tick()//��ʱˢ��
{
    update();
}

void GameWidget::InitPauseWidget()//��ʼ����ͣ����
{
    pauseWidget = new PauseWidget;
    volumeSliderForBGM = new Phonon::VolumeSlider(audioForBGM, pauseWidget);
    volumeSliderForEffects = new Phonon::VolumeSlider(audioForEffectsBoom, pauseWidget);
    //volumeSliderForEffects->setAudioOutput(audioForEffectsBoom);

    volumeLabelForBGM = new QLabel(tr("music��"), pauseWidget);
    volumeLabelForEffects = new QLabel(tr("effect��"), pauseWidget);

    volumeSliderForBGM->move(130, 170);
    volumeSliderForEffects->move(130, 270);
    volumeLabelForBGM->move(90, 175);
    volumeLabelForEffects->move(90, 275);

    connect(pauseWidget, SIGNAL(Resume()), this, SLOT(Resume()));
    connect(pauseWidget, SIGNAL(ReturnToMenu()), this, SLOT(ReturnToMenu()));
    connect(pauseWidget, SIGNAL(Restart()), this, SLOT(Restart()));
}

void GameWidget::InitNextSceneDialog()//��ʼ����һ�ؽ���
{
    nextSceneDialog = new NextSceneDialog;

    connect(nextSceneDialog, SIGNAL(Next()), this, SLOT(Next()));
    connect(nextSceneDialog, SIGNAL(ReturnToMenu()), this, SLOT(ReturnToMenu()));
    connect(nextSceneDialog, SIGNAL(Restart()), this, SLOT(Restart()));
}

void GameWidget::InitWinWidget()//��ʼ��ͨ�ؽ���
{
    winWidget = new WinWidget;

    connect(winWidget, SIGNAL(ReturnToMenu()), this, SLOT(ReturnToMenu()));
    connect(winWidget, SIGNAL(Restart()), this, SLOT(Restart()));
}

void GameWidget::InitLoseWidget()//��ʼ��ʧ�ܽ���
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
    //��������ͳ��
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

bool GameWidget::LoadDatabase()//�����ݿ��е�����
{
    if(!query.exec("select * from gameparam"))
    {
        qDebug() << "selet wrong!";
        return false;
    }
    //�������ݿ��е�����
    while(query.next())
    {
        difficulty = query.value(0).toInt();//��Ϸ�Ѷ� 1���� 2����ͨ 3������ 4����Ԩ
        gameMode = query.value(1).toInt();//��Ϸģʽ 1��ð��ģʽ�����أ� 2������ģʽ
        whichPlane = query.value(2).toInt();//�ɻ����� 1��interceptor 2��boomer 3:chopper
        whichScene = query.value(3).toInt();//�ؿ��� 1 2 3 4 5��
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

void GameWidget::ShowWinDialog()//��ʾͨ�ؽ���
{
    isPause = true;
    winWidget->show();
}

void GameWidget::ShowLoseDialog()//��ʾʧ�ܽ���
{
    isPause = true;
    loseWidegt->show();
}

void GameWidget::closeEvent(QCloseEvent *)
{
    UpdateDatabase();
}
