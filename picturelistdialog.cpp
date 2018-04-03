#include "picturelistdialog.h"
#include "ui_picturelistdialog.h"
#include "iconhelper.h"
#include "messagedialog.h"
#include "common.h"
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QDebug>
#include <QMenuBar>
#include <QCheckBox>
#include <QPainter>
#include <QFileDialog>
#include <QModelIndex>
#include <QFileSystemModel>
#include <stdio.h>
#include "itemeditdialog.h"
#define LAST 1
#define NEXT 2
#define FULL 3
#define NORMAL 4
#define DELE 5

#define IconWidth  192 //48
#define IconHeight 108  //32
#define IconHintWidth   (IconWidth + 8)
#define IconHintHeight  (IconHeight + 6)

PictureListDialog::PictureListDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PictureListDialog), isDeltAll(false)
{
    initUI();
    ShowFileType = PictureFile;
    IconHelper::Instance()->SetIcon(ui->label_Ico, QChar(0xf03e), 20);//0xf03e
    pshowDlg = new PictureShowDialog();
    connect(pshowDlg, SIGNAL(updateAlbum(int)), this, SLOT(dele_picture(int)));
    connect(pshowDlg, SIGNAL(sig_FullScreen(QString)), this, SLOT(FullScreen(QString)));
    connect(pshowDlg, SIGNAL(sig_NormalScreen(QString)), this, SLOT(NormalScreen(QString)));
    isSingle = true;
    bePressed = false;
    ui->actionSingle->setChecked(isSingle);
    QActionGroup actionScreenGroup2(this);
    actionScreenGroup2.setExclusive(true);
    actionScreenGroup2.addAction(ui->actionSingle);
    actionScreenGroup2.addAction(ui->actionMult);
}

PictureListDialog::PictureListDialog(QString path, QString filter, bool isExport, QWidget *parent):
         QDialog(parent), ui(new Ui::PictureListDialog), isDeltAll(false)
{
    initUI();
    ShowFileType = TextFile;
    suffix = filter.mid(1);
    if(isExport)
    {
        ui->label_Ico->setStyleSheet("image: url(:/res/images/export.png)");
        ui->actionExOrImFiles->setText(tr("Export Udisk"));
        ui->label_Title->setText(tr("Export Udisk"));
    }
    else
    {
        ui->label_Ico->setStyleSheet( "image: url(:/res/images/import.png)");
        ui->actionExOrImFiles->setText(tr("Import Camera"));
        ui->label_Title->setText(tr("Import Camera"));
    }

    this->isExport = isExport;
    QString expFile(path);
    QDir expFileDir(expFile, filter);
    QFileInfoList list = expFileDir.entryInfoList();

    for(int nIndex = 0; nIndex < list.size(); ++nIndex)
    {
        //获取图片
        QFileInfo fileInfo = list.at(nIndex);
        QString fileName = fileInfo.completeBaseName();
        //basename();
        dispFile(fileName);
    }

    selfileNames.clear();
}

void PictureListDialog::initUI()
{
    ui->setupUi(this);
    //设置图标
    IconHelper::Instance()->SetIcon(ui->pushButton_Close, QChar(0xf00d), 20);//0xf00d

    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint);

    ui->pushButton_Menu->hide();
    ui->pushButton_Min->hide();
    ui->pushButton_Max->hide();
    ui->label_Title->setText(tr("Image List"));

    //设置QListWidget中的单元项的图片大小
    ui->listWidget->setIconSize(QSize(IconWidth, IconHeight));//QSize(32, 24)
    // ui->listWidget->setIconSize(QSize(100, 100));
    ui->listWidget->setResizeMode(QListView::Adjust);
    //设置QListWidget的显示模式
    ui->listWidget->setViewMode(QListView::IconMode);
    //设置QListWidget中的单元项不可被拖动
    ui->listWidget->setMovement(QListView::Static);
    //设置QListWidget中的单元项的间距
    ui->listWidget->setSpacing(16);
    //设置QListWidget中的单元项初始选择模式为单选
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);   //选择模式有:ExtendedSelection 按住ctrl多选, SingleSelection 单选 MultiSelection 点击多选 ContiguousSelection 鼠标拖拉多选
    ui->listWidget->setSelectionBehavior(QAbstractItemView::SelectItems);

   // showThumbnail();
    connect(ui->pushButton_Close, SIGNAL(clicked()), this, SLOT(hide()));


}

