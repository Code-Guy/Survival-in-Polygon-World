#ifndef GLOBALS_H
#define GLOBALS_H

#include <QSqlQuery>
#include <QSqlDatabase>

const int EnemysNum = 10;//��10�����ͣ���״���ĵл�
const int ItemsNum = 7;//��7�����͵ĵ���

//���ݿ��ѯ
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
