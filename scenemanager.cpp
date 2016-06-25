#include "scenemanager.h"
#include "globals.h"
#include <QDebug>

QList< QList<EnemyParams> > EnemyClusters[SceneNum];
double ItemRatios[ItemNum];

SceneManager::SceneManager(int whichScene, int whichPlane,
                           Phonon::MediaObject *boom, Phonon::MediaObject *getItemAudio,
                           QObject *parent)
    : QObject(parent), whichScene(whichScene-1), whichPlane(whichPlane), boom(boom), getItemAudio(getItemAudio)
{
    InitEnemyCluster();//初始化敌机群
    InitScene();//初始化场景
}

void SceneManager::Draw(QPainter *painter)
{
    player->Draw(painter);

    for(int i=0; i<enemies.size(); i++)
    {
        enemies[i].Draw(painter);
    }

    for(int i=0; i<enemyBullets.size(); i++)
    {
        enemyBullets[i]->Draw(painter);
    }

    for(int i=0; i<boomAnims.size(); i++)
    {
        boomAnims[i].Draw(painter);
    }

    for(int i=0; i<items.size(); i++)
    {
        items[i].Draw(painter);
    }
}

void SceneManager::Logic()
{
    if(boom->state() != Phonon::PlayingState && boom->state() != Phonon::StoppedState)
    {
        boom->stop();
    }

    if(getItemAudio->state() != Phonon::PlayingState && getItemAudio->state() != Phonon::StoppedState)
    {
        getItemAudio->stop();
    }

    GenEnemy();//创建新的敌机
    GenItem();//生成道具
    HandleSpecialBullets();//处理一些特殊的子弹（可分裂的）
    HandleCollisionDetect();//处理碰撞检测
    HandleRemoveStuff();//处理所有物体（子弹、敌人的删除）
    HandlePosUpdate();//处理所有物体（子弹、敌人的移动）
}

void SceneManager::HandlePosUpdate()//处理所有物体（子弹、敌人的移动）
{
    player->Logic();

    for(int i=0; i<enemies.size(); i++)
    {
        enemies[i].Logic(enemyBullets);
    }

    for(int i=0; i<enemyBullets.size(); i++)
    {
        enemyBullets[i]->Logic();
    }

    for(int i=0; i<boomAnims.size(); i++)
    {
        boomAnims[i].Logic();
    }

    for(int i=0; i<items.size(); i++)
    {
        items[i].Logic();
    }
}

