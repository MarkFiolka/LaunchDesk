#pragma once
#include <QMainWindow>

#include "application/controllers/DebugAppConsole.h"

class QMenuBar;
class QMenu;
class QAction;
class QSystemTrayIcon;
class QPlainTextEdit;

class LaunchDeskWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit LaunchDeskWindow(QWidget *parent = nullptr);

    QWidget* consoleOverlay() const { return m_consoleOverlay; }
    QPlainTextEdit* logView() const { return m_logView; }

signals:
    void newProfileRequested();

    void newActionRequested();

    void reloadRequested();

    void saveRequested();

    void hideRequested();

    void showRequested();

    void exitRequested();

    void toggleConsoleRequested();

private:
    void createMenuBar();
    QMenuBar *m_menuBar = nullptr;

    void createFile();
    QMenu *m_menuFile = nullptr;
    QAction *m_newProfile = nullptr;
    QAction *m_newAction = nullptr;
    QAction *m_reload = nullptr;
    QAction *m_save = nullptr;
    QAction *m_menuHide = nullptr;
    QAction *m_menuExit = nullptr;

    void createView();
    QMenu *m_menuView = nullptr;
    QAction *m_console = nullptr;

    void createConsoleOverlay();
    QWidget* m_consoleOverlay = nullptr;
    QPlainTextEdit* m_logView = nullptr;

    void createTray();
    void updateTrayText();
    QSystemTrayIcon *m_tray = nullptr;
    QMenu *m_menuTray = nullptr;
    QAction *m_toggleTray = nullptr;
    QAction *m_trayExit = nullptr;
};
