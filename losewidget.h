#ifndef LOSEWIDGET_H
#define LOSEWIDGET_H

#include <QWidget>
#include "helpwidget.h"

namespace Ui {
class LoseWidget;
}

class LoseWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit LoseWidget(QWidget *parent = 0);
    ~LoseWidget();
    
private slots:
    void on_restartBtn_clicked();

    void on_helpBtn_clicked();

    void on_menuBtn_clicked();

private:
    Ui::LoseWidget *ui;

    HelpWidget *helpWidget;

signals:
    void ReturnToMenu();
    void Restart();
};

#endif // LOSEWIDGET_H
