#include "LaunchDesk.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>

#include <QSplitter>
#include <QWidget>

LaunchDesk::LaunchDesk(QWidget *parent)
    : QMainWindow(parent) {
    setFixedSize(900, 550);
    createMenuBar();
    createCentralLayout();
}

void LaunchDesk::createMenuBar() {
    m_menuBar = menuBar();
    m_fileMenu = m_menuBar->addMenu("File");
    m_newProfile = m_fileMenu->addAction("New Profile");
    m_newAction = m_fileMenu->addAction("New Action");
    m_fileMenu->addSeparator();
    m_reloadLaunchDesk = m_fileMenu->addAction("Reload LaunchDesk");
    m_save = m_fileMenu->addAction("Save State");
    m_fileMenu->addSeparator();
    m_exit = m_fileMenu->addAction("Exit");

    connect(m_exit, &QAction::triggered, qApp, &QApplication::quit);

    connect(m_newAction, &QAction::triggered, this, [this]() {
        qDebug() << "New action";
    });
}

void LaunchDesk::createCentralLayout() {
    m_splitter = new QSplitter(Qt::Horizontal, this);

    m_leftPanel = new QWidget(m_splitter);
    m_rightPanel = new QWidget(m_splitter);

    m_leftPanel->setMinimumWidth(200);
    m_leftPanel->setObjectName("test");
    m_rightPanel->setMinimumWidth(400);

    m_splitter->addWidget(m_leftPanel);
    m_splitter->addWidget(m_rightPanel);

    m_splitter->setSizes({250, 650});

    setCentralWidget(m_splitter);
}
