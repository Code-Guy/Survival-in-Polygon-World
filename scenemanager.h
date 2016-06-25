#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <QKeyEvent>
#include <phonon>
#include <QObject>

#include "player.h"
#include "enemy.h"
#include "boomanim.h"
#include "item.h"

class SceneManager : public QObject
{
    Q_OBJECT
public:
    SceneManager(int whichScene, int whichPlane,
                 Phonon::MediaObject *boom, Phonon::MediaObject *getItemAudio,
                 QObject *parent = 0);

    void Draw(QPainter *painter);
    void Logic();

    void HandlePressKey(QKeyEvent *);
    void HandleReleaseKey(QKeyEvent *);

    void Restart();

    int GetWhichScene() const;
    void SetWhichScene(int whichScene);

private:
    Player *player;
    QList<Enemy> enemies;
    QList<EnemyBullet *> enemyBullets;//ע�⣬ֻ�����ָ����ж�̬��
    QList<BoomAnim> boomAnims;
    QList<Item> items;

    int whichScene;//������� 0 1 2
    int whichPlane;//�ɻ����

    //��Ч
    Phonon::MediaObject *boom;
    Phonon::MediaObject *getItemAudio;

    int deadEnemyCount;//�Ѿ������ĵл��ĸ���
    int genEnemiesCount;//�Ѿ������˵ڼ����л�

    int genItemTimer;//���ɵ��ߵļ�ʱ��

    void InitScene();//��ʼ������

    void HandlePosUpdate();//�����������壨�ӵ������˵��ƶ���
    void HandleCollisionDetect();//������ײ���
    void HandleRemoveStuff();//�����������壨�ӵ������˵�ɾ����

    void HandleSpecialBullets();//����һЩ������ӵ����ɷ��ѵģ�

    void HandleItem(ItemType itemType);

    void GenEnemy();//�����µĵл�
    void GenItem();//���ɵ���

    bool IsOutOfScreen(Pos2D pos, int imgW, int imgH);

    //���µ��˺͵��ߵ�ͳ������
    void UpdateEnemyStats(EnemyShapeType shape);
    void UpdateItemStats(ItemType itemType);

signals:
    void ShowNextDialog();
    void ShowWinDialog();
    void ShowLoseDialog();

public slots:

};

extern QList< QList<EnemyParams> > EnemyClusters[];
extern double ItemRatios[];

void InitEnemyCluster();//��ʼ���л�Ⱥ

#endif // SCENEMANAGER_H
