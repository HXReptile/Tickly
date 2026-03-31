#include "settingwidget2.h"
#include "ui_settingwidget2.h"
#include <QSettings>
#include <QDir>
settingwidget2::settingwidget2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::settingwidget2)
{
    ui->setupUi(this);
    setWindowTitle("设置");
    setWindowFlags(Qt::Window);
    ui->checkBox_autostart->setChecked(isAutoStartEnabled());
    connect(ui->checkBox_autostart,&QCheckBox::toggled,this,&settingwidget2::slots_isAutoStart);
}

settingwidget2::~settingwidget2()
{
    delete ui;
}

void settingwidget2::slots_isAutoStart(bool enable)
{
    QString appName = QCoreApplication::applicationName();
    QString appPath = QCoreApplication::applicationFilePath();

    QSettings settings(
        "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        QSettings::NativeFormat
        );

    if (enable){
        settings.setValue(appName, QDir::toNativeSeparators(appPath));
    }else{
        settings.remove(appName);
    }
}

bool settingwidget2::isAutoStartEnabled()
{
    QString appName = QCoreApplication::applicationName();

    QSettings settings(
        "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        QSettings::NativeFormat
        );

    return settings.contains(appName);
}
