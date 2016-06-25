#include "player.h"
#include <QPen>

Player::Player(PlayerType playerType)
{
    imgHp = QPixmap(":/images/blood.png");//血量标志图（心）
    this->playerType = playerType;
    hp = DefaultHP;
    grade = 0;

    bulletSpeed = DefaultCommonBulletSpeed;//子弹速度
    boomSpeed = DefaultBoomBulletSpeed;//导弹速度

    //各种道具计数器
    speedUpTimer = 0;
    shieldTimer = 0;
    addLinesTimer = 0;

    isSpeedUp = false;
    isShield = false;
    isAddLines = false;
    isWaveBullet = false;

    QPixmap tmpImgPlayer;
    switch(playerType)
    {
    case Plane1:
        tmpImgPlayer = QPixmap(":/images/player1.png");
        imgPlayer = tmpImgPlayer.scaled(PlayerScale * tmpImgPlayer.size());
        imgPlayerAvatar = tmpImgPlayer.scaled(PlayerAvatarScale * tmpImgPlayer.size());
        speed = Plane1Speed;
        break;
    case Plane2:
        tmpImgPlayer = QPixmap(":/images/player2.png");
        imgPlayer = tmpImgPlayer.scaled(PlayerScale * tmpImgPlayer.size());
        imgPlayerAvatar = tmpImgPlayer.scaled(PlayerAvatarScale * tmpImgPlayer.size());
        speed = Plane2Speed;
        break;
    case Plane3:
        tmpImgPlayer = QPixmap(":/images/player3.png");
        imgPlayer = tmpImgPlayer.scaled(PlayerScale * tmpImgPlayer.size());
        imgPlayerAvatar = tmpImgPlayer.scaled(PlayerAvatarScale * tmpImgPlayer.size());
        speed = Plane3Speed;
        break;
    }

    pos.x = (Width - imgPlayer.width()) / 2;
    pos.y = Height - imgPlayer.height() - 50;//位置

    //敌机图片的长和宽
    imgW = imgPlayer.width();
    imgH = imgPlayer.height();

    //方向标志位
    isUp = false;
    isDown = false;
    isLeft = false;
    isRight = false;

    isDead = false;
    isBoomMode = false;

    noCollisionCount = 0;
    isCollision = false;

    centerPos = Vector2D(pos.x+imgW/2, pos.y+imgH/2);

    imgBullet = QPixmap(":/images/bullet.png");
    imgBoom = QPixmap(":/images/boom.png");
    imgWaveBullet = QPixmap(":/images/wave_bullet.png");
    imgShield = QPixmap(":/images/ring.png");
    shieldRadius = imgShield.width() / 2;

    commonBulletCount = 0;//发射普通子弹的计时器
    commonBulletTime = DefaultCommonBulletTime;//发射普通子弹的时间间隔
    isGenCommonBullet = false;//是否产生普通子弹

    boomBulletCount = 0;//发射导弹的计时器
    boomBulletTime = DefaultBoomBulletTime;//发射导弹的时间间隔
    isGenBoomBullet = false;//是否产生导弹
}

Player::~Player()
{

}

void Player::Draw(QPainter *painter)
{
    //画主角
    if(isCollision)
    {
        if(noCollisionCount % 2 == 0)
            painter->drawPixmap(pos.x, pos.y, imgPlayer);
    }
    else
    {
        painter->drawPixmap(pos.x, pos.y, imgPlayer);
    }

    if(isShield)
    {
        painter->drawPixmap(centerPos.x-imgShield.width()/2, centerPos.y-imgShield.height()/2, imgShield);
    }

    //画主角子弹
    for(int i=0; i<playerBullets.size(); i++)
    {
        playerBullets[i]->Draw(painter);
    }
    //画主角缩略图及血量
    painter->drawPixmap(PlayerAvatarPos.x, PlayerAvatarPos.y, imgPlayerAvatar);
    for(int i=0; i<hp; i++)
    {
        painter->drawPixmap(HPPos.x + i*(HPSpan+imgHp.width()), HPPos.y, imgHp);
    }

    QPen pen(Qt::black);
    QBrush brush(Qt::black);
    QFont font("Consolas", 20);
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->setFont(font);
    //绘制分数
    painter->drawText(QRect(GradePos.x, GradePos.y, GradeWH.x, GradeWH.y), QString("Grade: %1").arg(grade));

    pen.setColor(Qt::white);
    brush.setColor(Qt::white);
    painter->setPen(pen);
    painter->setBrush(brush);
}

void Player::Logic()
{
    if(isCollision)
    {
        noCollisionCount++;
        if(noCollisionCount >= NoCollisionTime)
        {
            isCollision = false;
            noCollisionCount = 0;
        }
    }

    if(isSpeedUp)
    {
        commonBulletTime = SpeedUpCommonBulletTime;
        speedUpTimer++;
        if(speedUpTimer >= SpeedUpTime)
        {
            isSpeedUp = false;
            speedUpTimer = 0;
            commonBulletTime = DefaultCommonBulletTime;
        }
    }

    if(isShield)
    {
        shieldTimer++;
        if(shieldTimer >= ShieldTime)
        {
            isShield = false;
            shieldTimer = 0;
        }
    }

    if(isAddLines)
    {
        addLinesTimer++;
        if(addLinesTimer >= AddLinesTime)
        {
            isAddLines = false;
            addLinesTimer = 0;
        }
    }

    RemoveBullets();//清除需要被删除的子弹
    GenBullets();//生成子弹
    //处理主角子弹的逻辑
    for(int i=0; i<playerBullets.size(); i++)
    {
        playerBullets[i]->Logic();
    }

    if(isUp)
        pos.y -= speed;
    if(isLeft)
        pos.x -= speed;
    if(isDown)
        pos.y += speed;
    if(isRight)
        pos.x += speed;

    if(pos.x + imgPlayer.width() >= Width)
        pos.x = Width - imgPlayer.width();
    else if(pos.x <= 0)
        pos.x = 0;

    if(pos.y + imgPlayer.height() >= Height)
        pos.y = Height - imgPlayer.height();
    else if(pos.y <= 0)
        pos.y = 0;

    centerPos = Vector2D(pos.x+imgW/2, pos.y+imgH/2);
}

