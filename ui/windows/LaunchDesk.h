#pragma once

#include <QMainWindow>

class QSplitter;
class QWidget;

class LaunchDesk : public QMainWindow
{
    Q_OBJECT

public:
    explicit LaunchDesk(QWidget* parent = nullptr);

private:
    void createMenuBar();
    QMenuBar* m_menuBar;
    QMenu* m_fileMenu;
    QAction* m_newProfile;
    QAction* m_newAction;
    QAction* m_reloadLaunchDesk;
    QAction* m_save;
    QAction* m_exit;

    void createCentralLayout();
    QSplitter* m_splitter;
    QWidget* m_leftPanel;
    QWidget* m_rightPanel;
};
