#include "boomanim.h"

BoomAnim::BoomAnim(Pos2D pos, BoomEffectType boomEffectType)
    : pos(pos), boomEffectType(boomEffectType)
{
    isPlayEnd = false;
    currentFrameIndex = 0;
    frameTimer = 0;

    switch(boomEffectType)
    {
    case SimpleBoomEffect:
        imgBoom = QPixmap(":/images/boom_effect.png");
        totalFrame = SimpleBoomEffectTotalFrame;
        break;
    case BossBoomEffect:
        imgBoom = QPixmap(":/images/bossboom_effect.png");
        totalFrame = BossBoomEffectTotalFrame;
        break;
    }

    frameW = imgBoom.width() / totalFrame;
    frameH = imgBoom.height();
    GetBoomFrames();//从一张图片中得到爆炸效果的帧
}

void BoomAnim::Draw(QPainter *painter)
{
    painter->drawPixmap(pos.x, pos.y, imgBoomFrames[currentFrameIndex]);
}

void BoomAnim::Logic()
{
    frameTimer++;
    if(frameTimer >= BoomEffectFrameTime)
    {
        if(currentFrameIndex < totalFrame - 1)
        {
            currentFrameIndex++;
        }
        else
        {
            isPlayEnd = true;
            currentFrameIndex = 0;
            return;
        }
        frameTimer = 0;
    }
}

void BoomAnim::GetBoomFrames()//从一张图片中得到爆炸效果的帧
{
    for(int i=0; i<totalFrame; i++)
    {
        imgBoomFrames.append(imgBoom.copy(i*frameW, 0, frameW, frameH));
    }
}

bool BoomAnim::IsPlayEnd() const
{
    return isPlayEnd;
}
