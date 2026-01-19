#include "DockController.h"
#include "debug.h"

#include <QCursor>
#include <QScreen>
#include <QGuiApplication>

DockController::DockController(QWidget *window, QObject *parent)
    : QObject(parent)
      , m_window(window) {
    m_window->installEventFilter(this);
    setupDockFlags();
}

void DockController::setupDockFlags() {
    m_window->setWindowFlags(
        Qt::Tool |
        Qt::FramelessWindowHint |
        Qt::WindowStaysOnTopHint
    );
}

void DockController::toggleDock() {
    if (m_window->isVisible()) {
        hideDock();
        dlog("Dock shown");
    } else {
        showDock();
        dlog("Dock shown");
    }

}

void DockController::showDock() {
    if (m_window->isVisible())
        return;

    m_window->show();


    QTimer::singleShot(0, m_window, [this] {
        if (!m_dockPosSet)
            moveToBottomRight();

        m_window->raise();
        m_window->activateWindow();
    });
}

void DockController::hideDock() {
    m_window->hide();
}

void DockController::requestQuit() {
    m_allowClose = true;
    m_window->close();
    QCoreApplication::quit();
}

void DockController::moveToBottomRight(int margin) {
    auto *screen = QGuiApplication::screenAt(QCursor::pos());
    if (!screen)
        screen = QGuiApplication::primaryScreen();
    if (!screen)
        return;

    const QRect avail = screen->availableGeometry();
    m_window->move(
        avail.right() - m_window->width() - margin,
        avail.bottom() - m_window->height() - margin
    );

    m_dockPosSet = true;
}

bool DockController::eventFilter(QObject *obj, QEvent *ev) {
    if (obj == m_window && ev->type() == QEvent::Close) {
        auto *ce = static_cast<QCloseEvent *>(ev);
        if (!m_allowClose) {
            ce->ignore();
            m_window->hide();
            return true;
        }
    }
    return QObject::eventFilter(obj, ev);
}
