#ifndef SCENETITLEANIM_H
#define SCENETITLEANIM_H

#include <QPixmap>
#include <QPainter>
#include "constants.h"

class SceneTitleAnim
{
public:
    SceneTitleAnim(int whichScene);

    void Draw(QPainter *painter);
    void Logic();
    bool IsPlayEnd() const;
    void Play();
    void SetSceneIndex(int whichScene);

private:
    QString title;
    Pos2D pos;
    int sceneTitleTimer;//开场标题动画计时器
    double sceneTitleStep;//开场标题的运动步长
    bool isSceneTitleAnim;//是否播放完毕
};

#endif // SCENETITLEANIM_H
