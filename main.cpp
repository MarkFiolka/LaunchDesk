#include <QApplication>
#include <QFile>
#include <QDebug>

#include "application/ui/LaunchDeskWindow.h"
#include "application/controllers/DockController.h"
#include "application/platform/WinHotkeyFilter.h"
#include "application/controllers/WindowsConsole.h"
#include "application/controllers/DebugAppConsole.h"

static void applyQss(QApplication &app, const QString &path) {
    QFile f(path);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        qWarning() << "QSS open failed:" << path << f.errorString();
        return;
    }
    app.setStyleSheet(app.styleSheet() + QString::fromUtf8(f.readAll()));
}


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

#ifdef _WIN32
    const bool enableConsole = false; // easy console toggle
    if (enableConsole) {
        WinConsole::attach();
        qDebug() << "Console attached";
    }
#endif

    applyQss(app, ":/styles/QWidget.qss");
    applyQss(app, ":/styles/QMenu.qss");
    applyQss(app, ":/styles/QMenuBar.qss");

    LaunchDeskWindow window;
    DockController dock(&window);
    DebugAppConsole debug(window.consoleOverlay(), window.logView());
    DebugAppConsole::setInstance(&debug);
    WinHotkeyFilter hotkey;
    app.installNativeEventFilter(&hotkey);
    hotkey.registerHotkey();

    QObject::connect(&hotkey, &WinHotkeyFilter::activated, &dock, &DockController::toggleDock);

    QObject::connect(&window, &LaunchDeskWindow::hideRequested, &dock, &DockController::hideDock);
    QObject::connect(&window, &LaunchDeskWindow::showRequested, &dock, &DockController::showDock);
    QObject::connect(&window, &LaunchDeskWindow::toggleConsoleRequested, &debug, &DebugAppConsole::toggle);
    QObject::connect(&window, &LaunchDeskWindow::exitRequested, &dock, &DockController::requestQuit);

    window.hide();
    const int rc = app.exec();

    hotkey.unregisterHotkey();
    return rc;
}
