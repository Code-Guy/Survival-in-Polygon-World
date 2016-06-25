#ifndef BULLET_H
#define BULLET_H

#include <QPixmap>
#include <QPainter>
#include "constants.h"

class Bullet
{
public:
    Bullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet);

    virtual void Draw(QPainter *painter) = 0;
    virtual void Logic() = 0;
    bool IsOutOfScreen();//是否超出屏幕范围
    bool IsDead() const;

    void SetDead(bool flag);

    Pos2D GetPos() const;
    Vector2D GetVel() const;
    QPixmap GetImgBullet() const;

    int GetImgW() const;
    int GetImgH() const;
    int GetDamage() const;

protected:
    Vector2D velocity;//速度
    double speed;//速率
    Pos2D pos;//位置
    QPixmap imgBullet;//子弹的图片
    int damage;//伤害
    int imgW, imgH;//子弹的长和宽
    bool isDead;//是否消亡
};

class EnemyBullet;//类的前置声明

class PlayerBullet : public Bullet
{
public:
    PlayerBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet);

    virtual void Draw(QPainter *painter) = 0;
    virtual void Logic() = 0;

    bool IsCollisionWithEnemy(int enemyPosX, int enemyPosY, int enemyImgW, int enemyImgH) const;
    bool IsCollisionWithEnemyBullet(const EnemyBullet *const enemyBullet)  const;

    void SetHp(int hp);
    int GetHp() const;

protected:
    PlayerBulletType playerBulletType;
    int hp;
};

class PurePlayerBullet : public PlayerBullet
{
public:
    PurePlayerBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet);
    virtual void Draw(QPainter *painter);
    virtual void Logic();
};

class BoomPlayerBullet : public PlayerBullet
{
public:
    BoomPlayerBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet);
    virtual void Draw(QPainter *painter);
    virtual void Logic();
};

class WavePlayerBullet : public PlayerBullet
{
public:
    WavePlayerBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet);
    virtual void Draw(QPainter *painter);
    virtual void Logic();
};

class EnemyBullet : public Bullet
{
public:
    EnemyBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet, bool isUnDestroyed = false, bool isTracking = false);

    void Draw(QPainter *painter);
    virtual void Logic() = 0;

    bool IsUnDestroyed() const;
    bool IsTracking() const;

    bool IsCollisionWithPlayer(int playerPosX, int playerPosY, int playerImgW, int playerImgH);
    bool IsCollisionWithShield(int centerPosX, int centerPosY, int radius);

    EnemyBulletType GetEnemyBulletType();

protected:
    EnemyBulletType enemyBulletType;
    bool isUnDestroyed;//是不是可以被player的子弹消灭的
    bool isTracking;//是不是追踪弹
    Pos2D playerPos;//主角的位置

    void Target();//重新定位，追逐主角
};

class CommonEnemyBullet : public EnemyBullet
{
public:
    CommonEnemyBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet, bool isUnDestroyed = false, bool isTracking = false);

    virtual void Logic();
};

class BounceEnemyBullet : public EnemyBullet
{
public:
    BounceEnemyBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet, bool isUnDestroyed = false, bool isTracking = false);

    virtual void Logic();

private:
    int bounceCount;

    bool IsOutOfLeftRightEdge() const;//是否在左右边界
    bool IsOutOfUpBottomEdge() const;//是否在上下边界
};

class SplitEnemyBullet : public EnemyBullet
{
public:
    SplitEnemyBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet, bool isUnDestroyed = false, bool isTracking = false,
                     int splitNum = DefaultSplitNum, int splitTime = DefaultSplitTime);

    virtual void Logic();

    void ResetSplitTimer();
    bool IsSpliting() const;
    void SetSplitNum(int splitNum);//设置分裂的次数
    int GetSplitNum();//获取分裂次数

private:
    bool isSplit;//是否在分裂
    int splitTimer;//分裂计时器
    int splitNum;//可以分裂的次数
    int splitTime;//每次分裂所需的时间
};

class SBEnemyBullet : public EnemyBullet
{
public:
    SBEnemyBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet, bool isUnDestroyed = false, bool isTracking = false,
                  int splitNum = DefaultSplitNum, int splitTime = DefaultSplitTime);

    virtual void Logic();

    void ResetSplitTimer();
    bool IsSpliting() const;
    void SetSplitNum(int splitNum);//设置分裂的次数
    int GetSplitNum();//获取分裂次数

private:
    bool isSplit;//是否处于分裂阶段
    int splitTimer;//分裂计时器
    int splitNum;//可以分裂的次数
    int splitTime;//每次分裂所需的时间

    int bounceCount;

    bool IsOutOfLeftRightEdge() const;//是否在左右边界
    bool IsOutOfUpBottomEdge() const;//是否在上下边界
};

#endif // BULLET_H