void SceneManager::HandleCollisionDetect()//处理碰撞检测
{
    //判断敌人子弹与主角的碰撞
    for(int i=0; i<enemyBullets.size(); i++)
    {
        if(player->IsShield())
        {
            if(enemyBullets[i]->IsCollisionWithShield(player->GetCenterPos().x, player->GetCenterPos().y, player->GetShieldRadius()))
                enemyBullets[i]->SetDead(true);//敌人的子弹和主角碰撞后立即消亡，防止多次碰撞
        }
        else
        {
            if(!player->GetIsCollision() &&
                    enemyBullets[i]->IsCollisionWithPlayer(player->GetPos().x, player->GetPos().y, player->GetImgW(), player->GetImgH()))
            {
                enemyBullets[i]->SetDead(true);//敌人的子弹和主角碰撞后立即消亡，防止多次碰撞
                if(player->GetHp() > 0)
                {
                    player->SetHp(player->GetHp()-1);
                }
                else
                {
                    player->SetDead(true);
                    emit ShowLoseDialog();
                    return;
                }

                player->SetIsCollision(true);
                player->SetBoomMode(false);
            }
        }
    }
    //判断主角的子弹和敌人的子弹的碰撞(需要分别遍历主角的子弹和敌人的子弹)
    for(int i=0; i<player->playerBullets.size(); i++)
    {
        for(int j=0; j<enemyBullets.size(); j++)
        {
            if(!enemyBullets[j]->IsDead() && player->playerBullets[i]->IsCollisionWithEnemyBullet(enemyBullets[j]))
            {
                player->playerBullets[i]->SetHp(player->playerBullets[i]->GetHp()-1);
                if(player->playerBullets[i]->GetHp() <= 0)
                    player->playerBullets[i]->SetDead(true);

                enemyBullets[j]->SetDead(true);
            }
        }
    }

    //判断主角与敌人的碰撞
    if(!player->IsShield())
    {
        for(int i=0; i<enemies.size(); i++)
        {
            Enemy &enemy = enemies[i];
            if(player->IsCollisionWithEnemy(enemy.GetPos().x, enemy.GetPos().y, enemy.GetImgW(), enemy.GetImgH()))
            {
                if(enemy.GetHp() > 0)
                    enemy.SetHp(enemy.GetHp()-1);
                else
                {
                    boom->play();
                    enemy.SetDead(true);
                    boomAnims.append(BoomAnim(enemy.GetPos(), SimpleBoomEffect));
                }

                if(player->GetHp() > 0)
                    player->SetHp(player->GetHp()-1);
                else
                {
                    player->SetDead(true);
                    emit ShowLoseDialog();
                    return;
                }
            }
        }
    }
    //判断主角子弹与敌人的碰撞(需要分别遍历主角的子弹和敌人)
    for(int i=0; i<player->playerBullets.size(); i++)
    {
        for(int j=0; j<enemies.size(); j++)
        {
            Enemy &enemy = enemies[j];
            if(!enemy.IsDead() &&
                    player->playerBullets[i]->IsCollisionWithEnemy(enemy.GetPos().x, enemy.GetPos().y, enemy.GetImgW(), enemy.GetImgH()))
            {
                enemy.SetHp(enemy.GetHp()-player->playerBullets[i]->GetDamage());

                if(enemy.GetHp() <= 0)
                {
                    enemy.SetDead(true);
                    boomAnims.append(BoomAnim(enemy.GetPos(), SimpleBoomEffect));
                    boom->play();
                }

                player->playerBullets[i]->SetDead(true);
            }
        }
    }
    //判断主角与道具的碰撞
    for(int i=0; i<items.size(); i++)
    {
        if(items[i].IsCollisionWithPlayer(player->GetPos().x, player->GetPos().y, player->GetImgW(), player->GetImgH()))
        {
            getItemAudio->play();
            HandleItem(items[i].GetItemType());
            items[i].SetDead(true);
        }
    }
}

void SceneManager::HandleRemoveStuff()//处理所有物体（子弹、敌人的删除）
{
    for(int i=0; i<enemyBullets.size(); i++)
    {
        if(enemyBullets[i]->IsDead())
            enemyBullets.removeAt(i);
    }
    //删除已经消亡的敌人
    for(int i=0; i<enemies.size(); i++)
    {
        if(enemies[i].IsDead())
        {
            UpdateEnemyStats(enemies[i].GetShapeType());//更新数据统计
            player->SetGrade(player->GetGrade()+enemies[i].GetAward());
            enemies.removeAt(i);
            deadEnemyCount++;
        }
    }

    for(int i=0; i<boomAnims.size(); i++)
    {
        if(boomAnims[i].IsPlayEnd())
            boomAnims.removeAt(i);
    }

    for(int i=0; i<items.size(); i++)
    {
        if(items[i].IsDead())
        {
            UpdateItemStats(items[i].GetItemType());//更新数据统计
            items.removeAt(i);
        }
    }
}

