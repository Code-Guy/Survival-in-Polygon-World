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
    int sceneTitleTimer;//�������⶯����ʱ��
    double sceneTitleStep;//����������˶�����
    bool isSceneTitleAnim;//�Ƿ񲥷����
};

#endif // SCENETITLEANIM_H
