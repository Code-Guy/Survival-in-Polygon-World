#include "scenetitleanim.h"

SceneTitleAnim::SceneTitleAnim(int whichScene)
{
    title = QString("Scene %1").arg(whichScene);

    isSceneTitleAnim = true;
    sceneTitleTimer = 0;
    sceneTitleStep = Width / (2.0*SceneTitlePrevMoveTime);
}

void SceneTitleAnim::Draw(QPainter *painter)
{
    QPen pen(Qt::black);
    QBrush brush(Qt::black);
    QFont font("Consolas", 30, QFont::Bold);

    painter->save();

    painter->setPen(pen);
    painter->setBrush(brush);
    painter->setFont(font);

    painter->drawText(QRectF(pos.x - SceneTitleRectW/2.0, pos.y, SceneTitleRectW, SceneTitleRectH), title);

    painter->restore();
}

void SceneTitleAnim::Logic()
{
    sceneTitleTimer++;
    if(sceneTitleTimer <= SceneTitlePrevMoveTime)
    {
        pos.x = sceneTitleTimer*sceneTitleStep;
        pos.y = Height/2;
    }
    else if(sceneTitleTimer <= SceneTitlePrevMoveTime + SceneTitleStandTime)
    {
        pos.x = SceneTitlePrevMoveTime*sceneTitleStep;
        pos.y = Height/2;
    }
    else if(sceneTitleTimer <= SceneTitlePrevMoveTime + SceneTitleNextMoveTime + SceneTitleStandTime)
    {
        pos.x = (sceneTitleTimer-SceneTitleStandTime)*sceneTitleStep;
        pos.y = Height/2;
    }
    else
    {
        isSceneTitleAnim = false;
    }
}

bool SceneTitleAnim::IsPlayEnd() const
{
    return !isSceneTitleAnim;
}

void SceneTitleAnim::Play()
{
    isSceneTitleAnim = true;
    sceneTitleTimer = 0;
}

void SceneTitleAnim::SetSceneIndex(int whichScene)
{
    title = QString("Scene %1").arg(whichScene);
}
