#pragma once

#include <QCloseEvent>
#include <QTimer>
#include <QObject>
#include <QCoreApplication>
#include <QWidget>

class QWidget;

class DockController : public QObject {
    Q_OBJECT
public:
    explicit DockController(QWidget* window,
                            QObject* parent = nullptr);

public slots:
    void toggleDock();
    void hideDock();
    void showDock();
    void requestQuit();

protected:
    bool eventFilter(QObject* obj, QEvent* ev) override;

private:
    void setupDockFlags();
    void moveToBottomRight(int margin = 12);

private:
    QWidget* m_window = nullptr;
    bool m_allowClose = false;
    bool m_dockPosSet = false;
};
