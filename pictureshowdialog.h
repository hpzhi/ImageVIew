#ifndef PICTURESHOWDIALOG_H
#define PICTURESHOWDIALOG_H

#include <QTimer>
#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
    class PictureShowDialog;
}

class PictureShowDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PictureShowDialog(QWidget *parent = 0);
    ~PictureShowDialog();
    void handleListFile(QListWidget *pListItem);
    void handleSignal(QListWidget *pListItem, int sig);

protected:
    void resizeEvent(QResizeEvent *);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

signals:
    void sendSignal(int );
    void updateAlbum(int row);
    void sig_FullScreen(QString);
    void sig_NormalScreen(QString);

public slots:
    void setBackGround(QString );
    void passValue(QString str, QListWidget *list);


private:
    Ui::PictureShowDialog *ui;
    int showSize;
    QString strPicNamePath;
    int sigValue;
    QListWidget *listWidget;
    QTimer *timer;
    //变量传递listdiaglog-->showdialog，通过pass..
    QPoint mousePoint;
    bool bePressed;

private slots:

    void on_pushButtonClose_clicked();
    void on_pushButton_Delete_clicked();
    void on_pushButton_Fullscreen_clicked();
    void on_pushButton_Forward_clicked();
    void on_pushButton_Backward_clicked();
    void hideToolButton();
    void handleSignal(int sig);

};

#endif // PICTURESHOWDIALOG_H