Pos2D Player::GetPos() const
{
    return pos;
}

Pos2D Player::GetCenterPos() const
{
    return centerPos;
}

int Player::GetShieldRadius() const
{
    return shieldRadius;
}

int Player::GetImgW() const
{
    return imgW;
}
int Player::GetImgH() const
{
    return imgH;
}

void Player::SetBoomMode(bool flag)
{
    this->isBoomMode = flag;
}

bool Player::IsCollisionWithEnemy(int enemyPosX, int enemyPosY, int enemyImgW, int enemyImgH)
{
    if(isShield)
        return false;

    if(!isCollision)
    {
        if(pos.x >= enemyPosX + enemyImgW ||
                pos.x + imgW <= enemyPosX ||
                pos.y >= enemyPosY + enemyImgH ||
                pos.y + imgH <= enemyPosY)
        {
            return false;
            isCollision = true;
            isBoomMode = false;
        }
        return true;
    }
    else
    {
        return false;
    }
}

void Player::GenBullets()//生成子弹
{
    if(isWaveBullet)
    {
        playerBullets.append(new WavePlayerBullet(Vector2D(0, -bulletSpeed),
                                                  Pos2D(0, pos.y),
                                                  imgWaveBullet));
        isWaveBullet = false;
    }
    else
    {
        commonBulletCount++;
        if(commonBulletCount >= commonBulletTime)
        {
            isGenCommonBullet = true;
        }

        if(isGenCommonBullet)
        {
            if(isAddLines)
            {
                playerBullets.append(new PurePlayerBullet(Vector2D(0, -bulletSpeed),
                                                          Pos2D(pos.x+imgPlayer.width()/4-imgBullet.width()/2, pos.y),
                                                          imgBullet));
                playerBullets.append(new PurePlayerBullet(Vector2D(0, -bulletSpeed),
                                                          Pos2D(pos.x+3*imgPlayer.width()/4-imgBullet.width()/2, pos.y),
                                                          imgBullet));
            }
            playerBullets.append(new PurePlayerBullet(Vector2D(0, -bulletSpeed),
                                                      Pos2D(pos.x+imgPlayer.width()/2-imgBullet.width()/2, pos.y),
                                                      imgBullet));
            isGenCommonBullet = false;
            commonBulletCount = 0;
        }

        if(isBoomMode)
        {
            boomBulletCount++;
            if(boomBulletCount >= boomBulletTime)
            {
                isGenBoomBullet = true;
            }

            if(isGenBoomBullet)
            {
                playerBullets.append(new BoomPlayerBullet(Vector2D(0, -boomSpeed),
                                                          Pos2D(pos.x-imgBoom.width(), pos.y+imgPlayer.height()-imgBoom.height()),
                                                          imgBoom));
                playerBullets.append(new BoomPlayerBullet(Vector2D(0, -boomSpeed),
                                                          Pos2D(pos.x+imgPlayer.width(), pos.y+imgPlayer.height()-imgBoom.height()),
                                                          imgBoom));
                isGenBoomBullet = false;
                boomBulletCount = 0;
            }
        }
    }
}

void Player::RemoveBullets()//清除需要被删除的子弹
{
    for(int i=0; i<playerBullets.size(); i++)
    {
        if(playerBullets[i]->IsDead())
            playerBullets.removeAt(i);
    }
}

void Player::HandlePressKey(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
        isUp = true;
    if(event->key() == Qt::Key_A)
        isLeft = true;
    if(event->key() == Qt::Key_S)
        isDown = true;
    if(event->key() == Qt::Key_D)
        isRight = true;
}

void Player::HandleReleaseKey(QKeyEvent *event)
{
    if(event->key() == Qt::Key_W)
        isUp = false;
    if(event->key() == Qt::Key_A)
        isLeft = false;
    if(event->key() == Qt::Key_S)
        isDown = false;
    if(event->key() == Qt::Key_D)
        isRight = false;
}

void Player::SetHp(int hp)
{
    this->hp = hp;
}

int Player::GetHp()
{
    return this->hp;
}

void Player::SetBulletSpeed(int bulletSpeed)
{
    this->bulletSpeed = bulletSpeed;
}

int Player::GetBulletSpeed()
{
    return bulletSpeed;
}

void Player::SetIsCollision(bool isCollision)
{
    this->isCollision = isCollision;
}

bool Player::GetIsCollision() const
{
    return isCollision;
}

void Player::SetGrade(int grade)
{
    this->grade = grade;
}

int Player::GetGrade()
{
    return grade;
}

bool Player::IsDead() const
{
    return isDead;
}

void Player::SetDead(bool flag)
{
    this->isDead = flag;
}

bool Player::IsSpeedUp() const
{
    return isSpeedUp;
}

void Player::SetSpeedUp(bool flag)
{
    this->isSpeedUp = flag;
}

bool Player::IsShield() const
{
    return isShield;
}

bool Player::IsAddLines() const
{
    return isAddLines;
}

void Player::SetAddLines(bool flag)
{
    this->isAddLines = flag;
}

bool Player::IsWaveBullet() const
{
    return isWaveBullet;
}

void Player::SetWaveBullet(bool flag)
{
    this->isWaveBullet = flag;
}

void Player::SetShield(bool flag)
{
    this->isShield = flag;
}
