#ifndef SETDIALOG_H
#define SETDIALOG_H

#include <QDialog>
#include <QWidget>
#include <phonon>

namespace Ui {
class SetDialog;
}

class SetDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SetDialog(QWidget *parent = 0);
    ~SetDialog();
    
protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_okBtn_clicked();
    void on_cancelBtn_clicked();
    void radioChange();

private:
    Ui::SetDialog *ui;
    QWidget *parent;

    QPixmap backgroundLace;
    int difficulty;

signals:
    void UpdateDifficulty(int difficulty);//更新游戏难度
};

#endif // SETDIALOG_H
