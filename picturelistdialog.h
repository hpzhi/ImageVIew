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
    void showThumbnail();
    void dispFile(QString filename);
    void initUI();
//    QStringList getTextFilesName();
//    void exportFiles();
//    void readTempFile(QString strFile);
//    void renameFile(QString);
//    void writeTempFile(QString strFile);
//    void importFiles();

signals:
    void fileName(QStringList);

protected:
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:

    bool isSingle;
    bool isDeltAll;
    QString newName;
    QString groupCalib;
    QString groupName;
    Ui::PictureListDialog *ui;
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

//    void UpdateThumb(QString,QString);
//    void showPicture(QListWidgetItem *item );
//    void sigProcess(int);
};

#endif // PICTURELISTDIALOG_H
