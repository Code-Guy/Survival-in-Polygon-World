#include "bullet.h"

Bullet::Bullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet)
    : velocity(velocity), pos(pos), imgBullet(imgBullet)
{
    isDead = false;
    //子弹的长和宽
    imgW = imgBullet.width();
    imgH = imgBullet.height();
    speed = velocity.GetLength();
}

bool Bullet::IsOutOfScreen()//是否超出屏幕范围
{
    return pos.x > Width || pos.x  + imgBullet.width()< 0 || pos.y > Height || pos.y + imgBullet.height()< 0;
}

bool Bullet::IsDead() const
{
    return isDead;
}

Pos2D Bullet::GetPos() const
{
    return pos;
}

Vector2D Bullet::GetVel() const
{
    return velocity;
}

QPixmap Bullet::GetImgBullet() const
{
    return imgBullet;
}

int Bullet::GetImgW() const
{
    return imgW;
}
int Bullet::GetImgH() const
{
    return imgH;
}

int Bullet::GetDamage() const
{
    return damage;
}

void Bullet::SetDead(bool flag)
{
    this->isDead = flag;
}

PlayerBullet::PlayerBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet)
    : Bullet(velocity, pos, imgBullet)
{

}

bool PlayerBullet::IsCollisionWithEnemy(int enemyPosX, int enemyPosY, int enemyImgW, int enemyImgH) const
{
    if(pos.x >= enemyPosX + enemyImgW ||
            pos.x + imgW <= enemyPosX ||
            pos.y >= enemyPosY + enemyImgH ||
            pos.y + imgH <= enemyPosY)
        return false;
    return true;
}

bool PlayerBullet::IsCollisionWithEnemyBullet(const EnemyBullet * const enemyBullet) const
{
    if(enemyBullet->IsUnDestroyed())//如果敌人的子弹是无敌的，则碰撞检测失败
        return false;

    int enemyBulletPosX = enemyBullet->GetPos().x;
    int enemyBulletPosY = enemyBullet->GetPos().y;
    int enemyBulletImgW = enemyBullet->GetImgW();
    int enemyBulletImgH = enemyBullet->GetImgH();

    if(pos.x >= enemyBulletPosX + enemyBulletImgW ||
            pos.x + imgW <= enemyBulletPosX ||
            pos.y >= enemyBulletPosY + enemyBulletImgH ||
            pos.y + imgH <= enemyBulletPosY)
        return false;
    return true;
}

void PlayerBullet::SetHp(int hp)
{
    this->hp = hp;
}

int PlayerBullet::GetHp() const
{
    return hp;
}

PurePlayerBullet::PurePlayerBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet)
    : PlayerBullet(velocity, pos, imgBullet)
{
    playerBulletType = PureBullet;
    damage = PureBulletDamage;
    hp = PureBulletHP;
}

void PurePlayerBullet::Draw(QPainter *painter)
{
    painter->drawPixmap(pos.x, pos.y, imgBullet);
}

void PurePlayerBullet::Logic()
{
    //处理边界
    if(IsOutOfScreen())
    {
        isDead = true;
        return;
    }
    pos += velocity;
}

BoomPlayerBullet::BoomPlayerBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet)
    : PlayerBullet(velocity, pos, imgBullet)
{
    playerBulletType = BoomBullet;
    damage = BoomBulletDamage;
    hp = BoomBulletHP;
}

void BoomPlayerBullet::Draw(QPainter *painter)
{
    painter->drawPixmap(pos.x, pos.y, imgBullet);
}

void BoomPlayerBullet::Logic()
{
    //处理边界
    if(IsOutOfScreen())
    {
        isDead = true;
        return;
    }
    pos += velocity;
}

WavePlayerBullet::WavePlayerBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet)
    : PlayerBullet(velocity, pos, imgBullet)
{
    playerBulletType = WaveBullet;
    damage = WaveBulletDamage;
    hp = WaveBulletHP;
}

void WavePlayerBullet::Draw(QPainter *painter)
{
    painter->drawPixmap(pos.x, pos.y, imgBullet);
}

void WavePlayerBullet::Logic()
{
    //处理边界
    if(IsOutOfScreen())
    {
        isDead = true;
        return;
    }
    pos += velocity;
}

EnemyBullet::EnemyBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet, bool isUnDestroyed, bool isTracking)
    : Bullet(velocity, pos, imgBullet), isUnDestroyed(isUnDestroyed), isTracking(isTracking)
{
    damage = EnemyBulletDamage;
}

void EnemyBullet::Draw(QPainter *painter)
{
    painter->drawPixmap(pos.x, pos.y, imgBullet);
    if(isUnDestroyed)
    {
        painter->fillRect(pos.x, pos.y, GetImgW(), GetImgH(), QBrush(QColor(125, 125, 125, 100)));
    }
}

bool EnemyBullet::IsCollisionWithPlayer(int playerPosX, int playerPosY, int playerImgW, int playerImgH)
{
    playerPos.x = playerPosX;
    playerPos.y = playerPosY;

    if(pos.x >= playerPosX + playerImgW ||
            pos.x + imgW <= playerPosX ||
            pos.y >= playerPosY + playerImgH ||
            pos.y + imgH <= playerPosY)
        return false;
    return true;
}

bool EnemyBullet::IsCollisionWithShield(int centerPosX, int centerPosY, int radius)
{
    return (pos.x - centerPosX)*(pos.x - centerPosX) + (pos.y - centerPosY)*(pos.y - centerPosY) < radius*radius;
}

