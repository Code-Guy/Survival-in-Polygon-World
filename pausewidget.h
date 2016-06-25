#ifndef PAUSEWIDGET_H
#define PAUSEWIDGET_H

#include <QWidget>
#include "helpwidget.h"

namespace Ui {
class PauseWidget;
}

class PauseWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PauseWidget(QWidget *parent = 0);
    ~PauseWidget();

private:
    Ui::PauseWidget *ui;

    HelpWidget *helpWidget;

private slots:
    void on_resumeBtn_clicked();
    void on_helpBtn_clicked();
    void on_menuBtn_clicked();

    void on_restartBtn_clicked();

signals:
    void Resume();
    void ReturnToMenu();
    void Restart();
};

#endif // PAUSEWIDGET_H
