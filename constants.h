#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <qmath.h>

enum EnemyBulletType{CommonBullet, BounceBullet, SplitBullet, SBBullut};
enum PlayerBulletType{PureBullet, BoomBullet, WaveBullet};

enum PlayerType{Plane1, Plane2, Plane3};

enum EnemyShapeType{Polygon3, Polygon4, Polygon5, Polygon6, Polygon7,
                   Polygon8, Polygon9, Polygon10, Polygon11, Polygon12};
enum EnemySplitMode{Split, NoSplit};
enum EnemyBounceMode{Bounce, NoBounce};
enum EnemyRotateMode{Rotate, NoRotate};
enum EnemyMoveMode{Circle, Square, Horizontal, Vertical, Sine, NoMove};
enum EnemyGenBulletMode{EveryEdge, LoopEdge};
enum EnemyTrackMode{Track, NoTrack};
enum EnemyDestroyedMode{CanBeDestroyed, UnDestroyed};

enum BoomEffectType{SimpleBoomEffect, BossBoomEffect};

enum ItemType{FirstAid, Boomer, SpeedUp, Shield, AddLines, Disappear, Wave};

const int SceneNum = 5;
const double PI = 3.1416;
const int Width = 800;
const int Height = 600;
const double PlayerScale = 0.5;
const double PlayerAvatarScale = 0.25;
const int NoCollisionTime = 60;
const int HPSpan = 10;
const int DefaultHP = 5;
const int BounceNum = 1;
const int ItemBounceNum = 3;
const int DefaultSplitTime = 20;
const int DefaultSplitNum = 1;
const double EnemyBulletRatio = 0.4;
const int LoopEdgeTime = 10;

const double Plane1Speed = 5;
const double Plane2Speed = 5.5;
const double Plane3Speed = 6;

const int DefaultRotateTime = 8;
const int DefaultBulletTime = 60;
const int DefaultCommonBulletTime = 15;//默认玩家的普通子弹发射速度
const int SpeedUpCommonBulletTime = 10;//加速状态下玩家的普通子弹发射速度
const int DefaultBoomBulletTime = 30;//默认玩家的导弹发射速度
const int DefaultCommonBulletSpeed = 5;
const int DefaultBoomBulletSpeed = 6;

const int SpeedUpTime = 1000;//子弹加速时间
const int ShieldTime = 600;//盾牌时间
const int AddLinesTime = 1000;//盾牌时间

const int PureBulletDamage = 1, BoomBulletDamage = 3, WaveBulletDamage = 100;
const int PureBulletHP = 1, BoomBulletHP = 3, WaveBulletHP = 100;
const int EnemyBulletDamage = 1;

const int SplitSubRotateAngle = 35;
const int DefaultRotateAngle = 2;
const int CircleRadius = 25;
const int SquareSideLen = 50;
const int EnemyMoveFrame = 100;
const int HorEnemyMoveFrame = 500;
const int SimpleBoomEffectTotalFrame = 7;
const int BossBoomEffectTotalFrame = 5;
const int BoomEffectFrameTime = 2;
const double ItemSpeeds[] = {5, 6, 7, 7.5, 6, 6, 6};

const int SceneTitlePrevMoveTime = 50;
const int SceneTitleStandTime = 50;
const int SceneTitleNextMoveTime = 75;

const int SceneTitleRectW = 200;
const int SceneTitleRectH = 50;

const int ItemNum = 7;
const int GenItemTime = 100;//出现一个道具的时间
const double RoughItemRatios[] = {4, 3.5, 3, 3, 3, 2, 4};//每种道具出现的概率（大概的概率）

struct Vector2D
{
    double x;
    double y;

    Vector2D(double x = 0, double y = 0) : x(x), y(y)
    {}

    void operator +=(const Vector2D &rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
    }

    Vector2D operator +(const Vector2D &rhs)
    {
        return Vector2D(this->x+rhs.x, this->y+rhs.y);
    }

    Vector2D operator -(const Vector2D &rhs)
    {
        return Vector2D(this->x-rhs.x, this->y-rhs.y);
    }

    Vector2D operator *(const double rhs)
    {
        return Vector2D(rhs*this->x, rhs*this->y);
    }

    void operator *=(const double rhs)
    {
        this->x *= rhs;
        this->y *= rhs;
    }

    Vector2D Normalize()
    {
        if(x == 0 && y == 0)
            return (*this);
        double len = qSqrt(GetSquareSum());
        return Vector2D(this->x/len, this->y/len);
    }

    void NormalizeSelf()
    {
        if(x == 0 && y == 0)
            return;
        double len = qSqrt(GetSquareSum());
        x /= len;
        y /= len;
    }

    double GetSquareSum()
    {
        return x*x + y*y;
    }

    Vector2D Rotate(double angle)
    {
        double degree = angle * PI / 180.0;
        return Vector2D(this->x*qCos(degree) - this->y*qSin(degree),
                    this->x*qSin(degree) + this->y*qCos(degree));
    }

    double DistanceTo(Vector2D &rhs)
    {
        return qSqrt((rhs.x-this->x)*(rhs.x-this->x) + (rhs.y-this->y)*(rhs.y-this->y));
    }

    double GetLength()
    {
        return qSqrt(x*x + y*y);
    }
};

typedef Vector2D Pos2D;

struct EnemyParams
{
    Pos2D pos;
    EnemyShapeType enemyShapeType;
    EnemySplitMode enemySplitMode;
    EnemyBounceMode enemyBounceMode;
    EnemyGenBulletMode enemyGenBulletMode;
    EnemyRotateMode enemyRotateMode;
    EnemyMoveMode enemyMoveMode;
    EnemyTrackMode enemyTrackMode;
    EnemyDestroyedMode enemyDestroyedMode;

    EnemyParams(Pos2D pos,
                EnemyShapeType enemyShapeType,
                EnemySplitMode enemySplitMode = NoSplit,
                EnemyBounceMode enemyBounceMode = NoBounce,
                EnemyGenBulletMode enemyGenBulletMode = EveryEdge,
                EnemyRotateMode enemyRotateMode = NoRotate,
                EnemyMoveMode enemyMoveMode = NoMove,
                EnemyTrackMode enemyTrackMode = NoTrack,
                EnemyDestroyedMode enemyDestroyedMode = CanBeDestroyed)
        : pos(pos), enemyShapeType(enemyShapeType), enemySplitMode(enemySplitMode),
          enemyBounceMode(enemyBounceMode), enemyGenBulletMode(enemyGenBulletMode),
          enemyRotateMode(enemyRotateMode), enemyMoveMode(enemyMoveMode),
          enemyTrackMode(enemyTrackMode), enemyDestroyedMode(enemyDestroyedMode)
    {}
};

const Pos2D PlayerAvatarPos(20, 15);
const Pos2D HPPos(65, 15);
const Pos2D GradePos(625, 15);
const Pos2D GradeWH(200, 50);

#endif // CONSTANTS_H
