#include "LaunchDeskWindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QMenuBar>
#include <QApplication>
#include <QComboBox>
#include <QStackedWidget>
#include <QPlainTextEdit>
#include <QSystemTrayIcon>

LaunchDeskWindow::LaunchDeskWindow(QWidget *parent)
    : QMainWindow(parent) {
    setFixedSize(300, 600);
    createMenuBar();
    createCentralUI();
    createConsoleOverlay();
    createTray();
}

void LaunchDeskWindow::createMenuBar() {
    m_menuBar = menuBar();

    createFile();
    createView();
}

void LaunchDeskWindow::createFile() {
    m_menuFile = m_menuBar->addMenu(tr("&File"));

    QAction *newProfile = m_menuFile->addAction(
        QApplication::style()->standardIcon(QStyle::SP_FileDialogNewFolder),
        tr("New Profile"));

    QAction *newAction = m_menuFile->addAction(
        QApplication::style()->standardIcon(QStyle::SP_FileIcon),
        tr("New Action"));

    m_menuFile->addSeparator();

    QAction *reload = m_menuFile->addAction(
        QApplication::style()->standardIcon(QStyle::SP_BrowserReload),
        tr("Reload"));

    QAction *save = m_menuFile->addAction(
        QApplication::style()->standardIcon(QStyle::SP_DialogSaveButton),
        tr("Save"));

    m_menuFile->addSeparator();

    QAction *menuHide = m_menuFile->addAction(
        QApplication::style()->standardIcon(QStyle::SP_DialogCloseButton),
        tr("Hide"));

    QAction *menuExit = m_menuFile->addAction(
        QApplication::style()->standardIcon(QStyle::SP_MediaStop),
        tr("Exit"));

    connect(newProfile, &QAction::triggered, this, &LaunchDeskWindow::newProfileRequested);
    connect(newAction, &QAction::triggered, this, &LaunchDeskWindow::newActionRequested);
    connect(reload, &QAction::triggered, this, &LaunchDeskWindow::reloadRequested);
    connect(save, &QAction::triggered, this, &LaunchDeskWindow::saveRequested);
    connect(menuHide, &QAction::triggered, this, &LaunchDeskWindow::hideRequested);
    connect(menuExit, &QAction::triggered, this, &LaunchDeskWindow::exitRequested);
}

void LaunchDeskWindow::createView() {
    m_menuView = m_menuBar->addMenu(tr("&View"));

    QAction *console = m_menuView->addAction(
        QApplication::style()->standardIcon(QStyle::SP_CommandLink),
        tr("Toggle Console"));

    connect(console, &QAction::triggered, this, &LaunchDeskWindow::toggleConsoleRequested);
}

void LaunchDeskWindow::createCentralUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setContentsMargins(6, 6, 6, 6);
    centralLayout->setSpacing(6);

    m_profileSelect = new QComboBox(centralWidget);

    m_profileSelect->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed
    );

    m_profileStack = new QStackedWidget(centralWidget);

    m_profileSelect->addItem( tr("No Profile"));

    QWidget *stateView = new QWidget(m_profileStack);
    QVBoxLayout *stateLayout = new QVBoxLayout(stateView);
    stateLayout->addStretch();

    QLabel *title = new QLabel(
        tr("No profiles available"),
        stateView);

    title->setAlignment(Qt::AlignCenter);

    QLabel *hint = new QLabel(
        tr("Create your first profile to get started."),
        stateView
    );

    hint->setAlignment(Qt::AlignCenter);
    hint->setWordWrap(true);

    stateLayout->addWidget(title);
    stateLayout->addWidget(hint);
    stateLayout->addStretch();

    m_profileStack->addWidget(stateView); // index 0
    m_profileStack->setCurrentIndex(0);

    connect(m_profileSelect, QOverload<int>::of(&QComboBox::currentIndexChanged), m_profileStack,
            &QStackedWidget::setCurrentIndex);

    centralLayout->addWidget(m_profileSelect);
    centralLayout->addWidget(m_profileStack, 1);
}


void LaunchDeskWindow::createConsoleOverlay() {
    m_consoleOverlay = new QWidget(this);
    m_consoleOverlay->setObjectName("ConsoleOverlay");
    m_consoleOverlay->setAttribute(Qt::WA_StyledBackground, true);
    m_consoleOverlay->setAutoFillBackground(false);
    m_consoleOverlay->hide();

    m_logView = new QPlainTextEdit(m_consoleOverlay);
    m_logView->setReadOnly(true);
    m_logView->setMaximumBlockCount(5000);
    m_logView->setFrameStyle(QFrame::NoFrame);

    QVBoxLayout* layout = new QVBoxLayout(m_consoleOverlay);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(m_logView);

    m_consoleOverlay->resize(width(), 300);
}

void LaunchDeskWindow::createTray() {
    QSystemTrayIcon* tray = new QSystemTrayIcon(this);
    tray->setIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));

    QMenu* menuTray = new QMenu(this);

    QAction* toggleTray = menuTray->addAction(tr("Show"));
    QAction* trayExit = menuTray->addAction(tr("Exit"));

    connect(toggleTray, &QAction::triggered, this, &LaunchDeskWindow::toggleConsoleRequested);

    connect(trayExit, &QAction::triggered, this, &LaunchDeskWindow::exitRequested);

    connect(tray, &QSystemTrayIcon::activated, this, [this](QSystemTrayIcon::ActivationReason reason) {
                if (reason == QSystemTrayIcon::Trigger)
                    emit toggleConsoleRequested();
            });

    tray->setContextMenu(m_menuTray);
    tray->show();

    updateTrayText();
}

void LaunchDeskWindow::updateTrayText() {
    if (!m_toggleTray)
        return;

    if (isVisible()) {
        m_toggleTray->setText(tr("Hide"));
    } else {
        m_toggleTray->setText(tr("Show"));
    }
}
