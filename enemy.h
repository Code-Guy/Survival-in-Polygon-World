#ifndef ENEMY_H
#define ENEMY_H

#include <QPixmap>
#include <QPainter>
#include "constants.h"
#include "bullet.h"

struct VelPos
{
    Vector2D vel;
    Pos2D pos;

    VelPos(Vector2D vel, Pos2D pos) : vel(vel), pos(pos)
    {}
};

class Enemy
{
public:
    Enemy(EnemyParams enemyParams);

    void Draw(QPainter *painter);
    void Logic(QList<EnemyBullet *> &enemyBullets);

    void SetHp(int hp);
    int GetHp();

    bool IsDead() const;
    void SetDead(bool flag);

    Pos2D GetPos() const;
    int GetImgW() const;
    int GetImgH() const;
    int GetAward() const;

    EnemyShapeType GetShapeType() const;

private:
    Pos2D pos;
    Pos2D centerPos;//敌人的中心位置
    Pos2D originPos;//敌机的原始位置
    double metaAngle;//元角（弧度制）
    double metaAngleDegree;//元角（角度制）
    //各种类型的敌人
    EnemyShapeType enemyShapeType;
    EnemySplitMode enemySplitMode;
    EnemyBounceMode enemyBounceMode;
    EnemyGenBulletMode enemyGenBulletMode;
    EnemyRotateMode enemyRotateMode;
    EnemyMoveMode enemyMoveMode;
    EnemyTrackMode enemyTrackMode;
    EnemyDestroyedMode enemyDestroyedMode;

    bool isTrack;
    bool isUndestroyed;

    QPixmap imgEnemy;
    QPixmap imgBullet;
    Vector2D velocity;
    bool isDead;
    int polygonIndex;
    int imgW, imgH;//敌机图片的长和宽
    int hp;//敌人的血量，根据敌人的类型来定
    double speed;//敌人的子弹的速率

    int bulletTimer;//发射子弹的计时器
    int bulletTime;//发射子弹的时间间隔
    bool isGenBullet;//是否产生子弹

    //单边循环发射的计时器
    int loopEdgeTimer;
    int loopEdgeTime;
    int loopEdgeIndex;//当前旋转到了哪一个边

    double rotateAngle;//旋转的角度

    Pos2D circleCenter;
    int moveFrameCount;
    int award;//奖励

    //根据敌机类型判断敌机的外形
    void SwitchEnemyImg(EnemyShapeType enemyShapeType,
                              EnemySplitMode enemySplitMode,
                              EnemyBounceMode enemyBounceMode);
    void DetermineEnemyImg(EnemySplitMode enemySplitMode,
                              EnemyBounceMode enemyBounceMode,
                              int index);

    void GenBullets(QList<EnemyBullet *> &enemyBullets);//生成子弹
    void GetBullets(QList<EnemyBullet *> &enemyBullets);
    void UpdateEnemyPos();//更新敌机的位置
    void ClampEnemyPos();//防止敌机的位置超出屏幕
    bool IsOutOfLeftRightEdge() const;//是否在左右边界
    bool IsOutOfUpBottomEdge() const;//是否在上下边界

    QList<VelPos> GetShootVelPos();//得到发射子弹的速度和位置
};

#endif // ENEMY_H
