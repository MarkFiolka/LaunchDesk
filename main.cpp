#include <QApplication>
#include <QFile>
#include <QDebug>

#ifdef _WIN32
#include <windows.h>
#endif

#include "ui/windows/LaunchDesk.h"

#ifdef _WIN32
static void setupConsole() {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}
#endif

static bool applyQss(QApplication &app, const QString &path) {
    QFile file(path);

    if (!file.exists()) {
        qDebug() << "QSS missing:" << path;
        return false;
    }

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "QSS open failed:" << path << "-" << file.errorString();
        return false;
    }

    const QString qss = QString::fromUtf8(file.readAll());
    app.setStyleSheet(app.styleSheet() + "\n\n" + qss);

    qDebug() << "QSS applied:" << path << "(" << qss.size() << "chars)";
    return true;
}

int main(int argc, char *argv[]) {
#ifdef _WIN32
    setupConsole();
#endif

    QApplication app(argc, argv);

    applyQss(app, ":/styles/QWidget.qss");
    app.setQuitOnLastWindowClosed(false);

    LaunchDesk window;

    return app.exec();
}
