#ifndef PICTURELISTDIALOG_H
#define PICTURELISTDIALOG_H

#include <QDir>
#include <QDialog>
#include <QListWidgetItem>
#include "pictureshowdialog.h"
#define  PictureFile 1
#define  TextFile 2
namespace Ui {
    class PictureListDialog;
}

class PictureListDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PictureListDialog(QWidget *parent = 0);
     PictureListDialog(QString path, QString filter, bool isExport, QWidget *parent = 0);
    ~PictureListDialog();
    QStringList getImgFile(QString Path);
    void initUI();

signals:
    void fileName(QStringList);

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    Ui::PictureListDialog *ui;
    bool isSingle;
    bool isDeltAll;
    QString newName;
    QString groupCalib;
    QString groupName;
    QListWidgetItem *pItem;
    QPoint mousePoint;
    bool isExport;
    bool bePressed;
    QDir dir;
    PictureShowDialog *pshowDlg;
    int ShowFileType;
    QStringList selfileNames;
    QString suffix;

private slots:

    void on_actionDeleteAll_triggered();
    void on_actionExOrImFiles_triggered();
    void deleteItems();
    void UpdateThumb(QString filepath, QString filename);
    void on_listWidget_itemDoubleClicked(QListWidgetItem* item);
    void on_actionDelt_triggered();
    void on_actionMult_triggered();
    void on_actionSingle_triggered();
    void on_actionIcon_triggered();
    void on_actionList_triggered();
    void on_listWidget_customContextMenuRequested(QPoint pos);
    void dele_picture(int row);
    void FullScreen(QString);
    void NormalScreen(QString);
    void on_actionRename_triggered();
};

#endif // PICTURELISTDIALOG_H
