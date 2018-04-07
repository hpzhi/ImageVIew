#include "pictureshowdialog.h"
#include "ui_pictureshowdialog.h"
#include "picturelistdialog.h"
#include <QResizeEvent>
#include <QMouseEvent>
#include <QDebug>
#include <QRect>

#define test
#ifndef test

#include "messagedialog.h"
#include "common.h"
#include <QtGui/QApplication>
#include "iconhelper.h"

#endif

#define LAST 1
#define NEXT 2
#define FULL 3
#define NORMAL 4
#define DELE 5
#ifdef MAC_OS_X_VERSION_10_13_4
const char* PICPATH = "/Users/hpzh/home/local/qtdir/ImageVIew/Images/";
const char* THUMBPATH = "/Users/hpzh/home/local/qtdir/ImageVIew/Images/thumb";
#elif
const char* PICPATH = "../ImageVIew/Images/";
const char* THUMBPATH = "../ImageVIew/Images/thumb";
#endif
const int SCREENW = 1920;

#define ImageTitle "picture"
PictureShowDialog::PictureShowDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PictureShowDialog)
{
    ui->setupUi(this);

    //设置图标
//    IconHelper::Instance()->SetIcon(ui->labelIcon, QChar(0xf03e), 20);//0xf03e
//    IconHelper::Instance()->SetIcon(ui->pushButtonClose, QChar(0xf00d), 20);//0xf00d

    //设置窗体标题栏隐藏
    this->setWindowFlags(Qt::FramelessWindowHint);
    //this->setWindowFlags(Qt::CustomizeWindowHint);

    ui->pushButtonMenu->hide();
    ui->pushButtonMin->hide();
    ui->pushButtonMax->hide();
    ui->labelTitle->setText(tr(ImageTitle));

    this->setMouseTracking(true);
    ui->widget_Main->setMouseTracking(true);
    ui->widget_Main->setAutoFillBackground(true);//这个属性一定要设置
    ui->pushButton_Forward->setMouseTracking(true);
    ui->pushButton_Fullscreen->setMouseTracking(true);
    ui->pushButton_Backward->setMouseTracking(true);
    ui->pushButton_Delete->setMouseTracking(true);

    showSize = NORMAL;
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(hideToolButton()));
    timer->start(4000);
}

void PictureShowDialog::hideToolButton( )
{
    ui->pushButton_Fullscreen->hide();
    ui->pushButton_Backward->hide();
    ui->pushButton_Delete->hide();
    ui->pushButton_Forward->hide();
}

PictureShowDialog::~PictureShowDialog()
{
    delete ui;
}

//实现鼠标拖动窗口移动
void PictureShowDialog::mouseMoveEvent(QMouseEvent *e)
{
    if(this->width() != SCREENW)
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

    QRect rect1,rect2,rect3,rect4;
    if(ui->widgetTitle->isHidden())
    {
        rect1 = QRect(ui->pushButton_Forward->pos().x(), ui->pushButton_Forward->pos().y(),
                      ui->pushButton_Forward->size().width(), ui->pushButton_Forward->size().height());

        rect2 = QRect(ui->pushButton_Fullscreen->pos().x(), ui->pushButton_Fullscreen->pos().y(),
                      ui->pushButton_Fullscreen->size().width(), ui->pushButton_Fullscreen->size().height());

        rect3 = QRect(ui->pushButton_Backward->pos().x(), ui->pushButton_Backward->pos().y(),
                      ui->pushButton_Backward->size().width(), ui->pushButton_Backward->size().height());

        rect4 = QRect(ui->pushButton_Delete->pos().x(), ui->pushButton_Delete->pos().y(),
                      ui->pushButton_Delete->size().width(), ui->pushButton_Delete->size().height());
    }

    else
    {
        rect1 = QRect(ui->pushButton_Forward->pos().x(), ui->pushButton_Forward->pos().y() + ui->widgetTitle->height(),
                      ui->pushButton_Forward->size().width(), ui->pushButton_Forward->size().height());

        rect2 = QRect(ui->pushButton_Fullscreen->pos().x(), ui->pushButton_Fullscreen->pos().y() + ui->widgetTitle->height(),
                      ui->pushButton_Fullscreen->size().width(), ui->pushButton_Fullscreen->size().height());

        rect3 = QRect(ui->pushButton_Backward->pos().x(), ui->pushButton_Backward->pos().y() + ui->widgetTitle->height(),
                      ui->pushButton_Backward->size().width(), ui->pushButton_Backward->size().height());

        rect4 = QRect(ui->pushButton_Delete->pos().x(), ui->pushButton_Delete->pos().y() + ui->widgetTitle->height(),
                      ui->pushButton_Delete->size().width(), ui->pushButton_Delete->size().height());
    }

    if(rect1.contains(e->pos()) || rect2.contains(e->pos()) ||
            rect3.contains(e->pos()) || rect4.contains(e->pos()))
    {
        timer->stop();
        e->accept();
    }
    else
    {
        timer->start(4000);
        ui->pushButton_Fullscreen->show();
        ui->pushButton_Forward->show();
        ui->pushButton_Delete->show();
        ui->pushButton_Backward->show();
    }
    //qDebug("e->pos() %d,%d",e->pos().x(),e->pos().y());
    //qDebug("rect1 %d,%d,%d,%d",rect1.x(),rect1.y(),rect1.width(),rect1.height());
    //qDebug("rect2 %d,%d,%d,%d",rect2.x(),rect2.y(),rect2.width(),rect2.height());
    //qDebug("rect3 %d,%d,%d,%d",rect3.x(),rect3.y(),rect3.width(),rect3.height());
    //qDebug("rect4 %d,%d,%d,%d",rect4.x(),rect4.y(),rect4.width(),rect4.height());
}