//获取当前目录下的图片，返回图片的名称列表
QStringList PictureListDialog::getImgFile(QString Path)
{
    dir.setPath(Path);
    // qDebug("name = %s", dir.currentPath().toAscii().data());
    QStringList filters;
    filters << "*.jpg" << "*.bmp" << "*.pnp";
    dir.setNameFilters(filters);
    dir.setSorting(QDir::Size);
    QStringList fileList = dir.entryList();
    return fileList;
}
//显示缩略图函数
void PictureListDialog::showThumbnail()
{
    QStringList fileList = getImgFile(PICPATH);
    int num = fileList.count();
    qDebug("num = %d", num);
    for(int i = 0; i < num; i ++)
    {
        //设置item项的图标和名称，通过filelist获取每个文件的名称
            //QIcon(":/res/images/zoom.jpg")
        QListWidgetItem *picItem = new QListWidgetItem(QIcon(":/res/images/zoom.jpg"), fileList.at(i));
        ui->listWidget->insertItem(i, picItem);
    }
}

void PictureListDialog::deleteItems()
{
    QList<QListWidgetItem *> pitems = ui->listWidget->selectedItems();

    QMutableListIterator<QListWidgetItem *> i(pitems);
    MessageDialog msgDlg;
    msgDlg.setTitle(ICON_MSG,ICONSIZE_18,tr("Warning"));

    if(isDeltAll)
        msgDlg.setMessage(tr("Delete all the pictures ?"));

    else
        msgDlg.setMessage(tr("Delete the selected pictures ?"));

    if(msgDlg.exec() == QDialog::Accepted)
    {
        while (i.hasNext())
        {
            QListWidgetItem *link = dynamic_cast<QListWidgetItem *> (i.next());
            if (link)
            {
                QString filename = link->text();
                //dir从硬盘中删除文件
                dir.remove(PICPATH + filename);
                dir.remove(THUMBPATH + filename);
                delete link;
                i.remove();
            }
        }
        qDeleteAll(pitems);
    }
}

PictureListDialog::~PictureListDialog()
{
    delete ui;
}

//实现鼠标拖动窗口移动
void PictureListDialog::mouseMoveEvent(QMouseEvent *e)
{
    if (bePressed && (e->buttons() && Qt::LeftButton))
    {
        this->move(e->globalPos() - mousePoint);
        e->accept();
#ifdef QDEBUG_CAMCONTROL
        qDebug() << "move" << e->globalPos();
#endif
    }
}

void PictureListDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        QRect rect;
        rect = QRect(ui->widget_Title->pos(),ui->widget_Title->size());
        if(rect.contains(e->pos()))
        {
            bePressed = true;
            mousePoint = e->globalPos() - this->pos();
            e->accept();
#ifdef QDEBUG_CAMCONTROL
            qDebug() << "press" << e->globalPos();
#endif
        }
    }
}

void PictureListDialog::mouseReleaseEvent(QMouseEvent *e)
{

#ifdef QDEBUG_CAMCONTROL
    qDebug() << "release" << e->globalPos();
#endif
}

void PictureListDialog::UpdateThumb(QString filepath,QString filename)
{
    ShowFileType = PictureFile;
    pItem = new QListWidgetItem(QIcon(filepath + filename), filename);
    //设置单元项的宽度和高度
    pItem->setSizeHint(QSize(192,130));
    ui->listWidget->insertItem(ui->listWidget->count()+1, pItem);
    qApp->processEvents();
}

void PictureListDialog::dispFile(QString filename)
{
    ShowFileType = TextFile;
    pItem = new QListWidgetItem(QIcon(":/res/images/file32.png"), filename);
    //设置单元项的宽度和高度
    pItem->setSizeHint(QSize(32, 32));
    ui->listWidget->insertItem(ui->listWidget->count() + 1, pItem);
    ui->listWidget->setViewMode(QListView::ListMode);
    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
    ui->listWidget->setFlow(QListView::TopToBottom);
    ui->listWidget->setSpacing(2);
}


