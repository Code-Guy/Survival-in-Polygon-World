#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>

namespace Ui {
class HelpWidget;
}

class HelpWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit HelpWidget(QWidget *parent = 0);
    ~HelpWidget();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_okBtn_clicked();

private:
    Ui::HelpWidget *ui;

    QPixmap enemy1, enemy2, enemy3, enemy4, enemy5,
        enemy6, enemy7, enemy8, enemy9, enemy10;

    QPixmap enemyEffect1, enemyEffect2, enemyEffect3, enemyEffect4;

    QPixmap player1, player2, player3;

    QPixmap item1, item2, item3, item4, item5, item6, item7;

signals:
    void Closed();
};

#endif // HELPWIDGET_H
