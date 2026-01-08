#pragma once
#include <QObject>

class QWidget;
class QDockWidget;

class DockController : public QObject {
    Q_OBJECT

public:
    explicit DockController(QWidget *window,
                            QDockWidget *consoleDock,
                            QObject *parent = nullptr);

public slots:
    void toggleDock();

    void hideDock();

    void showDock();

    void toggleConsole();

    void requestQuit();

protected:
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    void setupDockFlags();
    void moveToBottomRight(int margin = 12);

private:
    QWidget *m_window = nullptr;
    QDockWidget *m_consoleDock = nullptr;
    bool m_allowClose = false;
    bool m_dockPosSet = false;
};
