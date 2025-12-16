#include "Guide.h"
#include "Welcome.h"

#include "../../core/ConfigManager.h"

#include <QScrollArea>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QStyle>

Guide::Guide(Welcome *previousWindow, QWidget *parent)
        : QMainWindow(parent),
          previousWindowPtr(previousWindow),
          cfg(ResourcePaths::CONFIG)
{
    cfg.load();

    createBasicWindow();
    createAndAdjustContent();
}

void Guide::createBasicWindow()
{
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    setCentralWidget(scrollArea);

    scrollContent = new QWidget(scrollArea);
    scrollArea->setWidget(scrollContent);

    mainLayout = new QVBoxLayout(scrollContent);
    gridLayout = new QGridLayout();

    mainLayout->addSpacing(5);
}

void Guide::createAndAdjustContent()
{
    QPushButton *goBack = new QPushButton("Back");
    goBack->setProperty("type", "back");
    goBack->setFixedSize(60, 25);
    goBack->setAttribute(Qt::WA_StyledBackground, true);

    mainLayout->addWidget(goBack, 0, Qt::AlignLeft);

    connect(goBack, &QPushButton::clicked, [this] {
        if (previousWindowPtr)
            previousWindowPtr->show();
        this->close();
    });

    QList<QString> titles = {
            "Sample 1",
            "Sample 2",
            "Sample 3",
            "Sample 4",
            "Sample 5",
            "Sample 6",
    };

    QList<QString> urls = {
            "https://youtube.com/...",
            "https://youtube.com/...",
            "https://youtube.com/...",
            "https://youtube.com/...",
            "https://youtube.com/...",
            "https://youtube.com/...",
    };

    int row = 0;

    for (int i = 0; i < titles.size(); i++)
    {
        QString fullTitle = QString::number(i + 1) + ". " + titles[i];
        QPushButton *btn = new QPushButton(fullTitle);

        btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

        bool viewed = cfg.getGuideViewed(i + 1);
        btn->setProperty("viewed", viewed);
        btn->setAttribute(Qt::WA_StyledBackground, true);

        gridLayout->addWidget(btn, row, 0, Qt::AlignLeft);
        row++;

        connect(btn, &QPushButton::clicked, [this, btn, i, urls] {
            cfg.setGuideViewed(i + 1, true);
            cfg.save();

            btn->setProperty("viewed", true);

            btn->style()->unpolish(btn);
            btn->style()->polish(btn);

            QDesktopServices::openUrl(QUrl(urls[i]));
        });
    }

    mainLayout->addLayout(gridLayout);
    mainLayout->addStretch();

    adjustSize();
}
