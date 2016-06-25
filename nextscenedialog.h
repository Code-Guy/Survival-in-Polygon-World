#ifndef NEXTSCENEDIALOG_H
#define NEXTSCENEDIALOG_H

#include <QDialog>
#include "helpwidget.h"

namespace Ui {
class NextSceneDialog;
}

class NextSceneDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit NextSceneDialog(QWidget *parent = 0);
    ~NextSceneDialog();

private slots:
    void on_nextBtn_clicked();

    void on_restartBtn_clicked();

    void on_helpBtn_clicked();

    void on_menuBtn_clicked();

private:
    Ui::NextSceneDialog *ui;

    HelpWidget *helpWidget;

signals:
    void Next();
    void ReturnToMenu();
    void Restart();
};

#endif // NEXTSCENEDIALOG_H
