#include "settingwidget.h"
#include "ui_settingwidget.h"
#include <QTimeEdit>
settingwidget::settingwidget(ItemInfo info,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::settingwidget)
    , m_ItemInfo(info)
{
    ui->setupUi(this);
    setWindowTitle("设置");
    setWindowFlags(Qt::Window);
    connect(ui->pushButton_sure,&QPushButton::clicked,this,&settingwidget::slots_sureclicked);
    connect(ui->pushButton_add,&QPushButton::clicked,this,&settingwidget::slots_newadd);
    connect(ui->pushButton_cancel,&QPushButton::clicked,this,[=](){
        close();
    });
    ui->lineEdit_name->setText(info.name);
    ui->spinBox_interal->setValue(info.interval);
    ui->plainTextEdit_content->setPlainText(info.message);

    for(const TimeRange& r : info.timevec){
        addTimeRange(r);
    }
}

settingwidget::~settingwidget()
{
    delete ui;
}

void settingwidget::slots_sureclicked()
{
    m_ItemInfo.name = ui->lineEdit_name->text();
    m_ItemInfo.interval = ui->spinBox_interal->value();
    m_ItemInfo.message = ui->plainTextEdit_content->toPlainText();
    m_ItemInfo.timevec = getRanges();

    emit signals_ItemInfo(m_ItemInfo);
    close();
}

void settingwidget::slots_newadd()
{
    TimeRange rg;
    rg.start = "10:00";
    rg.end = "11:00";
    addTimeRange(rg);
}

void settingwidget::addTimeRange(const TimeRange &range)
{
    QTimeEdit* startEdit = new QTimeEdit();
    startEdit->setDisplayFormat(TIME_FORMAT);
    QTime time = QTime::fromString(range.start, TIME_FORMAT);
    startEdit->setTime(time);

    QTimeEdit* endEdit   = new QTimeEdit();
    endEdit->setDisplayFormat(TIME_FORMAT);
    time = QTime::fromString(range.end, TIME_FORMAT);
    endEdit->setTime(time);

    QPushButton* btnDel = new QPushButton(QIcon(":/re/delete.png"),"");

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(startEdit);
    layout->addWidget(new QLabel("-"));
    layout->addWidget(endEdit);
    layout->addWidget(btnDel);

    ui->verticalLayout->addLayout(layout);

    connect(btnDel, &QPushButton::clicked, [=]() {
        removeLayout(layout);
    });
}

void settingwidget::removeLayout(QLayout *layout)
{
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* w = item->widget())
            delete w;
        delete item;
    }
    delete layout;
}

QVector<TimeRange> settingwidget::getRanges()
{
    QVector<TimeRange> vec;

    QVector<QTime> start_time,end_time;
    for (int i = 0; i < ui->verticalLayout->count(); ++i)
    {
        QLayout* layout = ui->verticalLayout->itemAt(i)->layout();
        QTimeEdit* start = qobject_cast<QTimeEdit*>(layout->itemAt(0)->widget());
        QTimeEdit* end = qobject_cast<QTimeEdit*>(layout->itemAt(2)->widget());

        start_time.append(start->time());
        end_time.append(end->time());
    }
    std::sort(start_time.begin(), start_time.end());
    std::sort(end_time.begin(), end_time.end());

    int size = start_time.size();
    for(int i = 0;i < size;++i){
        vec.append({ start_time[i].toString(TIME_FORMAT), end_time[i].toString(TIME_FORMAT) });
    }

    return vec;
}
