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
    void logic();//�����߼�����

    void closeEvent(QCloseEvent *);

private:
    Ui::GameWidget *ui;

    QTimer *tickTimer;
    QPixmap gameBackground;

    int difficulty;//��Ϸ�Ѷ� 1���� 2����ͨ 3������ 4����Ԩ
    int gameMode;//��Ϸģʽ 1��ð��ģʽ�����أ� 2������ģʽ
    int whichPlane;//�ɻ����� 1��interceptor 2��boomer 3:chopper
    int whichScene;//�ؿ��� 1 2 3 4 5��

    Phonon::MediaObject *gameBGM;
    Phonon::MediaObject *boom;
    Phonon::MediaObject *getItemAudio;
    //��̬����
    Phonon::AudioOutput *audioForBGM;//�������ֽڵ�
    Phonon::AudioOutput *audioForEffectsBoom;//��Ϸ��Ч�ڵ�(��ը)
    Phonon::AudioOutput *audioForEffectsItem;//��Ϸ��Ч�ڵ�(�õ�����)

    //����������
    Phonon::VolumeSlider *volumeSliderForBGM;
    Phonon::VolumeSlider *volumeSliderForEffects;
    //������ǩ
    QLabel *volumeLabelForBGM;
    QLabel *volumeLabelForEffects;

    PauseWidget *pauseWidget;
    NextSceneDialog *nextSceneDialog;

    WinWidget *winWidget;
    LoseWidget *loseWidegt;

    bool isPause;//�Ƿ���ͣ

    SceneManager *sceneManager;//����������
    SceneTitleAnim *sceneTitleAnim;//�������⶯��

    void InitPauseWidget();//��ʼ����ͣ����
    void InitNextSceneDialog();//��ʼ����һ�ؽ���
    void InitWinWidget();//��ʼ��ͨ�ؽ���
    void InitLoseWidget();//��ʼ��ʧ�ܽ���

    bool LoadDatabase();//�����ݿ��е�����

private slots:
    void Tick();//��ʱˢ��

    void Resume();
    void ReturnToMenu();
    void Restart();

    void Next();

    void ShowNextDialog();//��ʾ��һ�ؽ���
    void ShowWinDialog();//��ʾͨ�ؽ���
    void ShowLoseDialog();//��ʾʧ�ܽ���

    void GameBGMReplay();

signals:
    void ShowMenu();
};

#endif // GAMEWIDGET_H