void SceneManager::HandleSpecialBullets()//处理一些特殊的子弹（可分裂的）
{
    QList<EnemyBullet *> newSplitEnemyBullets;
    for(int i=0; i<enemyBullets.size(); i++)
    {
        if(enemyBullets[i]->GetEnemyBulletType() == SplitBullet)//如果子弹要分裂
        {
            SplitEnemyBullet *splitEnemyBullet = (SplitEnemyBullet *)enemyBullets[i];
            if(splitEnemyBullet->GetSplitNum() <= 0)
            {
                return;
            }
            else if(splitEnemyBullet->IsSpliting())
            {
                //先消亡自己
                splitEnemyBullet->SetDead(true);
                splitEnemyBullet->ResetSplitTimer();

                //生成三个新的可以分裂的子弹
                double splitRod = splitEnemyBullet->GetImgH() + splitEnemyBullet->GetImgW();
                Vector2D vel = splitEnemyBullet->GetVel();
                Pos2D pos = splitEnemyBullet->GetPos();
                Vector2D normalizedVel = vel.Normalize();//速度归一化
                QPixmap imgBullet = splitEnemyBullet->GetImgBullet();
                Pos2D midBulletPos = pos + normalizedVel*splitRod;//中间点的位置
                Vector2D midBulletVel = vel;

                if(!IsOutOfScreen(midBulletPos, imgBullet.width(), imgBullet.height()))//如果中间点的位置没有超出屏幕并且该子弹还可以再分裂
                {
                    Pos2D firstBulletPos = (normalizedVel*splitRod).Rotate(SplitSubRotateAngle) + pos;
                    Pos2D thirdBulletPos = (normalizedVel*splitRod).Rotate(-SplitSubRotateAngle) + pos;

                    Vector2D firstBulletVel = midBulletVel.Rotate(SplitSubRotateAngle);
                    Vector2D thirdBulletVel = midBulletVel.Rotate(-SplitSubRotateAngle);

                    newSplitEnemyBullets.append(new SplitEnemyBullet(firstBulletVel,
                                                                     firstBulletPos,
                                                                     splitEnemyBullet->GetImgBullet(),
                                                                     false, false,
                                                                     splitEnemyBullet->GetSplitNum()-1));
                    newSplitEnemyBullets.append(new SplitEnemyBullet(midBulletVel,
                                                                     midBulletPos,
                                                                     splitEnemyBullet->GetImgBullet(),
                                                                     false, false,
                                                                     splitEnemyBullet->GetSplitNum()-1));
                    newSplitEnemyBullets.append(new SplitEnemyBullet(thirdBulletVel,
                                                                     thirdBulletPos,
                                                                     splitEnemyBullet->GetImgBullet(),
                                                                     false, false,
                                                                     splitEnemyBullet->GetSplitNum()-1));
                }
            }
        }
        else if(enemyBullets[i]->GetEnemyBulletType() == SBBullut)//如果子弹要分裂
        {
            SBEnemyBullet *splitEnemyBullet = (SBEnemyBullet *)enemyBullets[i];
            if(splitEnemyBullet->GetSplitNum() <= 0)
            {
                return;
            }
            else if(splitEnemyBullet->IsSpliting())
            {
                //先消亡自己
                splitEnemyBullet->SetDead(true);
                splitEnemyBullet->ResetSplitTimer();

                //生成三个新的可以分裂的子弹
                double splitRod = splitEnemyBullet->GetImgW();
                Vector2D vel = splitEnemyBullet->GetVel();
                Pos2D pos = splitEnemyBullet->GetPos();
                Vector2D normalizedVel = vel.Normalize();//速度归一化
                QPixmap imgBullet = splitEnemyBullet->GetImgBullet();
                Pos2D midBulletPos = pos + normalizedVel*splitRod;//中间点的位置
                Vector2D midBulletVel = vel;

                if(!IsOutOfScreen(midBulletPos, imgBullet.width(), imgBullet.height()) &&
                        splitEnemyBullet->GetSplitNum() > 0)//如果中间点的位置超出屏幕
                {
                    Pos2D firstBulletPos = (normalizedVel*splitRod).Rotate(SplitSubRotateAngle) + pos;
                    Pos2D thirdBulletPos = (normalizedVel*splitRod).Rotate(-SplitSubRotateAngle) + pos;

                    Vector2D firstBulletVel = midBulletVel.Rotate(SplitSubRotateAngle);
                    Vector2D thirdBulletVel = midBulletVel.Rotate(-SplitSubRotateAngle);

                    newSplitEnemyBullets.append(new SBEnemyBullet(firstBulletVel,
                                                                  firstBulletPos,
                                                                  splitEnemyBullet->GetImgBullet(),
                                                                  false, false,
                                                                  splitEnemyBullet->GetSplitNum()-1));
                    newSplitEnemyBullets.append(new SBEnemyBullet(midBulletVel,
                                                                  midBulletPos,
                                                                  splitEnemyBullet->GetImgBullet(),
                                                                  false, false,
                                                                  splitEnemyBullet->GetSplitNum()-1));
                    newSplitEnemyBullets.append(new SBEnemyBullet(thirdBulletVel,
                                                                  thirdBulletPos,
                                                                  splitEnemyBullet->GetImgBullet(),
                                                                  false, false,
                                                                  splitEnemyBullet->GetSplitNum()-1));
                }
            }
        }
    }

    for(int i=0; i<newSplitEnemyBullets.size(); i++)
    {
        enemyBullets.append(newSplitEnemyBullets[i]);
    }
}

