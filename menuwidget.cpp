#include "menuwidget.h"
#include "ui_menuwidget.h"
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include "globals.h"

MenuWidget::MenuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuWidget)
{
    ui->setupUi(this);
    this->setFixedSize(800, 600);
    this->setWindowTitle("Survival");

    tickTimer = new QTimer(this);

    isRight = true;
    connect(tickTimer, SIGNAL(timeout()), this, SLOT(Tick()));
    tickTimer->start(20);

    playerPos = QPoint(500, 450);

    bullet = QPixmap(":/images/bullet.png");
    player = QPixmap(":/images/player1.png");
    title = QPixmap(":/images/title.png");
    menuBackground = QPixmap(":/images/menu_background.png");

    menuBGM = new Phonon::MediaObject(this);//���˵���������
    audioForBGM = new Phonon::AudioOutput(Phonon::MusicCategory, this);//�������ֽڵ�
    audioForEffects = new Phonon::AudioOutput(Phonon::MusicCategory, this);//��Ч�ڵ�
    Phonon::createPath(menuBGM, audioForBGM);//����·��
    menuBGM->setCurrentSource(Phonon::MediaSource("musics/menu.mp3"));
    menuBGM->play();

    connect(menuBGM, SIGNAL(aboutToFinish()), this, SLOT(MenuBGMReplay()));

    difficulty = 2;//1���� 2����ͨ 3������ 4����Ԩ
    gameMode = 1;//Ĭ��Ϊð��ģʽ
    whichPlane = 1;//Ĭ��Ϊ��һ�ܷɻ�

    helpWidget = new HelpWidget;
    statsWidget =new StatsWidget;
    InitSetDialog();//��ʼ���������ý���
    InitStartDialog();//��ʼ����ʼ���ý���

    InitDatabase();//��ʼ�����ݿ�
}

MenuWidget::~MenuWidget()
{
    delete ui;

    delete tickTimer;

    delete volumeLabelForBGM;
    delete volumeLabelForEffects;

    //��ת�Ľ���
    delete setDialog;
    delete startDialog;
    delete helpWidget;
    delete statsWidget;
}

void MenuWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPen pen(Qt::white);
    QBrush brush(Qt::white);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(rect());

    painter.drawPixmap(0, 0, menuBackground);

    //����
    painter.save();
    QPen penSignature(Qt::black);
    QBrush brushSignature(Qt::black);
    QFont font("Consolas", 15, QFont::Light, true);
    painter.setPen(penSignature);
    painter.setBrush(brushSignature);
    painter.setFont(font);

    painter.drawText(QRect(40, 565, 200, 50), "Made By GJJ");
    painter.restore();

    UpdateScene();//�������Ǻ��ӵ���λ��
    //������Ϸ��
    painter.drawPixmap(380, 50, title);
    //��������
    painter.drawPixmap(playerPos.x(), playerPos.y(), player);
    //�����ӵ�
    for(int i=0; i<bulletPos.size(); i++)
    {
        painter.drawPixmap(bulletPos[i].x(), bulletPos[i].y(), bullet);
    }
}

void MenuWidget::UpdateScene()//�������Ǻ��ӵ���λ��
{
    if(isRight && playerPos.x() > 600)
    {
        bulletPos.append(QPoint(playerPos.x()+player.width()/2, playerPos.y()));
        isRight = false;
    }
    if(!isRight && playerPos.x() < 400)
    {
        bulletPos.append(QPoint(playerPos.x()+player.width()/2, playerPos.y()));
        isRight = true;
    }

    if(isRight)
        playerPos.setX(playerPos.x()+2);
    else
        playerPos.setX(playerPos.x()-2);

    QList<int> removeIndices;

    for(int i=0; i<bulletPos.size(); i++)
    {
        if(bulletPos[i].y() + bullet.height() <= 0)
            removeIndices.append(i);
        else
        {
            bulletPos[i].setY(bulletPos[i].y()-3);
        }
    }

    for(int i=0; i<removeIndices.size(); i++)
    {
        bulletPos.removeAt(removeIndices[i]);
    }
}

