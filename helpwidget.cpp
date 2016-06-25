#include "helpwidget.h"
#include "ui_helpwidget.h"
#include <QPainter>

HelpWidget::HelpWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpWidget)
{
    ui->setupUi(this);

    this->setWindowTitle("Help");
    this->setFixedSize(744, 592);

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

    enemyEffect1 = QPixmap(":/images/polygon_3.png");
    enemyEffect2 = QPixmap(":/images/polygon_3_bounce.png");
    enemyEffect3 = QPixmap(":/images/polygon_3_split.png");
    enemyEffect4 = QPixmap(":/images/polygon_3_sb.png");

    player1 = QPixmap(":/images/player1.png");
    player2 = QPixmap(":/images/player2.png");
    player3 = QPixmap(":/images/player3.png");

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

    ui->enemyEffect1->setPixmap(enemyEffect1.scaled(ui->enemyEffect1->size()));
    ui->enemyEffect2->setPixmap(enemyEffect2.scaled(ui->enemyEffect2->size()));
    ui->enemyEffect3->setPixmap(enemyEffect3.scaled(ui->enemyEffect3->size()));
    ui->enemyEffect4->setPixmap(enemyEffect4.scaled(ui->enemyEffect4->size()));

    ui->player1->setPixmap(player1.scaled(ui->player1->size()));
    ui->player2->setPixmap(player2.scaled(ui->player2->size()));
    ui->player3->setPixmap(player3.scaled(ui->player3->size()));

    ui->item1->setPixmap(item1.scaled(ui->item1->size()));
    ui->item2->setPixmap(item2.scaled(ui->item2->size()));
    ui->item3->setPixmap(item3.scaled(ui->item3->size()));
    ui->item4->setPixmap(item4.scaled(ui->item4->size()));
    ui->item5->setPixmap(item5.scaled(ui->item5->size()));
    ui->item6->setPixmap(item6.scaled(ui->item6->size()));
    ui->item7->setPixmap(item7.scaled(ui->item7->size()));
}

HelpWidget::~HelpWidget()
{
    delete ui;
}

void HelpWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //»­±³¾°£¨·ÀÖ¹¾â³Ý»¯£©
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

    painter.drawText(QRect(130, 30, 450, 100), Qt::AlignCenter, "Welcome to the mysterious polygon world!\nAll you need to do"
                     " is survive!");

    font.setPointSize(10);
    painter.setFont(font);

    painter.drawText(QRect(90, 150, 100, 50), "Triangle");
    painter.drawText(QRect(90, 210, 100, 50), "Square");
    painter.drawText(QRect(90, 270, 100, 50), "Pentagon");
    painter.drawText(QRect(90, 330, 100, 50), "Hexagon");
    painter.drawText(QRect(90, 390, 100, 50), "Heptagon");

    painter.drawText(QRect(220, 150, 100, 50), "Octagon");
    painter.drawText(QRect(220, 210, 100, 50), "Nonagon");
    painter.drawText(QRect(220, 270, 100, 50), "Decagon");
    painter.drawText(QRect(220, 330, 100, 50), "Hendecagon");
    painter.drawText(QRect(220, 390, 100, 50), "Dodecagon");

    painter.drawText(QRect(45, 500, 100, 50), "Common");
    painter.drawText(QRect(110, 500, 100, 50), "Bounce");
    painter.drawText(QRect(170, 500, 100, 50), "Split");
    painter.drawText(QRect(210, 500, 100, 50), "Bounce&Split");

    painter.drawText(QRect(400, 210, 100, 50), "Jet");
    painter.drawText(QRect(400, 295, 100, 50), "Boomer");
    painter.drawText(QRect(400, 390, 100, 50), "Glider");

    painter.drawText(QRect(605, 150, 200, 50), "First Aid");
    painter.drawText(QRect(605, 205, 200, 50), "Boom");
    painter.drawText(QRect(605, 265, 200, 50), "Speed Up(Bullet)");
    painter.drawText(QRect(605, 325, 200, 50), "Shield");
    painter.drawText(QRect(595, 380, 200, 50), "Add Columns(Bullet)");
    painter.drawText(QRect(595, 430, 200, 50), "Clean Enemy Bullets");
    painter.drawText(QRect(605, 490, 200, 50), "Wave");
}

void HelpWidget::on_okBtn_clicked()
{
    this->hide();
    emit Closed();
}