bool SceneManager::IsOutOfScreen(Pos2D pos, int imgW, int imgH)
{
    return pos.x + imgW > Width || pos.x < 0 || pos.y + imgH > Height || pos.y < 0;
}

void SceneManager::HandlePressKey(QKeyEvent *event)
{
    player->HandlePressKey(event);
}

void SceneManager::HandleReleaseKey(QKeyEvent *event)
{
    player->HandleReleaseKey(event);
}

void SceneManager::InitScene()//初始化场景
{
    deadEnemyCount = 0;//已经消亡的敌机的个数
    genEnemiesCount = 0;//已经生成了第几波敌机
    genItemTimer = 0;

    switch(whichPlane)
    {
    case 1:
        player = new Player(Plane1);
        break;
    case 2:
        player = new Player(Plane2);
        break;
    case 3:
        player = new Player(Plane3);
        break;
    }

    //先加入第一波敌机
    for(int i=0; i<EnemyClusters[whichScene][genEnemiesCount].size(); i++)
        enemies.append(Enemy(EnemyClusters[whichScene][genEnemiesCount][i]));
}

void SceneManager::GenEnemy()//创建新的敌机
{
    if(deadEnemyCount >= EnemyClusters[whichScene][genEnemiesCount].size() && boomAnims.isEmpty())//保证爆炸结束
    {
        genEnemiesCount++;//下一波
        if(genEnemiesCount > EnemyClusters[whichScene].size()-1)
        {
            genEnemiesCount = 0;//每一关的敌人波数
            deadEnemyCount = 0;//每一波敌人的死亡数
            genItemTimer = 0;//生成道具的计数器

            if(whichScene < SceneNum -1)
            {
                winSceneIndex++;
                emit ShowNextDialog();
            }
            else
            {
                winSceneIndex++;
                emit ShowWinDialog();
            }
        }
        else
        {
            deadEnemyCount = 0;

            for(int i=0; i<EnemyClusters[whichScene][genEnemiesCount].size(); i++)
                enemies.append(Enemy(EnemyClusters[whichScene][genEnemiesCount][i]));
        }
    }
}

void SceneManager::Restart()
{
    if(player != NULL)
    {
        delete player;
        player = NULL;
    }

    enemies.clear();
    enemyBullets.clear();//注意，只有类的指针才有多态性
    boomAnims.clear();
    items.clear();

    InitScene();
}

int SceneManager::GetWhichScene() const
{
    return whichScene;
}

void SceneManager::SetWhichScene(int whichScene)
{
    this->whichScene = whichScene;
}

void SceneManager::GenItem()//生成道具
{
    genItemTimer++;
    if(genItemTimer > GenItemTime)
    {
        genItemTimer = 0;//reset

        int random = qrand() % 200;//产生0-200（不包含200）的随机数
        if(random > 100)
            return;
        else
        {
            if(random < ItemRatios[0])
                items.append(Item(FirstAid));
            else if(random < ItemRatios[1])
                items.append(Item(Boomer));
            else if(random < ItemRatios[2])
                items.append(Item(SpeedUp));
            else if(random < ItemRatios[3])
                items.append(Item(Shield));
            else if(random < ItemRatios[4])
                items.append(Item(AddLines));
            else if(random < ItemRatios[5])
                items.append(Item(Disappear));
            else if(random < ItemRatios[6])
                items.append(Item(Wave));
        }
    }
}

