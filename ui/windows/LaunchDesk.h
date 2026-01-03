#pragma once

#include <QMainWindow>

class QSystemTrayIcon;
class QMenu;
class QAction;

class LaunchDesk : public QMainWindow
{
    Q_OBJECT

public:
    explicit LaunchDesk(QWidget *parent = nullptr);
    ~LaunchDesk() override;

public slots:
    void toggleDock();

protected:
    void closeEvent(QCloseEvent *event) override;
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;

private:
    void setupUi();
    void setupDockFlags();
    void setupTray();
    void registerHotkey();
    void unregisterHotkey();
    void moveToBottomRight(int margin = 12);

private:
    QSystemTrayIcon* m_tray = nullptr;
    QMenu* m_menu = nullptr;
    QAction* m_toggle = nullptr;
    QAction* m_quit = nullptr;

    bool m_allowClose = false;
    static constexpr int HOTKEY_ID = 0x4C44;
};
