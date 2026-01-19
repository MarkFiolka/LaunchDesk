#pragma once

#include <QMainWindow>
#include <QVBoxLayout>
#include <qabstractitemview.h>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QPlainTextEdit>
#include <QStyle>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QListWidget>
#include <QComboBox>
#include <QLabel>

#include "application/controllers/debug.h"

class QMenuBar;
class QMenu;
class QAction;
class QSystemTrayIcon;
class QPlainTextEdit;
class QStackedWidget;
class QComboBox;

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
    QMenuBar* m_menuBar = nullptr;

    QMenu* m_menuFile = nullptr;
    QAction* m_newProfile = nullptr;
    QAction* m_newAction = nullptr;
    QAction* m_reload = nullptr;
    QAction* m_save = nullptr;
    QAction* m_menuHide = nullptr;
    QAction* m_menuExit = nullptr;

    QMenu* m_menuView = nullptr;
    QAction* m_console = nullptr;

    QWidget* m_centralWidget = nullptr;
    QVBoxLayout* m_centralLayout = nullptr;
    QComboBox* m_profileSelect = nullptr;
    QStackedWidget* m_profileStack = nullptr;

    QWidget* m_consoleOverlay = nullptr;
    QPlainTextEdit* m_logView = nullptr;

    QSystemTrayIcon* m_tray = nullptr;
    QMenu* m_menuTray = nullptr;
    QAction* m_toggleTray = nullptr;
    QAction* m_trayExit = nullptr;
};
