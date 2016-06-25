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
    bool IsOutOfScreen();//�Ƿ񳬳���Ļ��Χ
    bool IsDead() const;

    void SetDead(bool flag);

    Pos2D GetPos() const;
    Vector2D GetVel() const;
    QPixmap GetImgBullet() const;

    int GetImgW() const;
    int GetImgH() const;
    int GetDamage() const;

protected:
    Vector2D velocity;//�ٶ�
    double speed;//����
    Pos2D pos;//λ��
    QPixmap imgBullet;//�ӵ���ͼƬ
    int damage;//�˺�
    int imgW, imgH;//�ӵ��ĳ��Ϳ�
    bool isDead;//�Ƿ�����
};

class EnemyBullet;//���ǰ������

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
    bool isUnDestroyed;//�ǲ��ǿ��Ա�player���ӵ������
    bool isTracking;//�ǲ���׷�ٵ�
    Pos2D playerPos;//���ǵ�λ��

    void Target();//���¶�λ��׷������
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

    bool IsOutOfLeftRightEdge() const;//�Ƿ������ұ߽�
    bool IsOutOfUpBottomEdge() const;//�Ƿ������±߽�
};

class SplitEnemyBullet : public EnemyBullet
{
public:
    SplitEnemyBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet, bool isUnDestroyed = false, bool isTracking = false,
                     int splitNum = DefaultSplitNum, int splitTime = DefaultSplitTime);

    virtual void Logic();

    void ResetSplitTimer();
    bool IsSpliting() const;
    void SetSplitNum(int splitNum);//���÷��ѵĴ���
    int GetSplitNum();//��ȡ���Ѵ���

private:
    bool isSplit;//�Ƿ��ڷ���
    int splitTimer;//���Ѽ�ʱ��
    int splitNum;//���Է��ѵĴ���
    int splitTime;//ÿ�η��������ʱ��
};

class SBEnemyBullet : public EnemyBullet
{
public:
    SBEnemyBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet, bool isUnDestroyed = false, bool isTracking = false,
                  int splitNum = DefaultSplitNum, int splitTime = DefaultSplitTime);

    virtual void Logic();

    void ResetSplitTimer();
    bool IsSpliting() const;
    void SetSplitNum(int splitNum);//���÷��ѵĴ���
    int GetSplitNum();//��ȡ���Ѵ���

private:
    bool isSplit;//�Ƿ��ڷ��ѽ׶�
    int splitTimer;//���Ѽ�ʱ��
    int splitNum;//���Է��ѵĴ���
    int splitTime;//ÿ�η��������ʱ��

    int bounceCount;

    bool IsOutOfLeftRightEdge() const;//�Ƿ������ұ߽�
    bool IsOutOfUpBottomEdge() const;//�Ƿ������±߽�
};

#endif // BULLET_H
