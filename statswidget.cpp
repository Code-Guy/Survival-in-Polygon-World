#include "statswidget.h"
#include "ui_statswidget.h"

#include "globals.h"
#include <QPainter>

StatsWidget::StatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatsWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("Stats");
    this->setFixedSize(582, 531);

    enemy1 = QPixmap(":/images/polygon_3.png");
    enemy2 = QPixmap(":/images/polygon_4.png");
    enemy3 = QPixmap(":/images/polygon_5.png");
    enemy4 = QPixmap(":/images/polygon_6.png");
    enemy5 = QPixmap(":/images/polygon_7.png");
    enemy6 = QPixmap(":/images/polygon_8.png");
    enemy7 = QPixmap(":/images/polygon_9.png");
    enemy8 = QPixmap(":/images/polygon_10.png");
    enemy9 = QPixmap(":/images/polygon_11.png");
    enemy10 = QPixmap(":/images/polygon_12.png");

    item1 = QPixmap(":/images/firstaid.png");
    item2 = QPixmap(":/images/boomer.png");
    item3 = QPixmap(":/images/speedup.png");
    item4 = QPixmap(":/images/shield.png");
    item5 = QPixmap(":/images/lines.png");
    item6 = QPixmap(":/images/disappear.png");
    item7 = QPixmap(":/images/wave.png");

    ui->enemy1->setPixmap(enemy1.scaled(ui->enemy1->size()));
    ui->enemy2->setPixmap(enemy2.scaled(ui->enemy2->size()));
    ui->enemy3->setPixmap(enemy3.scaled(ui->enemy3->size()));
    ui->enemy4->setPixmap(enemy4.scaled(ui->enemy4->size()));
    ui->enemy5->setPixmap(enemy5.scaled(ui->enemy5->size()));
    ui->enemy6->setPixmap(enemy6.scaled(ui->enemy6->size()));
    ui->enemy7->setPixmap(enemy7.scaled(ui->enemy7->size()));
    ui->enemy8->setPixmap(enemy8.scaled(ui->enemy8->size()));
    ui->enemy9->setPixmap(enemy9.scaled(ui->enemy9->size()));
    ui->enemy10->setPixmap(enemy10.scaled(ui->enemy10->size()));

    ui->item1->setPixmap(item1.scaled(ui->item1->size()));
    ui->item2->setPixmap(item2.scaled(ui->item2->size()));
    ui->item3->setPixmap(item3.scaled(ui->item3->size()));
    ui->item4->setPixmap(item4.scaled(ui->item4->size()));
    ui->item5->setPixmap(item5.scaled(ui->item5->size()));
    ui->item6->setPixmap(item6.scaled(ui->item6->size()));
    ui->item7->setPixmap(item7.scaled(ui->item7->size()));
}

StatsWidget::~StatsWidget()
{
    delete ui;
}

void StatsWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    //画背景（防止锯齿化）
    QPen penBack(Qt::white);
    QBrush brushBack(Qt::white);
    painter.setPen(penBack);
    painter.setBrush(brushBack);
    painter.drawRect(rect());

    QPen pen(Qt::black);
    QBrush brush(Qt::black);
    QFont font("Consolas", 15);
    painter.setPen(pen);
    painter.setBrush(brush);
    painter.setFont(font);

    painter.drawText(QRect(100, 145, 100, 50), QString("X %1").arg(enemyStats[0]));
    painter.drawText(QRect(100, 205, 100, 50), QString("X %1").arg(enemyStats[1]));
    painter.drawText(QRect(100, 265, 100, 50), QString("X %1").arg(enemyStats[2]));
    painter.drawText(QRect(100, 325, 100, 50), QString("X %1").arg(enemyStats[3]));
    painter.drawText(QRect(100, 385, 100, 50), QString("X %1").arg(enemyStats[4]));

    painter.drawText(QRect(230, 145, 100, 50), QString("X %1").arg(enemyStats[5]));
    painter.drawText(QRect(230, 205, 100, 50), QString("X %1").arg(enemyStats[6]));
    painter.drawText(QRect(230, 265, 100, 50), QString("X %1").arg(enemyStats[7]));
    painter.drawText(QRect(230, 325, 100, 50), QString("X %1").arg(enemyStats[8]));
    painter.drawText(QRect(230, 385, 100, 50), QString("X %1").arg(enemyStats[9]));

    painter.drawText(QRect(460, 105, 100, 50), QString("X %1").arg(itemStats[0]));
    painter.drawText(QRect(460, 155, 100, 50), QString("X %1").arg(itemStats[1]));
    painter.drawText(QRect(460, 215, 100, 50), QString("X %1").arg(itemStats[2]));
    painter.drawText(QRect(460, 270, 100, 50), QString("X %1").arg(itemStats[3]));
    painter.drawText(QRect(460, 325, 100, 50), QString("X %1").arg(itemStats[4]));
    painter.drawText(QRect(460, 385, 100, 50), QString("X %1").arg(itemStats[5]));
    painter.drawText(QRect(460, 435, 100, 50), QString("X %1").arg(itemStats[6]));
}

void StatsWidget::on_okBtn_clicked()
{
    this->hide();
}

void StatsWidget::UpdateStats()
{
    ReadStats();//读取数据库,读取可能被更新的数据
}
