#ifndef ITEM_H
#define ITEM_H

#include <QPixmap>
#include <QPainter>
#include "constants.h"

class Item
{
public:
    Item(ItemType itemType);

    void Draw(QPainter *painter);
    void Logic();
    bool IsCollisionWithPlayer(int playerPosX, int playerPosY, int playerImgW, int playerImgH);

    Pos2D GetPos() const;
    QPixmap GetImgItem() const;
    int GetImgW() const;
    int GetImgH() const;
    bool IsDead() const;
    void SetDead(bool flag);
    ItemType GetItemType() const;//得到道具的类型

private:
    Pos2D pos;
    Vector2D velocity;
    ItemType itemType;
    QPixmap imgItem;
    int imgW, imgH;
    bool isDead;
    double itemSpeed;

    int bounceCount;

    void GetRandomPosVel();//随机得到一个位置和速度

    bool IsOutOfLeftRightEdge() const;//是否在左右边界
    bool IsOutOfUpBottomEdge() const;//是否在上下边界

    void DetermineImgItem();
};

#endif // ITEM_H
