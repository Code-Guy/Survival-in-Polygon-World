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

    QList<PlayerBullet *> playerBullets;//���ǵ��ӵ�����
    bool IsCollisionWithEnemy(int enemyPosX, int enemyPosY, int enemyImgW, int enemyImgH);

private:
    PlayerType playerType;//�ɻ�������
    int hp;//Ѫ��
    int grade;
    QPixmap imgHp;//Ѫ����־ͼ���ģ�
    QPixmap imgPlayer;
    QPixmap imgPlayerAvatar;
    QPixmap imgBullet;
    QPixmap imgBoom;
    QPixmap imgWaveBullet;
    QPixmap imgShield;

    int bulletSpeed;//�ӵ��ٶ�
    int boomSpeed;//�����ٶ�

    //���ֵ��߼�����
    int speedUpTimer;
    int shieldTimer;
    int addLinesTimer;

    bool isSpeedUp;
    bool isShield;
    bool isAddLines;
    bool isWaveBullet;

    Pos2D pos;//λ��
    Pos2D centerPos;//��ҵ�����λ��
    double speed;//������ƶ��ٶ�
    bool isUp, isDown, isLeft, isRight;//�����־λ

    int shieldRadius;//����Ȧ�İ뾶

    int noCollisionCount;
    bool isCollision;

    int imgW, imgH;//�л�ͼƬ�ĳ��Ϳ�
    bool isDead;//�Ƿ�����

    int commonBulletCount;//������ͨ�ӵ��ļ�ʱ��
    int commonBulletTime;//������ͨ�ӵ���ʱ����
    bool isGenCommonBullet;//�Ƿ������ͨ�ӵ�

    int boomBulletCount;//���䵼���ļ�ʱ��
    int boomBulletTime;//���䵼����ʱ����
    bool isGenBoomBullet;//�Ƿ��������
    bool isBoomMode;

    void GenBullets();//�����ӵ�
    void RemoveBullets();//�����Ҫ��ɾ�����ӵ�
};

#endif // PLAYER_H