EnemyBulletType EnemyBullet::GetEnemyBulletType()
{
    return enemyBulletType;
}

void EnemyBullet::Target()//重新定位，追逐主角
{
    Vector2D bulletToPlayer = playerPos - pos;
    bulletToPlayer.NormalizeSelf();
    velocity = bulletToPlayer * speed;
}

bool EnemyBullet::IsUnDestroyed() const
{
    return isUnDestroyed;
}

bool EnemyBullet::IsTracking() const
{
    return isTracking;
}

CommonEnemyBullet::CommonEnemyBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet, bool isUnDestroyed, bool isTracking)
    : EnemyBullet(velocity, pos, imgBullet, isUnDestroyed, isTracking)

{
    enemyBulletType = CommonBullet;
}

void CommonEnemyBullet::Logic()
{
    if(IsOutOfScreen())
    {
        isDead = true;
        return;
    }

    if(isTracking)
        Target();//重新定位，追逐主角

    pos += velocity;//更新子弹位置
}

BounceEnemyBullet::BounceEnemyBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet, bool isUnDestroyed, bool isTracking)
    : EnemyBullet(velocity, pos, imgBullet, isUnDestroyed, isTracking)
{
    bounceCount = 0;
    enemyBulletType = BounceBullet;
}

void BounceEnemyBullet::Logic()
{ 
    if(IsOutOfLeftRightEdge())
    {
        velocity.x *= -1;
        bounceCount++;
    }
    else if(IsOutOfUpBottomEdge())
    {
        velocity.y *= -1;
        bounceCount++;
    }

    if(bounceCount > BounceNum)
    {
        isDead = true;
        return;
    }

    if(isTracking)
        Target();//重新定位，追逐主角

    pos += velocity;//更新子弹位置
}

bool BounceEnemyBullet::IsOutOfLeftRightEdge() const//是否在左右边界
{
    return pos.x  + imgBullet.width()> Width || pos.x < 0;
}

bool BounceEnemyBullet::IsOutOfUpBottomEdge() const//是否在上下边界
{
    return pos.y  + imgBullet.height()> Height || pos.y < 0;
}

SplitEnemyBullet::SplitEnemyBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet, bool isUnDestroyed, bool isTracking,
                 int splitNum, int splitTime)
    : EnemyBullet(velocity, pos, imgBullet, isUnDestroyed, isTracking), splitNum(splitNum), splitTime(splitTime)
{
    isSplit = false;//是否处于分裂阶段
    splitTimer = 0;//分裂计时器
    enemyBulletType = SplitBullet;
}

void SplitEnemyBullet::Logic()
{
    if(IsOutOfScreen())
    {
        isDead = true;
        return;
    }

    if(!isDead)//在分裂总数范围内可以分裂
    {
        splitTimer++;
        if(splitTimer >= splitTime)
        {
            isSplit = true;
        }
    }

    if(isTracking)
        Target();//重新定位，追逐主角

    pos += velocity;//更新子弹位置
}

void SplitEnemyBullet::ResetSplitTimer()
{
    isSplit = false;//是否处于分裂阶段
    splitTimer = 0;//分裂计时器
}

bool SplitEnemyBullet::IsSpliting() const
{
    return isSplit;
}

void SplitEnemyBullet::SetSplitNum(int splitNum)//设置分裂的次数
{
    this->splitNum = splitNum;
}

int SplitEnemyBullet::GetSplitNum()//获取分裂次数
{
    return splitNum;
}

SBEnemyBullet::SBEnemyBullet(Vector2D velocity, Pos2D pos, QPixmap imgBullet, bool isUnDestroyed, bool isTracking,
                                      int splitNum, int splitTime)
    : EnemyBullet(velocity, pos, imgBullet, isUnDestroyed, isTracking), splitNum(splitNum), splitTime(splitTime)
{
    isSplit = false;//是否处于分裂阶段
    splitTimer = 0;//分裂计时器
    enemyBulletType = SBBullut;
}

void SBEnemyBullet::Logic()
{
    if(IsOutOfLeftRightEdge())
    {
        velocity.x *= -1;
        bounceCount++;
    }
    else if(IsOutOfUpBottomEdge())
    {
        velocity.y *= -1;
        bounceCount++;
    }

    if(bounceCount > BounceNum)
    {
        isDead = true;
        return;
    }

    if(!isDead)//在分裂总数范围内可以分裂
    {
        splitTimer++;
        if(splitTimer >= splitTime)
        {
            isSplit = true;
        }
    }

    if(isTracking)
        Target();//重新定位，追逐主角

    pos += velocity;//更新子弹位置
}

void SBEnemyBullet::ResetSplitTimer()
{
    isSplit = false;//是否处于分裂阶段
    splitTimer = 0;//分裂计时器
}

bool SBEnemyBullet::IsSpliting() const
{
    return isSplit;
}

void SBEnemyBullet::SetSplitNum(int splitNum)//设置分裂的次数
{
    this->splitNum = splitNum;
}

int SBEnemyBullet::GetSplitNum()//获取分裂次数
{
    return splitNum;
}


bool SBEnemyBullet::IsOutOfLeftRightEdge() const//是否在左右边界
{
    return pos.x + imgBullet.width()> Width || pos.x < 0;
}

bool SBEnemyBullet::IsOutOfUpBottomEdge() const//是否在上下边界
{
    return pos.y + imgBullet.height()> Height || pos.y < 0;
}
