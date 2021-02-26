#include "mainwindow.h"

#include <QApplication>
#include <QSettings>

static QMap<QString, QPalette::ColorGroup> GROUP_NAME_MAP({
    {"Active", QPalette::Active},
    {"Disabled", QPalette::Disabled},
    {"Inactive", QPalette::Inactive}
});

static QMap<QString, QPalette::ColorRole> ROLE_NAME_MAP({
    {"WindowText", QPalette::WindowText},
    {"Button", QPalette::Button},
    {"Light", QPalette::Light},
    {"Midlight", QPalette::Midlight},
    {"Dark", QPalette::Dark},
    {"Mid", QPalette::Mid},
    {"Text", QPalette::Text},
    {"BrightText", QPalette::BrightText},
    {"ButtonText", QPalette::ButtonText},
    {"Base", QPalette::Base},
    {"Window", QPalette::Window},
    {"Shadow", QPalette::Shadow},
    {"Highlight", QPalette::Highlight},
    {"HighlightedText", QPalette::HighlightedText},
    {"Link", QPalette::Link},
    {"LinkVisited", QPalette::LinkVisited},
    {"AlternateBase", QPalette::AlternateBase},
    {"NoRole", QPalette::NoRole},
    {"ToolTipBase", QPalette::ToolTipBase},
    {"ToolTipText", QPalette::ToolTipText}
});

int main(int argc, char *argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication a(argc, argv);

    {
        QPalette palette;
        QSettings baseColorSettings(":/res/Darkstyle.ini", QSettings::IniFormat);
        baseColorSettings.beginGroup("StyleColors");
        for(const QString& groupKey : GROUP_NAME_MAP.keys()) {
            for(const QString& roleKey : ROLE_NAME_MAP.keys()) {
                QString key(groupKey + "_" + roleKey);
                if(baseColorSettings.contains(key)) {
                    palette.setColor(GROUP_NAME_MAP.value(groupKey),
                                     ROLE_NAME_MAP.value(roleKey),
                                     QColor(baseColorSettings.value(key).toString()));
                }
            }
        }
        a.setPalette(palette);
    }

    MainWindow w;
    w.show();
    return a.exec();
}
