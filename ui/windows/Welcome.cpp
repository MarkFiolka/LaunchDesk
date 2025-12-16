#include "Welcome.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QWidget>

#include "Guide.h"
#include "LaunchDesk.h"
#include "../../core/ConfigManager.h"

QCheckBox* checkboxPtr = nullptr;

Welcome::Welcome(QWidget *parent)
    : QMainWindow(parent)
{
    createBasicWindow();
    createAndAdjustContent();
}

void Welcome::createBasicWindow() {
    setFixedSize(400, 300);

    auto *center = new QWidget(this);
    setCentralWidget(center);

    welcomeScreen_Layout = new QVBoxLayout(center);
    welcomeScreen_Layout->addStretch();
}

void Welcome::createAndAdjustContent() {
    auto *startButton = new QPushButton("Start LaunchDesk");
    auto *guideButton = new QPushButton("View Guides");
    auto *checkBox = new QCheckBox("Do not show this again");

    checkboxPtr = checkBox;

    connect(startButton, &QPushButton::clicked, [this] {
        ConfigManager cfg;
        cfg.load();

        bool dontShow = checkboxPtr->isChecked();
        cfg.setBool("showWelcome", !dontShow);

        cfg.save();

        auto *startLaunchDesk = new LaunchDesk();
        startLaunchDesk->show();
        this->close();
    });

    connect(guideButton, &QPushButton::clicked, [this] {
        auto *guideWindow = new Guide(this);
        guideWindow->show();
        this->hide();
    });

    startButton->setFixedSize(200, 30);
    guideButton->setFixedSize(200, 30);

    welcomeScreen_Layout->addWidget(startButton, 0, Qt::AlignHCenter);
    welcomeScreen_Layout->addWidget(guideButton, 0, Qt::AlignHCenter);
    welcomeScreen_Layout->addWidget(checkBox, 0, Qt::AlignHCenter);

    welcomeScreen_Layout->setSpacing(15);
    welcomeScreen_Layout->addStretch();
}
