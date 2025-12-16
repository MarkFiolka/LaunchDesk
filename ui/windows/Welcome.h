#pragma once
#include <QMainWindow>

class QVBoxLayout;

class Welcome : public QMainWindow {
    Q_OBJECT

public:
    explicit Welcome(QWidget *parent = nullptr);

private:
    void createBasicWindow();

    void createAndAdjustContent();

    QVBoxLayout *welcomeScreen_Layout = nullptr;
};
