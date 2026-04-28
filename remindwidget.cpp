#include "remindwidget.h"
#include "ui_remindwidget.h"
#include <QScreen>
#include <QTimer>

remindwidget::remindwidget(QWidget *parent)
    :     QWidget(NULL, Qt::Tool | Qt::FramelessWindowHint |
                        Qt::WindowStaysOnTopHint)
    , ui(new Ui::remindwidget)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);
    ui->label->setStyleSheet("background:transparent");
}

remindwidget::~remindwidget()
{
    delete ui;
}

void remindwidget::setText(QString text)
{
    ui->label->clear();

    ui->label->setText(text);
    move2center();
}

void remindwidget::move2center()
{
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect rect = screen->geometry();
    QPoint center = rect.center();

    int x = center.x() - width()/2;
    int y = center.y() - height()/2;
    move(x,y);
    QTimer* timer = new QTimer(this);
    timer->setInterval(1000);
    timer->start();
    int i = 0;
    bool bv = false;
    connect(timer,&QTimer::timeout,this,[=]()mutable{
        if(i > 30){
            if(timer->isActive()){
                timer->stop();
                close();
            }
        }else{
            if(bv){
                move(x + 10,y);
            }else{
                move(x - 10,y);
            }
            bv = !bv;
        }
        i++;
    });
}
