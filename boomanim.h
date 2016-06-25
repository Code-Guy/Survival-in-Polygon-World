#ifndef BOOMANIM_H
#define BOOMANIM_H

#include <QPixmap>
#include <QPainter>
#include "constants.h"

class BoomAnim
{
public:
    BoomAnim(Pos2D pos, BoomEffectType boomEffectType);

    void Draw(QPainter *painter);
    void Logic();
    bool IsPlayEnd() const;

private:
    QPixmap imgBoom;
    QList<QPixmap> imgBoomFrames;
    Pos2D pos;
    BoomEffectType boomEffectType;
    int currentFrameIndex;//当前动画帧下标
    int totalFrame;//总帧数
    int frameW, frameH;//每帧的宽高
    bool isPlayEnd;//是否播放完毕
    int frameTimer;//动画帧之间的时间间隔

    void GetBoomFrames();//从一张图片中得到爆炸效果的帧
};

#endif // BOOMANIM_H