void PictureListDialog::on_listWidget_customContextMenuRequested(QPoint pos)
{
    QMenu menu(this);
    if(ShowFileType == PictureFile)
    {
        if(ui->listWidget->count() == 0)
            return;

        menu.addAction(ui->actionSingle);
        menu.addAction(ui->actionMult);
        menu.addSeparator();
        menu.addAction(ui->actionDelt);
        menu.addAction(ui->actionDeleteAll);
    }

    else
    {
        menu.addAction(ui->actionExOrImFiles);
        if(ui->listWidget->selectedItems().isEmpty())
            ui->actionExOrImFiles->setEnabled(false);
        else
            ui->actionExOrImFiles->setEnabled(true);

    }
    menu.exec(this->cursor().pos());
}

void PictureListDialog::on_actionList_triggered()
{
    ui->listWidget->setIconSize(QSize(IconWidth - 16, IconHeight - 16));//QSize(32, 24)
    ui->listWidget->setViewMode(QListView::ListMode);
    ui->listWidget->setFlow(QListView::TopToBottom);
    ui->listWidget->setSpacing(2);
}

void PictureListDialog::on_actionIcon_triggered()
{
    ui->listWidget->setIconSize(QSize(IconWidth, IconHeight));
    ui->listWidget->setSpacing(16);
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setFlow(QListView::LeftToRight);
}

void PictureListDialog::on_actionSingle_triggered()
{
    ui->actionSingle->setChecked(true);
    ui->actionMult->setChecked(false);
    ui->listWidget->clearSelection();
    //bug：先多选，再单选时，多选的提示框仍存在
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}

void PictureListDialog::on_actionMult_triggered()
{
    ui->actionSingle->setChecked(false);
    ui->actionMult->setChecked(true);
    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
}

void PictureListDialog::on_actionDelt_triggered()
{
    if(ui->listWidget->selectedItems().count() > 0) {
        deleteItems();
    }
}

void PictureListDialog::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{
    if(ShowFileType == TextFile)
        return;
    //1. 双击缩略图，弹出新的对话窗用来显示图片
    //PictureShowDialog showDialog;

    //2. listWidget清除被选框
    ui->listWidget->clearSelection();
    QString fileName = item->text();
    //3. listwidget传递参数给showdialog
    pshowDlg->passValue(PICPATH + fileName, ui->listWidget);
    pshowDlg->exec();
}


void PictureListDialog::dele_picture(int row)
{
    //1. 从硬盘中删除图片
    QString filename = ui->listWidget->item(row)->text();
    dir.remove(PICPATH + filename);
    dir.remove(THUMBPATH + filename);
    //2. 从listwidget中删除图片
    ui->listWidget->takeItem(row);
}

void PictureListDialog::FullScreen(QString filename)
{
    this->setModal(false);
    pshowDlg->showFullScreen();
    pshowDlg->setBackGround(filename);
}

void PictureListDialog::NormalScreen(QString filename)
{
    pshowDlg->setModal(true);
    pshowDlg->showNormal();
    pshowDlg->setBackGround(filename);
}

//QStringList PictureListDialog::getTextFilesName()
//{
//    return selfileNames;
//}

void PictureListDialog::on_actionExOrImFiles_triggered()
{
    int index = 0;

    while(index < ui->listWidget->selectedItems().count())
    {
        selfileNames.append(ui->listWidget->selectedItems().at(index)->text() + suffix);
        index++;
    }

    if(selfileNames.count())
        emit fileName(selfileNames);

    close();
}


void PictureListDialog::on_actionDeleteAll_triggered()
{
    isDeltAll = true;
    ui->listWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

    for(int row = 0; row < ui->listWidget->count(); row++)
    {
        ui->listWidget->setCurrentRow(row);
        ui->listWidget->item(row)->setSelected(true);
    }

    deleteItems();
    isDeltAll = false;
    ui->listWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
}