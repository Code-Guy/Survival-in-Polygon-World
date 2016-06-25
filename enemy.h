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
    Pos2D centerPos;//���˵�����λ��
    Pos2D originPos;//�л���ԭʼλ��
    double metaAngle;//Ԫ�ǣ������ƣ�
    double metaAngleDegree;//Ԫ�ǣ��Ƕ��ƣ�
    //�������͵ĵ���
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
    int imgW, imgH;//�л�ͼƬ�ĳ��Ϳ�
    int hp;//���˵�Ѫ�������ݵ��˵���������
    double speed;//���˵��ӵ�������

    int bulletTimer;//�����ӵ��ļ�ʱ��
    int bulletTime;//�����ӵ���ʱ����
    bool isGenBullet;//�Ƿ�����ӵ�

    //����ѭ������ļ�ʱ��
    int loopEdgeTimer;
    int loopEdgeTime;
    int loopEdgeIndex;//��ǰ��ת������һ����

    double rotateAngle;//��ת�ĽǶ�

    Pos2D circleCenter;
    int moveFrameCount;
    int award;//����

    //���ݵл������жϵл�������
    void SwitchEnemyImg(EnemyShapeType enemyShapeType,
                              EnemySplitMode enemySplitMode,
                              EnemyBounceMode enemyBounceMode);
    void DetermineEnemyImg(EnemySplitMode enemySplitMode,
                              EnemyBounceMode enemyBounceMode,
                              int index);

    void GenBullets(QList<EnemyBullet *> &enemyBullets);//�����ӵ�
    void GetBullets(QList<EnemyBullet *> &enemyBullets);
    void UpdateEnemyPos();//���µл���λ��
    void ClampEnemyPos();//��ֹ�л���λ�ó�����Ļ
    bool IsOutOfLeftRightEdge() const;//�Ƿ������ұ߽�
    bool IsOutOfUpBottomEdge() const;//�Ƿ������±߽�

    QList<VelPos> GetShootVelPos();//�õ������ӵ����ٶȺ�λ��
};

#endif // ENEMY_H
