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
    int currentFrameIndex;//��ǰ����֡�±�
    int totalFrame;//��֡��
    int frameW, frameH;//ÿ֡�Ŀ��
    bool isPlayEnd;//�Ƿ񲥷����
    int frameTimer;//����֮֡���ʱ����

    void GetBoomFrames();//��һ��ͼƬ�еõ���ըЧ����֡
};

#endif // BOOMANIM_H
