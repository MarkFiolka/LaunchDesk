#include "LaunchDeskWindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QPlainTextEdit>
#include <QStyle>
#include <QDockWidget>

#include "application/controllers/DockController.h"

LaunchDeskWindow::LaunchDeskWindow(QWidget *parent)
    : QMainWindow(parent) {
    setFixedSize(300, 600);
    createMenuBar();
    createConsole();
    createTray();
}

void LaunchDeskWindow::createMenuBar() {
    m_menuBar = menuBar();

    createFile();
    createView();
}

void LaunchDeskWindow::createConsole() {
    m_logView = new QPlainTextEdit();
    m_logView->setReadOnly(true);
    m_logView->setMaximumBlockCount(5000);

    m_consoleDock = new QDockWidget("Console", this);
    m_consoleDock->setWidget(m_logView);
    m_consoleDock->setAllowedAreas(Qt::BottomDockWidgetArea);
    m_consoleDock->setFeatures(QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable);

    addDockWidget(Qt::BottomDockWidgetArea, m_consoleDock);
}

void LaunchDeskWindow::createFile() {
    m_menuFile = m_menuBar->addMenu(tr("&File"));

    m_newProfile = m_menuFile->addAction(
        QApplication::style()->standardIcon(QStyle::SP_FileDialogNewFolder),
        tr("New Profile"));

    m_newAction = m_menuFile->addAction(
        QApplication::style()->standardIcon(QStyle::SP_FileIcon),
        tr("New Action"));

    m_menuFile->addSeparator();

    m_reload = m_menuFile->addAction(
        QApplication::style()->standardIcon(QStyle::SP_BrowserReload),
        tr("Reload"));

    m_save = m_menuFile->addAction(
        QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton),
        tr("Save"));

    m_menuFile->addSeparator();

    m_menuHide = m_menuFile->addAction(
        QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton),
        tr("Hide"));

    m_menuExit = m_menuFile->addAction(
        QApplication::style()->standardIcon(QStyle::SP_MediaStop),
        tr("Exit"));

    connect(m_newProfile, &QAction::triggered, this, &LaunchDeskWindow::newProfileRequested);
    connect(m_newAction, &QAction::triggered, this, &LaunchDeskWindow::newActionRequested);
    connect(m_reload, &QAction::triggered, this, &LaunchDeskWindow::reloadRequested);
    connect(m_save, &QAction::triggered, this, &LaunchDeskWindow::saveRequested);
    connect(m_menuHide, &QAction::triggered, this, &LaunchDeskWindow::hideRequested);
    connect(m_menuExit, &QAction::triggered, this, &LaunchDeskWindow::exitRequested);
}

void LaunchDeskWindow::createView()
{
    m_menuView = m_menuBar->addMenu(tr("&View"));

    m_console = m_menuView->addAction(
        QApplication::style()->standardIcon(QStyle::SP_CommandLink),
        tr("Toggle Console"));

    connect(m_console, &QAction::triggered,
            this, &LaunchDeskWindow::toggleConsoleRequested);
}

void LaunchDeskWindow::createTray()
{
    m_tray = new QSystemTrayIcon(this);
    m_tray->setIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));

    m_menuTray = new QMenu(this);

    m_toggleTray = m_menuTray->addAction(tr("Show"));
    m_trayExit   = m_menuTray->addAction(tr("Exit"));

    connect(m_toggleTray, &QAction::triggered,
            this, &LaunchDeskWindow::toggleConsoleRequested);

    connect(m_trayExit, &QAction::triggered,
            this, &LaunchDeskWindow::exitRequested);

    connect(m_tray, &QSystemTrayIcon::activated,
            this, [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger)
            emit toggleConsoleRequested();
    });

    m_tray->setContextMenu(m_menuTray);
    m_tray->show();

    updateTrayText();
}

void LaunchDeskWindow::updateTrayText()
{
    if (!m_toggleTray)
        return;

    if (isVisible()) {
        m_toggleTray->setText(tr("Hide"));
    } else {
        m_toggleTray->setText(tr("Show"));
    }
}



