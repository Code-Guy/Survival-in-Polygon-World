#ifndef LEVELDIALOG_H
#define LEVELDIALOG_H

#include <QDialog>
#include <QWidget>
#include "gamewidget.h"

namespace Ui {
class LevelDialog;
}

class LevelDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit LevelDialog(QWidget *parent = 0);
    ~LevelDialog();

    void UpdateWinSceneIndex();
    
private slots:
    void on_scene1Btn_clicked();

    void on_scene2Btn_clicked();

    void on_scene3Btn_clicked();

    void on_scene4Btn_clicked();

    void on_scene5Btn_clicked();

    void on_okBtn_clicked();

    void on_cancelBtn_clicked();

private:
    Ui::LevelDialog *ui;

    int whichScene;
    int prevWhichScene;

    QWidget *parent;
    GameWidget *gameWidget;

    void SetUnchecked(int index);

signals:
    void UpdateWhichScene(int whichScene);//¸üÐÂ¹Ø¿¨ºÅ
    void ShowMenu();
    void StopMenuBGM();
};

#endif // LEVELDIALOG_H
