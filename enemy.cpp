#include "enemy.h"

Enemy::Enemy(EnemyParams enemyParams)
    : pos(enemyParams.pos), enemyShapeType(enemyParams.enemyShapeType), enemySplitMode(enemyParams.enemySplitMode),
      enemyBounceMode(enemyParams.enemyBounceMode), enemyGenBulletMode(enemyParams.enemyGenBulletMode),
      enemyRotateMode(enemyParams.enemyRotateMode), enemyMoveMode(enemyParams.enemyMoveMode),
      enemyTrackMode(enemyParams.enemyTrackMode), enemyDestroyedMode(enemyParams.enemyDestroyedMode)
{
    bulletTimer = 0;//�����ӵ��ļ�ʱ��
    bulletTime = DefaultBulletTime;//�����ӵ���ʱ����
    isGenBullet = false;//�Ƿ�����ӵ�
    isDead = false;

    SwitchEnemyImg(enemyShapeType, enemySplitMode, enemyBounceMode);//���ݵл������жϵл�������
    imgBullet = imgEnemy.scaled(EnemyBulletRatio * imgEnemy.size());//�ӵ���״���ǵл�����״

    hp = polygonIndex;
    metaAngle = 2.0*PI / polygonIndex;//�õ���ת��
    metaAngleDegree = 360.0 / polygonIndex;
    //�л�ͼƬ�ĳ��Ϳ�
    imgW = imgEnemy.width();
    imgH = imgEnemy.height();

    centerPos = Pos2D(pos.x+imgW/2, pos.y+imgH/2);//���˵�����λ��
    originPos = pos;

    speed = 4;
    //����ѭ������ļ�ʱ��
    loopEdgeTimer = 0;
    loopEdgeTime = LoopEdgeTime;
    loopEdgeIndex = 0;

    //��ת�ļ�ʱ��
    rotateAngle = 0;//��ת��
    circleCenter = Vector2D(pos.x-CircleRadius, pos.y);
    moveFrameCount = 0;

    switch(enemyTrackMode)
    {
    case Track:
        isTrack = true;
        break;
    case NoTrack:
        isTrack = false;
        break;
    }

    switch(enemyDestroyedMode)
    {
    case UnDestroyed:
        isUndestroyed = true;
        break;
    case CanBeDestroyed:
        isUndestroyed = false;
        break;
    }
}

void Enemy::Draw(QPainter *painter)
{
    if(enemyRotateMode == Rotate)
    {
        painter->save();
        painter->translate(centerPos.x, centerPos.y);
        painter->rotate(-rotateAngle);
        painter->translate(-centerPos.x, -centerPos.y);
        painter->drawPixmap(pos.x, pos.y, imgEnemy);
        painter->restore();
    }
    else
    {
        painter->drawPixmap(pos.x, pos.y, imgEnemy);
    }
}

void Enemy::Logic(QList<EnemyBullet *> &enemyBullets)
{
    if(enemyMoveMode != NoMove)
        UpdateEnemyPos();//���µл���λ��

    if(enemyRotateMode == Rotate)
    {
        rotateAngle += DefaultRotateAngle;
        if(rotateAngle >= 360.0)
            rotateAngle = 0;
    }

    GenBullets(enemyBullets);//�����ӵ�
}

void Enemy::SetHp(int hp)
{
    this->hp = hp;
}

int Enemy::GetHp()
{
    return this->hp;
}

bool Enemy::IsDead() const
{
    return isDead;
}

void Enemy::SetDead(bool flag)
{
    this->isDead = flag;
}

Pos2D Enemy::GetPos() const
{
    return pos;
}

int Enemy::GetImgW() const
{
    return imgW;
}
int Enemy::GetImgH() const
{
    return imgH;
}

