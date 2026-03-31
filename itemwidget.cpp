#include "itemwidget.h"
#include "ui_itemwidget.h"

itemwidget::itemwidget(ItemInfo info,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::itemwidget)
{
    ui->setupUi(this);
    ui->label_name->setText(info.name);
    ui->checkBox_isUse->setChecked(info.enabled);
    connect(ui->pushButton_delete,&QPushButton::clicked,[&](){
        emit signals_delete();
    });
    connect(ui->pushButton_setting,&QPushButton::clicked,[&](){
        emit signals_setting();
    });
    connect(ui->checkBox_isUse,&QPushButton::toggled,this,&itemwidget::slots_toggledclicked);
}

itemwidget::~itemwidget()
{
    delete ui;
}

void itemwidget::setItemInfo(ItemInfo info)
{
    ui->label_name->setText(info.name);
    ui->checkBox_isUse->setChecked(info.enabled);
}

void itemwidget::slots_toggledclicked(bool tg)
{
    emit signals_toggled(tg);
}
