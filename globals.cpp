#include "globals.h"
#include <QMessageBox>
#include <QDebug>

//���ݿ��ѯ
QSqlDatabase db;
QSqlQuery query;

int enemyStats[EnemysNum];
int itemStats[ItemsNum];
int winSceneIndex;//�Ѿ����˵ڼ���

bool InitDatabase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("GJJ");
    db.setDatabaseName("GameParam.db");
    db.setUserName("GJJ");//�������ݿ���û���

    if(!db.open())
    {
        QMessageBox::warning(NULL, "error", "Cannot open database successfully!", QMessageBox::Yes);
        return false;
    }
    query = QSqlQuery(db);

    //������Ϸ��ģʽ�趨��
    if(!query.exec("create table if not exists gameparam"
                   "(difficulty int,"
                   "gameMode int,"
                   "whichPlane int,"
                   "whichScene int,"
                   "winSceneIndex int)")) //winSceneIndexΪ�Ѿ����˵ڼ���
        return false;

    //������Ϸ������ͳ�Ʊ�(����)
    if(!query.exec("create table if not exists gameenemystats"
                   "(enemyStats1 int,"
                   "enemyStats2 int,"
                   "enemyStats3 int,"
                   "enemyStats4 int,"
                   "enemyStats5 int,"
                   "enemyStats6 int,"
                   "enemyStats7 int,"
                   "enemyStats8 int,"
                   "enemyStats9 int,"
                   "enemyStats10 int)"))
        return false;

    //������Ϸ������ͳ�Ʊ�(����)
    if(!query.exec("create table if not exists gameitemstats"
                   "(itemStats1 int,"
                   "itemStats2 int,"
                   "itemStats3 int,"
                   "itemStats4 int,"
                   "itemStats5 int,"
                   "itemStats6 int,"
                   "itemStats7 int)"))
        return false;


    query.exec("select * from gameparam");

    if(!query.first())
    {
        query.exec("insert into gameparam values (2, 1, 1, 1, 1)");
    }

    query.exec("select * from gameenemystats");

    if(!query.first())
    {
        query.exec("insert into gameenemystats values (0, 0, 0, 0, 0, 0, 0, 0, 0, 0)");
    }

    query.exec("select * from gameitemstats");

    if(!query.first())
    {
        query.exec("insert into gameitemstats values (0, 0, 0, 0, 0, 0, 0, 0)");
    }

    //db.commit();
    return true;
}

bool ReadStats()//��ȡ���ݿ�
{
    if(!query.exec("select * from gameenemystats"))
    {
        qDebug() << "selet gameenemystats wrong!";
        return false;
    }
    //�������ݿ��е�����
    while(query.next())
    {
        for(int i=0; i<EnemysNum; i++)
        {
            enemyStats[i] = query.value(i).toInt();
        }
    }

    if(!query.exec("select * from gameitemstats"))
    {
        qDebug() << "selet gameitemstats wrong!";
        return false;
    }
    //�������ݿ��е�����
    while(query.next())
    {
        for(int i=0; i<ItemsNum; i++)
        {
            itemStats[i] = query.value(i).toInt();
        }
    }

    return true;
}

bool ReadWinSceneIndex()
{
    if(!query.exec("select * from gameparam"))
    {
        qDebug() << "selet gameparam wrong!";
        return false;
    }
    //�������ݿ��е�����
    while(query.next())
    {
        winSceneIndex = query.value(4).toInt();
    }
    return true;
}

bool UpdateDatabase()
{
    for(int i=0; i<EnemysNum; i++)
    {
        if(!query.exec(QString("update gameenemystats set enemyStats%1 = %2").arg(i+1).arg(enemyStats[i])))
            return false;
    }

    for(int i=0; i<ItemsNum; i++)
    {
        if(!query.exec(QString("update gameitemstats set itemStats%1 = %2").arg(i+1).arg(itemStats[i])))
            return false;
    }

    query.exec(QString("update gameparam set winSceneIndex = %1").arg(winSceneIndex));

    //db.commit();
    return true;
}
