#include "LaunchDesk.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QCursor>
#include <QGuiApplication>
#include <QMenu>
#include <QPushButton>
#include <QScreen>
#include <QSystemTrayIcon>
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include <QTimer>

#ifdef _WIN32
#include <windows.h>
#endif

LaunchDesk::LaunchDesk(QWidget *parent)
    : QMainWindow(parent)
{
    setFixedSize(300, 600);

    setupDockFlags();
    setupTray();
    registerHotkey();
    setupUi();

    hide();
}

LaunchDesk::~LaunchDesk()
{
    unregisterHotkey();
}

void LaunchDesk::setupUi()
{
    auto *root = new QWidget(this);
    setCentralWidget(root);

    auto *layout = new QVBoxLayout(root);
    auto *button = new QPushButton("Click me", root);
    layout->addWidget(button);

    connect(button, &QPushButton::clicked, this, [] {
        qDebug() << "Button clicked!";
    });
}

void LaunchDesk::setupDockFlags()
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
}

void LaunchDesk::setupTray()
{
    m_tray = new QSystemTrayIcon(qApp->windowIcon(), this);
    m_tray->setToolTip("LaunchDesk");

    m_menu = new QMenu(this);
    m_toggle = m_menu->addAction("Show/Hide", this, &LaunchDesk::toggleDock);
    m_menu->addSeparator();
    m_quit = m_menu->addAction("Quit", this, [this] {
        m_allowClose = true;
        qApp->quit();
    });

    m_tray->setContextMenu(m_menu);

    connect(m_tray, &QSystemTrayIcon::activated, this,
            [this](QSystemTrayIcon::ActivationReason reason) {
                if (reason == QSystemTrayIcon::Trigger || reason == QSystemTrayIcon::DoubleClick)
                    toggleDock();
            });

    m_tray->show();
}

void LaunchDesk::registerHotkey()
{
#ifdef _WIN32
    (void)winId();
    RegisterHotKey(reinterpret_cast<HWND>(winId()), HOTKEY_ID,
                   MOD_CONTROL | MOD_SHIFT | MOD_ALT, 0x4C);
#endif
}

void LaunchDesk::unregisterHotkey()
{
#ifdef _WIN32
    UnregisterHotKey(reinterpret_cast<HWND>(winId()), HOTKEY_ID);
#endif
}

bool LaunchDesk::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
#ifdef _WIN32
    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG") {
        auto *msg = static_cast<MSG *>(message);
        if (msg && msg->message == WM_HOTKEY && (int)msg->wParam == HOTKEY_ID) {
            toggleDock();
            if (result) *result = 0;
            return true;
        }
    }
#else
    Q_UNUSED(eventType)
    Q_UNUSED(message)
    Q_UNUSED(result)
#endif
    return QMainWindow::nativeEvent(eventType, message, result);
}

void LaunchDesk::closeEvent(QCloseEvent *event)
{
    if (!m_allowClose) {
        event->ignore();
        hide();
        return;
    }
    QMainWindow::closeEvent(event);
}

void LaunchDesk::toggleDock()
{
    if (isVisible()) { hide(); return; }

    show();

    QTimer::singleShot(0, this, [this]{
        adjustSize();
        moveToBottomRight();
        raise();
        activateWindow();
    });
}

void LaunchDesk::moveToBottomRight(int margin)
{
    auto *screen = QGuiApplication::screenAt(QCursor::pos());
    if (!screen) screen = QGuiApplication::primaryScreen();
    if (!screen) return;

    const QRect avail = screen->availableGeometry();
    move(avail.right() - width() - margin, avail.bottom() - height() - margin);
}
