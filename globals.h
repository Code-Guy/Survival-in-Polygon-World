#ifndef GLOBALS_H
#define GLOBALS_H

#include <QSqlQuery>
#include <QSqlDatabase>

const int EnemysNum = 10;//有10种类型（形状）的敌机
const int ItemsNum = 7;//有7种类型的道具

//数据库查询
extern QSqlDatabase db;
extern QSqlQuery query;

extern int enemyStats[EnemysNum];
extern int itemStats[ItemsNum];
extern int winSceneIndex;

bool InitDatabase();
bool ReadStats();
bool ReadWinSceneIndex();
bool UpdateDatabase();

#endif // GLOBALS_H
