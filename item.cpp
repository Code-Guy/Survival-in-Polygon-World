#include "item.h"

Item::Item(ItemType itemType) : itemType(itemType)
{
    DetermineImgItem();

    imgW = imgItem.width();
    imgH = imgItem.height();

    GetRandomPosVel();//����õ�һ��λ�ú��ٶ�
    isDead = false;
    bounceCount = 0;
}

void Item::Draw(QPainter *painter)
{
    painter->drawPixmap(pos.x, pos.y, imgItem);
}

void Item::Logic()
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

    if(bounceCount > ItemBounceNum)
    {
        isDead = true;
        return;
    }

    pos += velocity;//�����ӵ�λ��
}

bool Item::IsCollisionWithPlayer(int playerPosX, int playerPosY, int playerImgW, int playerImgH)
{
    if(pos.x >= playerPosX + playerImgW ||
            pos.x + imgW <= playerPosX ||
            pos.y >= playerPosY + playerImgH ||
            pos.y + imgH <= playerPosY)
        return false;
    return true;
}

Pos2D Item::GetPos() const
{
    return pos;
}

QPixmap Item::GetImgItem() const
{
    return imgItem;
}

int Item::GetImgW() const
{
    return imgW;
}

int Item::GetImgH() const
{
    return imgH;
}

void Item::DetermineImgItem()
{
    switch(itemType)
    {
    case FirstAid:
        imgItem = QPixmap(":/images/firstaid.png");
        itemSpeed = ItemSpeeds[0];
        break;
    case Boomer:
        imgItem = QPixmap(":/images/boomer.png");
        itemSpeed = ItemSpeeds[1];
        break;
    case SpeedUp:
        imgItem = QPixmap(":/images/speedup.png");
        itemSpeed = ItemSpeeds[2];
        break;
    case Shield:
        imgItem = QPixmap(":/images/shield.png");
        itemSpeed = ItemSpeeds[3];
        break;
    case AddLines:
        imgItem = QPixmap(":/images/lines.png");
        itemSpeed = ItemSpeeds[4];
        break;
    case Disappear:
        imgItem = QPixmap(":/images/disappear.png");
        itemSpeed = ItemSpeeds[5];
        break;
    case Wave:
        imgItem = QPixmap(":/images/wave.png");
        itemSpeed = ItemSpeeds[6];
        break;
    }
}

bool Item::IsOutOfLeftRightEdge() const//�Ƿ������ұ߽�
{
    return pos.x  + imgItem.width()> Width || pos.x < 0;
}

bool Item::IsOutOfUpBottomEdge() const//�Ƿ������±߽�
{
    return pos.y  + imgItem.height()> Height || pos.y < 0;
}

bool Item::IsDead() const
{
    return isDead;
}

void Item::SetDead(bool flag)
{
    this->isDead = flag;
}

ItemType Item::GetItemType() const//�õ����ߵ�����
{
    return itemType;
}

void Item::GetRandomPosVel()//����õ�һ��λ�ú��ٶ�
{
    //�����λ�ã�y�̶�Ϊ1�� x�����1��width-1֮��
    pos.x = qrand() % (Width-2*imgItem.width()-1) + imgItem.width();
    pos.y = 50;

    velocity = Vector2D(itemSpeed, 0);
    double angle;//��ת�ĽǶ�
    //������ٶ�
    if(pos.x <= Width / 2.0)
    {
        angle = 30 + qrand() % 60;
    }
    else
    {
        angle = 90 + qrand() % 60;
    }
    velocity = velocity.Rotate(angle);
}
