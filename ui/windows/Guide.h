#pragma once

#include <QMainWindow>
#include <QScrollArea>
#include "../../core/ConfigManager.h"

class QVBoxLayout;
class QGridLayout;
class Welcome;
class ConfigManager;

class Guide : public QMainWindow {
    Q_OBJECT

public:
    explicit Guide(Welcome* previousWindow, QWidget *parent = nullptr);

private:
    void createBasicWindow();
    void createAndAdjustContent();

    QScrollArea* m_scrollArea;
    QWidget* m_scrollContent;

    QVBoxLayout* m_mainLayout;
    QGridLayout* m_gridLayout;

    Welcome* m_previousWindow;
    ConfigManager m_configManager;
};
