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
    bool isRight;//�����Ƿ����ң����ƶ�

    //��ת�Ľ���
    SetDialog *setDialog;
    StartDialog *startDialog;
    HelpWidget *helpWidget;
    StatsWidget *statsWidget;

    QList<QPoint> bulletPos;
    QPoint playerPos;

    Phonon::MediaObject *menuBGM;
    //��̬����
    Phonon::AudioOutput *audioForBGM;//�������ֽڵ�
    Phonon::AudioOutput *audioForEffects;//��Ϸ��Ч�ڵ�

    //����������
    Phonon::VolumeSlider *volumeSliderForBGM;
    Phonon::VolumeSlider *volumeSliderForEffects;
    //������ǩ
    QLabel *volumeLabelForBGM;
    QLabel *volumeLabelForEffects;

    int difficulty;//��Ϸ�Ѷ� 1���� 2����ͨ 3������ 4����Ԩ
    int gameMode;//��Ϸģʽ 1��ð��ģʽ�����أ� 2������ģʽ
    int whichPlane;//�ɻ����� 1��interceptor 2��boomer 3:chopper
    int whichScene;//�ؿ��� 1 2 3 4 5��
    void UpdateScene();//�������Ǻ��ӵ���λ��

    void InitSetDialog();//��ʼ���������ý���
    void InitStartDialog();//��ʼ����ʼ���ý���

private slots:
    void Tick();//��ʱˢ��
    void UpdateDifficulty(int difficulty);//������Ϸ�Ѷ�
    void UpdateGameMode(int gameMode);//������Ϸģʽ
    void UpdateWhichPlane(int whichPlane);//���·ɻ�����
    void UpdateWhichScene(int whichScene);//���¹ؿ���
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