void Enemy::SwitchEnemyImg(EnemyShapeType enemyShapeType,
                           EnemySplitMode enemySplitMode,
                           EnemyBounceMode enemyBounceMode)//���ݵл������жϵл�������
{
    switch(enemyShapeType)
    {
    case Polygon3:
        DetermineEnemyImg(enemySplitMode, enemyBounceMode, 3);
        polygonIndex = 3;
        break;
    case Polygon4:
        DetermineEnemyImg(enemySplitMode, enemyBounceMode, 4);
        polygonIndex = 4;
        break;
    case Polygon5:
        DetermineEnemyImg(enemySplitMode, enemyBounceMode, 5);
        polygonIndex = 5;
        break;
    case Polygon6:
        DetermineEnemyImg(enemySplitMode, enemyBounceMode, 6);
        polygonIndex = 6;
        break;
    case Polygon7:
        DetermineEnemyImg(enemySplitMode, enemyBounceMode, 7);
        polygonIndex = 7;
        break;
    case Polygon8:
        DetermineEnemyImg(enemySplitMode, enemyBounceMode, 8);
        polygonIndex = 8;
        break;
    case Polygon9:
        DetermineEnemyImg(enemySplitMode, enemyBounceMode, 9);
        polygonIndex = 9;
        break;
    case Polygon10:
        DetermineEnemyImg(enemySplitMode, enemyBounceMode, 10);
        polygonIndex = 10;
        break;
    case Polygon11:
        DetermineEnemyImg(enemySplitMode, enemyBounceMode, 11);
        polygonIndex = 11;
        break;
    case Polygon12:
        DetermineEnemyImg(enemySplitMode, enemyBounceMode, 12);
        polygonIndex = 12;
        break;
    }
}

void Enemy::DetermineEnemyImg(EnemySplitMode enemySplitMode,
                              EnemyBounceMode enemyBounceMode,
                              int index)
{
    if(enemySplitMode == Split)
    {
        if(enemyBounceMode == Bounce)
        {
            imgEnemy = QPixmap(QString(":/images/polygon_%1_sb.png").arg(index));
            award = 4;
        }
        else
        {
            imgEnemy = QPixmap(QString(":/images/polygon_%1_split.png").arg(index));
            award = 3;
        }
    }
    else
    {
        if(enemyBounceMode == Bounce)
        {
            imgEnemy = QPixmap(QString(":/images/polygon_%1_bounce.png").arg(index));
            award = 2;
        }
        else
        {
            imgEnemy = QPixmap(QString(":/images/polygon_%1.png").arg(index));
            award = 1;
        }
    }
    award *= index;
}

void Enemy::GenBullets(QList<EnemyBullet *> &enemyBullets)//�����ӵ�
{
    if(enemyGenBulletMode == EveryEdge)
    {
        bulletTimer++;
        if(bulletTimer >= bulletTime)
        {
            isGenBullet = true;
        }

        if(isGenBullet)
        {
            GetBullets(enemyBullets);

            isGenBullet = false;
            bulletTimer = 0;
        }
    }
    else if(enemyGenBulletMode == LoopEdge)
    {
        loopEdgeTimer++;
        if(loopEdgeTimer >= loopEdgeTime)
        {
            isGenBullet = true;
        }
        if(isGenBullet)
        {
            GetBullets(enemyBullets);
            isGenBullet = false;
            loopEdgeTimer = 0;
        }
    }
}

QList<VelPos> Enemy::GetShootVelPos()//�õ������ӵ���λ��
{
    centerPos = Pos2D(pos.x+imgW/2, pos.y+imgH/2);//���˵�����λ��
    double rod = centerPos.DistanceTo(pos);//�õ���ĳ���

    QList<VelPos> shootVelPos;

    if(enemyGenBulletMode == EveryEdge)
    {
        for(int i=0; i<polygonIndex; i++)
        {
            shootVelPos.append(VelPos(Vector2D(speed*qCos(metaAngle*i+rotateAngle*PI/180.0), speed*qSin(metaAngle*i+rotateAngle*PI/180.0)),
                                      Pos2D(centerPos.x+rod*qCos(metaAngle*i+rotateAngle*PI/180.0)-imgBullet.width()/2, centerPos.y+rod*qSin(metaAngle*i+rotateAngle*PI/180.0)-imgBullet.height()/2)));
        }
    }
    else if(enemyGenBulletMode == LoopEdge)
    {
        shootVelPos.append(VelPos(Vector2D(speed*qCos(metaAngle*loopEdgeIndex+rotateAngle*PI/180.0), speed*qSin(metaAngle*loopEdgeIndex+rotateAngle*PI/180.0)),
                                  Pos2D(centerPos.x+rod*qCos(metaAngle*loopEdgeIndex+rotateAngle*PI/180.0)-imgBullet.width()/2, centerPos.y+rod*qSin(metaAngle*loopEdgeIndex+rotateAngle*PI/180.0)-imgBullet.height()/2)));
        if(loopEdgeIndex < polygonIndex - 1)
            loopEdgeIndex++;
        else
            loopEdgeIndex = 0;
    }

    return shootVelPos;
}

