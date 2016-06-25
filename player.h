#ifndef PLAYER_H
#define PLAYER_H

#include <QPixmap>
#include <QPainter>
#include <QKeyEvent>
#include "constants.h"
#include "bullet.h"

class Player
{
public:
    Player(PlayerType playerType);
    ~Player();

    void Draw(QPainter *painter);
    void Logic();
    void HandlePressKey(QKeyEvent *event);
    void HandleReleaseKey(QKeyEvent *event);

    void SetHp(int hp);
    int GetHp();

    void SetBulletSpeed(int bulletSpeed);
    int GetBulletSpeed();

    void SetIsCollision(bool isCollision);
    bool GetIsCollision() const;

    void SetGrade(int grade);
    int GetGrade();

    bool IsDead() const;
    void SetDead(bool flag);

    bool IsSpeedUp() const;
    void SetSpeedUp(bool flag);

    bool IsShield() const;
    void SetShield(bool flag);

    bool IsAddLines() const;
    void SetAddLines(bool flag);

    bool IsWaveBullet() const;
    void SetWaveBullet(bool flag);

    Pos2D GetPos() const;
    Pos2D GetCenterPos() const;
    int GetShieldRadius() const;
    int GetImgW() const;
    int GetImgH() const;

    void SetBoomMode(bool flag);

    QList<PlayerBullet *> playerBullets;//主角的子弹容器
    bool IsCollisionWithEnemy(int enemyPosX, int enemyPosY, int enemyImgW, int enemyImgH);

private:
    PlayerType playerType;//飞机的类型
    int hp;//血量
    int grade;
    QPixmap imgHp;//血量标志图（心）
    QPixmap imgPlayer;
    QPixmap imgPlayerAvatar;
    QPixmap imgBullet;
    QPixmap imgBoom;
    QPixmap imgWaveBullet;
    QPixmap imgShield;

    int bulletSpeed;//子弹速度
    int boomSpeed;//导弹速度

    //各种道具计数器
    int speedUpTimer;
    int shieldTimer;
    int addLinesTimer;

    bool isSpeedUp;
    bool isShield;
    bool isAddLines;
    bool isWaveBullet;

    Pos2D pos;//位置
    Pos2D centerPos;//玩家的中心位置
    double speed;//自身的移动速度
    bool isUp, isDown, isLeft, isRight;//方向标志位

    int shieldRadius;//保护圈的半径

    int noCollisionCount;
    bool isCollision;

    int imgW, imgH;//敌机图片的长和宽
    bool isDead;//是否死亡

    int commonBulletCount;//发射普通子弹的计时器
    int commonBulletTime;//发射普通子弹的时间间隔
    bool isGenCommonBullet;//是否产生普通子弹

    int boomBulletCount;//发射导弹的计时器
    int boomBulletTime;//发射导弹的时间间隔
    bool isGenBoomBullet;//是否产生导弹
    bool isBoomMode;

    void GenBullets();//生成子弹
    void RemoveBullets();//清除需要被删除的子弹
};

#endif // PLAYER_H