void MenuWidget::InitSetDialog()//��ʼ���������ý���
{
    setDialog = new SetDialog(this);
    volumeSliderForBGM = new Phonon::VolumeSlider(audioForBGM, setDialog);
    volumeSliderForEffects = new Phonon::VolumeSlider(audioForEffects, setDialog);

    volumeLabelForBGM = new QLabel(tr("music��"), setDialog);
    volumeLabelForEffects = new QLabel(tr("effect��"), setDialog);

    volumeSliderForBGM->move(100, 70);
    volumeSliderForEffects->move(100, 170);
    volumeLabelForBGM->move(60, 75);
    volumeLabelForEffects->move(60, 175);

    connect(setDialog, SIGNAL(UpdateDifficulty(int)), this, SLOT(UpdateDifficulty(int)));//�����Ѷ�
}

void MenuWidget::InitStartDialog()//��ʼ����ʼ���ý���
{
    startDialog = new StartDialog(this);
    connect(startDialog, SIGNAL(UpdateGameMode(int)), this, SLOT(UpdateGameMode(int)));//������Ϸģʽ
    connect(startDialog, SIGNAL(UpdateWhichPlane(int)), this, SLOT(UpdateWhichPlane(int)));//���·ɻ�����
    connect(startDialog, SIGNAL(UpdateWhichScene(int)), this, SLOT(UpdateWhichScene(int)));
    connect(startDialog, SIGNAL(StopMenuBGM()), SLOT(StopMenuBGM()));
    connect(startDialog, SIGNAL(ShowMenu()), this,SLOT(ShowMenu()));
}

void MenuWidget::Tick()
{
    update();
}

void MenuWidget::on_helpBtn_clicked()
{
    helpWidget->show();
}

void MenuWidget::on_startBtn_clicked()
{
    this->hide();
    startDialog->show();
}

void MenuWidget::on_setBtn_clicked()
{
    setDialog->show();
}

void MenuWidget::on_aboutBtn_clicked()
{
    statsWidget->UpdateStats();//����һ������

    //for(int i=0; i<ite)
    statsWidget->show();
}

void MenuWidget::on_exitBtn_clicked()
{
    this->close();
}

void MenuWidget::UpdateDifficulty(int difficulty)//������Ϸ�Ѷ�
{
    this->difficulty = difficulty;
    qDebug() << difficulty;
    if(!query.exec(QString("update gameparam set difficulty = %1").arg(difficulty)))
    {
        qDebug() << "update wrong!";
    }
    //db.commit();
}

void MenuWidget::UpdateGameMode(int gameMode)//������Ϸģʽ
{
    this->gameMode = gameMode;
    qDebug() << gameMode;
    query.exec(QString("update gameparam set gameMode = %1").arg(gameMode));
    //db.commit();
}

void MenuWidget::UpdateWhichPlane(int whichPlane)//���·ɻ�����
{
    this->whichPlane = whichPlane;
    qDebug() << whichPlane;
    query.exec(QString("update gameparam set whichPlane = %1").arg(whichPlane));
    //db.commit();
}

void MenuWidget::UpdateWhichScene(int whichScene)//���¹ؿ���
{
    this->whichScene = whichScene;
    qDebug() << whichScene << "1";
    query.exec(QString("update gameparam set whichScene = %1").arg(whichScene));
    db.commit();
}

void MenuWidget::StopMenuBGM()
{
    menuBGM->stop();
}

void MenuWidget::ShowMenu()
{
    this->show();
    menuBGM->play();
}

void MenuWidget::MenuBGMReplay()
{
    qDebug() << "replay";
    menuBGM->stop();
    menuBGM->play();
}

void MenuWidget::closeEvent(QCloseEvent *)
{
    UpdateDatabase();//�������ݿ�
    db.close();//�ر����ݿ�
}