void SceneManager::HandleItem(ItemType itemType)
{
    switch(itemType)
    {
    case FirstAid:
        if(player->GetHp() < DefaultHP)
            player->SetHp(player->GetHp()+1);
        break;
    case Boomer:
        player->SetBoomMode(true);//发射导弹
        break;
    case SpeedUp:
        player->SetSpeedUp(true);//子弹更加密集
        break;
    case Shield:
        player->SetShield(true);
        break;
    case AddLines:
        player->SetAddLines(true);
        break;
    case Disappear:
        enemyBullets.clear();
        break;
    case Wave:
        player->SetWaveBullet(true);
        break;
    }
}

//更新敌人和道具的统计数据
void SceneManager::UpdateEnemyStats(EnemyShapeType shape)
{
    switch(shape)
    {
    case Polygon3:
        enemyStats[0]++;
        break;
    case Polygon4:
        enemyStats[1]++;
        break;
    case Polygon5:
        enemyStats[2]++;
        break;
    case Polygon6:
        enemyStats[3]++;
        break;
    case Polygon7:
        enemyStats[4]++;
        break;
    case Polygon8:
        enemyStats[5]++;
        break;
    case Polygon9:
        enemyStats[6]++;
        break;
    case Polygon10:
        enemyStats[7]++;
        break;
    case Polygon11:
        enemyStats[8]++;
        break;
    case Polygon12:
        enemyStats[9]++;
        break;
    }
}

void SceneManager::UpdateItemStats(ItemType itemType)
{
    switch(itemType)
    {
    case FirstAid:
        itemStats[0]++;
        break;
    case Boomer:
        itemStats[1]++;
        break;
    case SpeedUp:
        itemStats[2]++;
        break;
    case Shield:
        itemStats[3]++;
        break;
    case AddLines:
        itemStats[4]++;
        break;
    case Disappear:
        itemStats[5]++;
        break;
    case Wave:
        itemStats[6]++;
        break;
    }
}

