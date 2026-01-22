#pragma once

#include <QMainWindow>
#include "application/controllers/debug.h"

class QMenuBar;
class QMenu;
class QAction;
class QSystemTrayIcon;
class QPlainTextEdit;
class QStackedWidget;
class QComboBox;
class QVBoxLayout;

class LaunchDeskWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit LaunchDeskWindow(QWidget *parent = nullptr);

    QComboBox* profileSelect() const { return m_profileSelect; }
    QStackedWidget* profileStack() const { return m_profileStack; }
    QWidget* consoleOverlay() const { return m_consoleOverlay; }
    QPlainTextEdit* logView() const { return m_logView; }

    signals:
    void newProfileRequested();
    void newActionRequested();
    void reloadRequested();
    void saveRequested();
    void hideRequested();
    void exitRequested();
    void toggleConsoleRequested();

private:
    void createMenuBar();
    void createFile();
    void createView();
    void createCentralUI();
    void createConsoleOverlay();
    void createTray();
    void updateTrayText();

private:
    QMenuBar*       m_menuBar        = nullptr;
    QMenu*          m_menuFile       = nullptr;
    QMenu*          m_menuView       = nullptr;
    QComboBox*      m_profileSelect  = nullptr;
    QStackedWidget* m_profileStack   = nullptr;
    QWidget*        m_consoleOverlay = nullptr;
    QPlainTextEdit* m_logView        = nullptr;
    QMenu*          m_menuTray       = nullptr;
    QAction*        m_toggleTray     = nullptr;
};