void PictureShowDialog::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        QRect rect;
        rect = QRect(ui->widgetTitle->pos(),ui->widgetTitle->size());
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

void PictureShowDialog::mouseReleaseEvent(QMouseEvent *)
{
    bePressed = false;
#ifdef QDEBUG_CAMCONTROL
    qDebug() << "release" << e->globalPos();
#endif
}
//需要用户调用，与resize不同
void PictureShowDialog::setBackGround(QString filename)
{
    QImage image;
    image.load(filename);
    QPalette palette(ui->widget_Main->palette());
    palette.setBrush(QPalette::Window,
                     QBrush(image.scaled(this->size(), Qt::IgnoreAspectRatio,
                                         Qt::SmoothTransformation)));
    ui->widget_Main->setPalette(palette);
}

void PictureShowDialog::on_pushButton_Backward_clicked()
{
    handleSignal(NEXT);
}

void PictureShowDialog::on_pushButton_Forward_clicked()
{
    handleSignal(LAST);
}

void PictureShowDialog::on_pushButton_Fullscreen_clicked()
{
    static int clickNum = 0;
    if(clickNum % 2)
    {
        showSize = NORMAL;
        handleSignal(NORMAL);
        // IconHelper::Instance()->SetIcon(ui->pushButton_Fullscreen, QChar(0xf0b2), 20);
    }
    else
    {
        showSize = FULL;
        handleSignal(FULL);
        //  IconHelper::Instance()->SetIcon(ui->pushButton_Fullscreen, QChar(0xf066), 20);
    }

    clickNum++;
}

void PictureShowDialog::on_pushButton_Delete_clicked()
{
#ifndef test
    MessageDialog msgDlg;
    msgDlg.setTitle(ICON_MSG,ICONSIZE_18,tr("Warning"));
    msgDlg.setMessage(tr("Do You Delete The Selected File?"));
    if(msgDlg.exec() == QDialog::Accepted)
    {
        handleSignal(DELE);
    }
#endif
}

// 随着窗体变化而设置背景，无须用户调用
void PictureShowDialog::resizeEvent(QResizeEvent *event)
{
    QImage image;
    image.load(strPicNamePath);
    QWidget::resizeEvent(event);
    QPalette pal(ui->widget_Main->palette());
    pal.setBrush(QPalette::Window,
                 QBrush(image.scaled(event->size(), Qt::IgnoreAspectRatio,
                                     Qt::SmoothTransformation)));
    ui->widget_Main->setPalette(pal);
}

void PictureShowDialog::passValue(QString str, QListWidget *list)
{
    listWidget = list;
    strPicNamePath = str;//qDebug("%s",strPicName.toAscii().data());
    handleSignal(NORMAL);
}

void PictureShowDialog::handleSignal(int sig)
{
    int row = listWidget->currentRow();
    //row  从零开始，
    if(sig == NEXT)
    {
        if(row != listWidget->count()-1)
        {
            listWidget->setCurrentRow(row + 1);
        }
        else
        {
            row = 0;
            listWidget->setCurrentRow(row);
        }
        setBackGround(PICPATH + listWidget->currentItem()->text());
        ui->labelTitle->setText(listWidget->currentItem()->text());
    }

    else if(sig == LAST)
    {
        if(row > 0)
        {
            listWidget->setCurrentRow(row - 1);

        }
        else
        {
            row = listWidget->count() - 1;
            listWidget->setCurrentRow(row);
        }
        setBackGround(PICPATH + listWidget->currentItem()->text());
        ui->labelTitle->setText(listWidget->currentItem()->text());
    }

    else if(sig == FULL)
    {
        QString fullFileName = listWidget->currentItem()->text();
        strPicNamePath = PICPATH + fullFileName;
        ui->widgetTitle->hide();
        ui->pushButton_Fullscreen->setStyleSheet("QPushButton{\
                                                              image: url(:/res/images/iconfont-suoxiao_32.png);}\
QPushButton:hover{\
                  image: url(:/res/images/iconfont-suoxiao_64.png);}");
        emit sig_FullScreen(strPicNamePath);
    }


    else if(sig == NORMAL)
    {
        QString normalFileName = listWidget->currentItem()->text();
        strPicNamePath = PICPATH + normalFileName;
        ui->widgetTitle->show();
        ui->pushButton_Fullscreen->setStyleSheet("QPushButton{ \
                                                               image: url(:/res/images/iconfont-weibiaoti2_32.png);}\
QPushButton:hover{\
                  image: url(:/res/images/iconfont-weibiaoti2_64.png);}");
        emit sig_NormalScreen(strPicNamePath);
        ui->labelTitle->setText(listWidget->currentItem()->text());
    }
    else
    {
#ifdef DEBUG
        qDebug("f listWidget = %d",listWidget->count());
#endif
        //点击删除按钮后，首先给原来窗口发送删除缩略图的信号，
        emit updateAlbum(row);
        if(listWidget->count() > 0)
        {
            if(row == listWidget->count())
            {
                //当删除最后一张图片时，跳到第一张图片进行显示
                row = 0;
            }
            listWidget->setCurrentRow(row);
            setBackGround(PICPATH + listWidget->currentItem()->text());
            ui->labelTitle->setText(listWidget->currentItem()->text());
        }
        else
        {
            //重载函数，输入参数格式#AARRGGBB
            this->close();
            //图片全部删除后，禁止所有的按钮操作
            ui->pushButton_Delete->setDisabled(true);
            ui->pushButton_Backward->setDisabled(true);
            ui->pushButton_Forward->setDisabled(true);
            ui->pushButton_Fullscreen->setDisabled(true);
        }
    }
}

void PictureShowDialog::on_pushButtonClose_clicked()
{
    this->close();
}