void InitEnemyCluster()//初始化敌机群
{
    //第一关
    QList<EnemyParams> enemyCluster01;
    enemyCluster01.append(EnemyParams(Pos2D(200, 100), Polygon3));
    enemyCluster01.append(EnemyParams(Pos2D(600, 100), Polygon3));
    enemyCluster01.append(EnemyParams(Pos2D(400, 250), Polygon3));

    QList<EnemyParams> enemyCluster02;
    enemyCluster02.append(EnemyParams(Pos2D(160, 85), Polygon4));
    enemyCluster02.append(EnemyParams(Pos2D(640, 85), Polygon4));
    enemyCluster02.append(EnemyParams(Pos2D(160, 300), Polygon4));
    enemyCluster02.append(EnemyParams(Pos2D(640, 300), Polygon4));

    QList<EnemyParams> enemyCluster03;
    enemyCluster03.append(EnemyParams(Pos2D(250, 80), Polygon5));
    enemyCluster03.append(EnemyParams(Pos2D(550, 80), Polygon5));
    enemyCluster03.append(EnemyParams(Pos2D(125, 220), Polygon5));
    enemyCluster03.append(EnemyParams(Pos2D(675, 220), Polygon5));
    enemyCluster03.append(EnemyParams(Pos2D(400, 350), Polygon5));

    EnemyClusters[0].append(enemyCluster01);
    EnemyClusters[0].append(enemyCluster02);
    EnemyClusters[0].append(enemyCluster03);

    //第二关
    QList<EnemyParams> enemyCluster11;
    enemyCluster11.append(EnemyParams(Pos2D(160, 85), Polygon4, NoSplit, Bounce));
    enemyCluster11.append(EnemyParams(Pos2D(640, 85), Polygon4, NoSplit, Bounce));
    enemyCluster11.append(EnemyParams(Pos2D(160, 300), Polygon4, NoSplit, Bounce));
    enemyCluster11.append(EnemyParams(Pos2D(640, 300), Polygon4, NoSplit, Bounce));

    QList<EnemyParams> enemyCluster12;
    enemyCluster12.append(EnemyParams(Pos2D(250, 80), Polygon8, NoSplit, NoBounce, EveryEdge, Rotate));
    enemyCluster12.append(EnemyParams(Pos2D(550, 80), Polygon8, NoSplit, NoBounce, EveryEdge, Rotate));
    enemyCluster12.append(EnemyParams(Pos2D(125, 220), Polygon8, NoSplit, NoBounce, EveryEdge, Rotate));
    enemyCluster12.append(EnemyParams(Pos2D(675, 220), Polygon8, NoSplit, NoBounce, EveryEdge, Rotate));
    enemyCluster12.append(EnemyParams(Pos2D(400, 350), Polygon8, NoSplit, NoBounce, EveryEdge, Rotate));

    QList<EnemyParams> enemyCluster13;
    enemyCluster13.append(EnemyParams(Pos2D(148, 58), Polygon5, NoSplit, NoBounce, EveryEdge, Rotate));
    enemyCluster13.append(EnemyParams(Pos2D(362, 58), Polygon5, NoSplit, NoBounce, EveryEdge, Rotate));
    enemyCluster13.append(EnemyParams(Pos2D(570, 58), Polygon5, NoSplit, NoBounce, EveryEdge, Rotate));
    enemyCluster13.append(EnemyParams(Pos2D(242, 160), Polygon5, NoSplit, NoBounce, EveryEdge, Rotate));
    enemyCluster13.append(EnemyParams(Pos2D(459, 160), Polygon5, NoSplit, NoBounce, EveryEdge, Rotate));
    enemyCluster13.append(EnemyParams(Pos2D(362, 256), Polygon5, NoSplit, NoBounce, EveryEdge, Rotate));

    QList<EnemyParams> enemyCluster14;
    enemyCluster14.append(EnemyParams(Pos2D(180, 180), Polygon10, NoSplit, NoBounce, EveryEdge, Rotate, Circle));
    enemyCluster14.append(EnemyParams(Pos2D(540, 180), Polygon10, NoSplit, NoBounce, EveryEdge, Rotate, Square));

    EnemyClusters[1].append(enemyCluster11);
    EnemyClusters[1].append(enemyCluster12);
    EnemyClusters[1].append(enemyCluster13);
    EnemyClusters[1].append(enemyCluster14);

    //第三关
    QList<EnemyParams> enemyCluster21;
    enemyCluster21.append(EnemyParams(Pos2D(200, 100), Polygon3, Split));
    enemyCluster21.append(EnemyParams(Pos2D(600, 100), Polygon3, Split));
    enemyCluster21.append(EnemyParams(Pos2D(400, 250), Polygon3, Split));

    QList<EnemyParams> enemyCluster22;
    enemyCluster22.append(EnemyParams(Pos2D(160, 85), Polygon6, NoSplit, Bounce));
    enemyCluster22.append(EnemyParams(Pos2D(640, 85), Polygon6, NoSplit, Bounce));
    enemyCluster22.append(EnemyParams(Pos2D(160, 300), Polygon6, NoSplit, Bounce));
    enemyCluster22.append(EnemyParams(Pos2D(640, 300), Polygon6, NoSplit, Bounce));

    QList<EnemyParams> enemyCluster23;
    enemyCluster23.append(EnemyParams(Pos2D(167, 295), Polygon9, NoSplit, Bounce, LoopEdge, Rotate));
    enemyCluster23.append(EnemyParams(Pos2D(360, 60), Polygon9, NoSplit, Bounce, LoopEdge, Rotate));
    enemyCluster23.append(EnemyParams(Pos2D(555, 295), Polygon9, NoSplit, Bounce, LoopEdge, Rotate));

    QList<EnemyParams> enemyCluster24;
    enemyCluster24.append(EnemyParams(Pos2D(167, 295), Polygon9, NoSplit, Bounce, LoopEdge, Rotate));
    enemyCluster24.append(EnemyParams(Pos2D(360, 60), Polygon9, NoSplit, Bounce, LoopEdge, Rotate));
    enemyCluster24.append(EnemyParams(Pos2D(555, 295), Polygon9, NoSplit, Bounce, LoopEdge, Rotate));

    QList<EnemyParams> enemyCluster25;
    enemyCluster25.append(EnemyParams(Pos2D(360, 175), Polygon7, Split, Bounce, EveryEdge, NoRotate, Horizontal));

    EnemyClusters[2].append(enemyCluster21);
    EnemyClusters[2].append(enemyCluster22);
    EnemyClusters[2].append(enemyCluster23);
    EnemyClusters[2].append(enemyCluster24);
    EnemyClusters[2].append(enemyCluster25);

    //第四关
    QList<EnemyParams> enemyCluster31;
    enemyCluster31.append(EnemyParams(Pos2D(160, 85), Polygon5, NoSplit, NoBounce, EveryEdge, Rotate, NoMove, Track));
    enemyCluster31.append(EnemyParams(Pos2D(640, 85), Polygon5, NoSplit, NoBounce, EveryEdge, Rotate, NoMove, Track));
    enemyCluster31.append(EnemyParams(Pos2D(160, 300), Polygon5, NoSplit, NoBounce, EveryEdge, Rotate, NoMove, Track));
    enemyCluster31.append(EnemyParams(Pos2D(640, 300), Polygon5, NoSplit, NoBounce, EveryEdge, Rotate, NoMove, Track));

    QList<EnemyParams> enemyCluster32;
    enemyCluster32.append(EnemyParams(Pos2D(200, 100), Polygon3, NoSplit, Bounce, EveryEdge, Rotate, NoMove, Track));
    enemyCluster32.append(EnemyParams(Pos2D(600, 100), Polygon3, NoSplit, Bounce, EveryEdge, Rotate, NoMove, Track));
    enemyCluster32.append(EnemyParams(Pos2D(400, 250), Polygon3, NoSplit, Bounce, EveryEdge, Rotate, NoMove, Track));

    QList<EnemyParams> enemyCluster33;
    enemyCluster33.append(EnemyParams(Pos2D(240, 63), Polygon6, NoSplit, Bounce, EveryEdge, Rotate, Circle));
    enemyCluster33.append(EnemyParams(Pos2D(480, 63), Polygon6, NoSplit, Bounce, EveryEdge, Rotate, Circle));
    enemyCluster33.append(EnemyParams(Pos2D(240, 280), Polygon6, NoSplit, Bounce, EveryEdge, Rotate, Circle));
    enemyCluster33.append(EnemyParams(Pos2D(480, 280), Polygon6, NoSplit, Bounce, EveryEdge, Rotate, Circle));

    QList<EnemyParams> enemyCluster34;
    enemyCluster34.append(EnemyParams(Pos2D(350, 60), Polygon8, Split, Bounce, EveryEdge, Rotate));
    enemyCluster34.append(EnemyParams(Pos2D(350, 280), Polygon4, Split, Bounce, EveryEdge, Rotate));

    QList<EnemyParams> enemyCluster35;
    enemyCluster35.append(EnemyParams(Pos2D(185, 160), Polygon10, Split, NoBounce, EveryEdge, Rotate, Vertical));
    enemyCluster35.append(EnemyParams(Pos2D(550, 160), Polygon5, Split, NoBounce, EveryEdge, Rotate, Vertical));

    QList<EnemyParams> enemyCluster36;
    enemyCluster36.append(EnemyParams(Pos2D(350, 130), Polygon11, NoSplit, Bounce, EveryEdge, Rotate));

    EnemyClusters[3].append(enemyCluster31);
    EnemyClusters[3].append(enemyCluster32);
    EnemyClusters[3].append(enemyCluster33);
    EnemyClusters[3].append(enemyCluster34);
    EnemyClusters[3].append(enemyCluster35);
    EnemyClusters[3].append(enemyCluster36);

    //第五关
    QList<EnemyParams> enemyCluster41;
    enemyCluster41.append(EnemyParams(Pos2D(200, 100), Polygon5, NoSplit, Bounce));
    enemyCluster41.append(EnemyParams(Pos2D(600, 100), Polygon6, Split, Bounce));
    enemyCluster41.append(EnemyParams(Pos2D(400, 250), Polygon7, Split, Bounce));

    QList<EnemyParams> enemyCluster42;
    enemyCluster42.append(EnemyParams(Pos2D(185, 165), Polygon3, NoSplit, Bounce));
    enemyCluster42.append(EnemyParams(Pos2D(350, 25), Polygon4, Split, Bounce));
    enemyCluster42.append(EnemyParams(Pos2D(350, 310), Polygon5, Split, NoBounce));
    enemyCluster42.append(EnemyParams(Pos2D(548, 165), Polygon6, Split, Bounce));

    QList<EnemyParams> enemyCluster43;
    enemyCluster43.append(EnemyParams(Pos2D(85, 165), Polygon3, NoSplit, Bounce));
    enemyCluster43.append(EnemyParams(Pos2D(255, 165), Polygon4, Split, Bounce));
    enemyCluster43.append(EnemyParams(Pos2D(430, 165), Polygon5, Split, NoBounce));
    enemyCluster43.append(EnemyParams(Pos2D(605, 165), Polygon6, Split, Bounce));

    QList<EnemyParams> enemyCluster44;
    enemyCluster44.append(EnemyParams(Pos2D(250, 80), Polygon5, NoSplit, Bounce, EveryEdge, Rotate, NoMove, Track));
    enemyCluster44.append(EnemyParams(Pos2D(550, 80), Polygon5, NoSplit, Bounce, EveryEdge, Rotate, NoMove, Track));
    enemyCluster44.append(EnemyParams(Pos2D(125, 220), Polygon5, NoSplit, Bounce, EveryEdge, Rotate, NoMove, Track));
    enemyCluster44.append(EnemyParams(Pos2D(675, 220), Polygon5, NoSplit, Bounce, EveryEdge, Rotate, NoMove, Track));
    enemyCluster44.append(EnemyParams(Pos2D(400, 350), Polygon5, NoSplit, Bounce, EveryEdge, Rotate, NoMove, Track));

    QList<EnemyParams> enemyCluster45;
    enemyCluster45.append(EnemyParams(Pos2D(270, 65), Polygon3, Split, Bounce));
    enemyCluster45.append(EnemyParams(Pos2D(455, 65), Polygon3, Split, Bounce));
    enemyCluster45.append(EnemyParams(Pos2D(160, 220), Polygon3, Split, NoBounce));
    enemyCluster45.append(EnemyParams(Pos2D(555, 220), Polygon3, Split, Bounce));
    enemyCluster45.append(EnemyParams(Pos2D(270, 390), Polygon3, Split, NoBounce));
    enemyCluster45.append(EnemyParams(Pos2D(455, 390), Polygon3, Split, Bounce));

    QList<EnemyParams> enemyCluster46;
    enemyCluster46.append(EnemyParams(Pos2D(350, 60), Polygon8, Split, Bounce, EveryEdge, Rotate, NoMove, NoTrack, UnDestroyed));
    enemyCluster46.append(EnemyParams(Pos2D(350, 280), Polygon4, Split, Bounce, EveryEdge, Rotate, NoMove, NoTrack, UnDestroyed));

    QList<EnemyParams> enemyCluster47;
    enemyCluster47.append(EnemyParams(Pos2D(360, 150), Polygon12, Split, Bounce, EveryEdge, Rotate, NoMove, NoTrack));

    EnemyClusters[4].append(enemyCluster41);
    EnemyClusters[4].append(enemyCluster42);
    EnemyClusters[4].append(enemyCluster43);
    EnemyClusters[4].append(enemyCluster44);
    EnemyClusters[4].append(enemyCluster45);
    EnemyClusters[4].append(enemyCluster46);
    EnemyClusters[4].append(enemyCluster47);

    memset(ItemRatios, 0, ItemNum);

    double ratioSum = 0;
    for(int i=0; i<ItemNum; i++)
    {
        ratioSum += RoughItemRatios[i];
    }

    for(int i=0; i<ItemNum; i++)
    {
        double ratio = RoughItemRatios[i] / ratioSum * 100.0;
        if(i == 0)
            ItemRatios[i] = ratio;
        else
            ItemRatios[i] = ItemRatios[i-1] + ratio;
    }
}
