#ifndef STATSWIDGET_H
#define STATSWIDGET_H

#include <QWidget>

namespace Ui {
class StatsWidget;
}

class StatsWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit StatsWidget(QWidget *parent = 0);
    ~StatsWidget();
    
    void UpdateStats();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_okBtn_clicked();

private:
    Ui::StatsWidget *ui;

    QPixmap enemy1, enemy2, enemy3, enemy4, enemy5,
        enemy6, enemy7, enemy8, enemy9, enemy10;

    QPixmap item1, item2, item3, item4, item5, item6, item7;
};

#endif // STATSWIDGET_H