void Enemy::GetBullets(QList<EnemyBullet *> &enemyBullets)
{
    QList<VelPos> shootVelPos = GetShootVelPos();
    for(int i=0; i<shootVelPos.size(); i++)
    {
        if(enemySplitMode == Split && enemyBounceMode == Bounce)
        {
            enemyBullets.append(new SBEnemyBullet(shootVelPos[i].vel,
                                                  shootVelPos[i].pos,
                                                  imgBullet,
                                                  isUndestroyed,
                                                  isTrack));
        }
        else if(enemySplitMode == Split)
        {
            enemyBullets.append(new SplitEnemyBullet(shootVelPos[i].vel,
                                                     shootVelPos[i].pos,
                                                     imgBullet,
                                                     isUndestroyed,
                                                     isTrack));
        }
        else if(enemyBounceMode == Bounce)
        {
            enemyBullets.append(new BounceEnemyBullet(shootVelPos[i].vel,
                                                      shootVelPos[i].pos,
                                                      imgBullet,
                                                      isUndestroyed,
                                                      isTrack));
        }
        else
        {
            enemyBullets.append(new CommonEnemyBullet(shootVelPos[i].vel,
                                                      shootVelPos[i].pos,
                                                      imgBullet,
                                                      isUndestroyed,
                                                      isTrack));
        }
    }
}

void Enemy::UpdateEnemyPos()//���µл���λ��
{
    switch(enemyMoveMode)
    {
    case Circle:
    {
        moveFrameCount++;
        if(moveFrameCount <= EnemyMoveFrame)
        {
            double angle = 2.0*PI*moveFrameCount / EnemyMoveFrame;
            pos = Vector2D(circleCenter.x+CircleRadius*qCos(angle),
                           circleCenter.y+CircleRadius*qSin(angle));
        }
        else
        {
            moveFrameCount = 0;
        }
        break;
    }
    case Square:
    {
        double step = 4.0 * SquareSideLen / EnemyMoveFrame;
        moveFrameCount++;
        if(moveFrameCount <= EnemyMoveFrame/4)
        {
            pos.x -= step;
        }
        else if(moveFrameCount <= EnemyMoveFrame/2)
        {
            pos.y -= step;
        }
        else if(moveFrameCount <= 3*EnemyMoveFrame/4)
        {
            pos.x += step;
        }
        else if(moveFrameCount <= EnemyMoveFrame)
        {
            pos.y += step;
        }
        else
        {
            moveFrameCount = 0;
        }
        break;
    }
    case Horizontal:
    {
        double step = 2.0*Width / HorEnemyMoveFrame;
        moveFrameCount++;
        if(moveFrameCount <= (1-1.0*originPos.x/Width)*HorEnemyMoveFrame/2.0)
        {
            pos.x += step;
        }
        else if(moveFrameCount <= (2-1.0*originPos.x/Width)*HorEnemyMoveFrame/2.0)
        {
            pos.x -= step;
        }
        else if(moveFrameCount <= HorEnemyMoveFrame)
        {
            pos.x += step;
        }
        else
        {
            moveFrameCount = 0;
        }
        break;
    }
    case Vertical:
    {
        double step = 2.0*Height / EnemyMoveFrame;
        moveFrameCount++;
        if(moveFrameCount <= (1-1.0*pos.y/Height)*EnemyMoveFrame/2.0)
        {
            pos.y += step;
        }
        else if(moveFrameCount <= (2-1.0*pos.y/Height)*EnemyMoveFrame/2.0)
        {
            pos.y -= step;
        }
        else if(moveFrameCount <= EnemyMoveFrame)
        {
            pos.y += step;
        }
        else
        {
            moveFrameCount = 0;
        }
        break;
    }
    case Sine:
        break;
    case NoMove:
        break;
    }
    ClampEnemyPos();//��ֹ�л���λ�ó�����Ļ
}

bool Enemy::IsOutOfLeftRightEdge() const//�Ƿ������ұ߽�
{
    return pos.x + imgEnemy.width()> Width || pos.x < 0;
}

bool Enemy::IsOutOfUpBottomEdge() const//�Ƿ������±߽�
{
    return pos.y + imgEnemy.height()> Height || pos.y < 0;
}

void Enemy::ClampEnemyPos()//��ֹ�л���λ�ó�����Ļ
{
    centerPos = Pos2D(pos.x+imgW/2, pos.y+imgH/2);//���˵�����λ��

    if(pos.x < 0)
        pos.x = 0;
    else if(pos.x + imgEnemy.width() > Width)
        pos.x = Width - imgEnemy.width();

    if(pos.y < 0)
        pos.y = 0;
    else if(pos.y + imgEnemy.height() > Height)
        pos.y = Height - imgEnemy.height();
}

int Enemy::GetAward() const
{
    return award;
}

EnemyShapeType Enemy::GetShapeType() const
{
    return enemyShapeType;
}
