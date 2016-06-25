#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QTime>
#include <QSplashScreen>
#include "menuwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForLocale());

    QPixmap pix(":/images/splash.png");
    QSplashScreen splash(pix);
    splash.resize(pix.size());
    splash.show();
    a.processEvents();

    //��ʼ�����������
    QTime t;
    t= QTime::currentTime();
    qsrand(t.msec()+t.second()*1000);

    //����qss��ʽ��
    QString qss;
    QFile qssFile(":/qss/format.qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        qss = QLatin1String(qssFile.readAll());
        a.setStyleSheet(qss);
        qssFile.close();
    }


    MenuWidget menuWidget;
    menuWidget.show();

    splash.finish(&menuWidget);

    return a.exec();
}
