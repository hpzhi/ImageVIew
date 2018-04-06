#include "widget.h"
#include "ui_widget.h"
#include "picturelistdialog.h"
#include "common.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    PictureListDialog list(PICPATH, "*.png", false);
    list.exec();
}
