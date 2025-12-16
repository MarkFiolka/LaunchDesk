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

    QScrollArea* scrollArea;
    QWidget* scrollContent;

    QVBoxLayout* mainLayout;
    QGridLayout* gridLayout;

    Welcome* previousWindowPtr;
    ConfigManager cfg;
};
