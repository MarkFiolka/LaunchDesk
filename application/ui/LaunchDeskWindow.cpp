#include "LaunchDeskWindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QPlainTextEdit>
#include <QStyle>
#include <QDockWidget>
#include <QVBoxLayout>

#include "application/controllers/debug.h"

LaunchDeskWindow::LaunchDeskWindow(QWidget *parent)
    : QMainWindow(parent) {
    setFixedSize(300, 600);
    createMenuBar();
    createConsoleOverlay();
    createTray();
}

void LaunchDeskWindow::createMenuBar() {
    m_menuBar = menuBar();

    createFile();
    createView();
}

void LaunchDeskWindow::createConsoleOverlay()
{
    m_consoleOverlay = new QWidget(this);
    m_consoleOverlay->setObjectName("ConsoleOverlay");
    m_consoleOverlay->setAttribute(Qt::WA_StyledBackground, true);
    m_consoleOverlay->setAutoFillBackground(false);
    m_consoleOverlay->hide();

    m_logView = new QPlainTextEdit(m_consoleOverlay);
    m_logView->setReadOnly(true);
    m_logView->setMaximumBlockCount(5000);
    m_logView->setFrameStyle(QFrame::NoFrame);

    auto* layout = new QVBoxLayout(m_consoleOverlay);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_logView);

    m_consoleOverlay->resize(width(), 300);
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
    connect(m_menuExit, &QAction::triggered, this, &LaunchDeskWindow::exitRequested); //implemented
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



