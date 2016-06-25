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
    QList<EnemyBullet *> enemyBullets;//注意，只有类的指针才有多态性
    QList<BoomAnim> boomAnims;
    QList<Item> items;

    int whichScene;//场景编号 0 1 2
    int whichPlane;//飞机编号

    //音效
    Phonon::MediaObject *boom;
    Phonon::MediaObject *getItemAudio;

    int deadEnemyCount;//已经消亡的敌机的个数
    int genEnemiesCount;//已经生成了第几波敌机

    int genItemTimer;//生成道具的计时器

    void InitScene();//初始化场景

    void HandlePosUpdate();//处理所有物体（子弹、敌人的移动）
    void HandleCollisionDetect();//处理碰撞检测
    void HandleRemoveStuff();//处理所有物体（子弹、敌人的删除）

    void HandleSpecialBullets();//处理一些特殊的子弹（可分裂的）

    void HandleItem(ItemType itemType);

    void GenEnemy();//创建新的敌机
    void GenItem();//生成道具

    bool IsOutOfScreen(Pos2D pos, int imgW, int imgH);

    //更新敌人和道具的统计数据
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

void InitEnemyCluster();//初始化敌机群

#endif // SCENEMANAGER_H
