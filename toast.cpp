#include "toast.h"
#include "ui_toast.h"
#include <QTimer>
#include <QApplication>
#include <QScreen>
toast::toast(const QString &text,QWidget *parent) :
    QWidget(NULL, Qt::Tool | Qt::FramelessWindowHint |
            Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint),
    ui(new Ui::toast)
  ,m_posY(0)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    m_autoCloseTimer = new QTimer(this);
    m_autoCloseTimer->setSingleShot(true);
    connect(m_autoCloseTimer, SIGNAL(timeout()), this, SLOT(slots_closeLater()));
    m_autoCloseTimer->start(60000);   // 10 秒

    static QIcon iconZero(":/header/Resource/message_info.svg");
    QPixmap pixmap = iconZero.pixmap(ui->label_icon->width(), ui->label_icon->height()); // 宽度, 高度
    ui->label_icon->setPixmap(pixmap);
    ui->label_content->setText(text);

    setStyleSheet("background-color: rgb(240, 244, 245);");
    setFixedWidth(QGuiApplication::primaryScreen()->availableGeometry().width()/8);
    show();
}

toast::~toast()
{
    delete ui;
}

void toast::setPosY(int y)
{
    if (m_posY != y) {
        m_posY = y;
        move(pos().x(), y);   // 只改 Y 轴
    }
}

int toast::posY()
{
    return m_posY;
}

void toast::slots_closeLater()
{
    close();
}
