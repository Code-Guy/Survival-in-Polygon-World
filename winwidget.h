#ifndef WINWIDGET_H
#define WINWIDGET_H

#include <QWidget>
#include "helpwidget.h"

namespace Ui {
class WinWidget;
}

class WinWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit WinWidget(QWidget *parent = 0);
    ~WinWidget();
    
private slots:
    void on_restartBtn_clicked();

    void on_helpBtn_clicked();

    void on_menuBtn_clicked();

private:
    Ui::WinWidget *ui;

    HelpWidget *helpWidget;

signals:
    void ReturnToMenu();
    void Restart();
};

#endif // WINWIDGET_H
