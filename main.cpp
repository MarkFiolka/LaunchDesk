#include <QApplication>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QDebug>

#include "ui/windows/Welcome.h"
#include "ui/windows/LaunchDesk.h"
#include "core/ConfigManager.h"

static QString loadAllStyles()
{
    const QStringList files = {
        ":/styles/QWidget.qss",
        ":/styles/QFrame.qss",
        ":/styles/QLineEdit.qss",
        ":/styles/QListWidget.qss",
        ":/styles/QPushButton.qss",
        ":/styles/QPushButton_back.qss",
        ":/styles/QPushButton_icons.qss",
        ":/styles/QPushButton_viewed.qss",
        ":/styles/QCheckBox.qss"
    };

    QString finalStyle;
    qDebug() << "Loading stylesheets:";

    for (const QString& path : files)
    {
        QFile f(path);

        if (!f.exists()) {
            qDebug() << "  MISSING:" << path;
            continue;
        }

        if (!f.open(QFile::ReadOnly)) {
            qDebug() << "  FAILED TO OPEN:" << path;
            continue;
        }

        finalStyle += QString::fromUtf8(f.readAll()) + "\n\n";
        qDebug() << "  loaded:" << path;
    }

    return finalStyle;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QDir::setCurrent(QCoreApplication::applicationDirPath());

    QString style = loadAllStyles();
    app.setStyleSheet(style);
    qDebug() << "Final stylesheet size:" << style.size();

    ConfigManager cfg;
    cfg.load();

    bool showWelcome = cfg.isFirstStart() || cfg.getBool("showWelcome", true);

    if (showWelcome) {
        Welcome w;
        w.show();
        return app.exec();
    }

    LaunchDesk window;
    window.show();

    return app.exec();
}
