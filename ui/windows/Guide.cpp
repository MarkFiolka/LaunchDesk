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
          m_previousWindow(previousWindow),
          m_configManager(ResourcePaths::CONFIG)
{
    m_configManager.load();

    this->createBasicWindow();
    this->createAndAdjustContent();
}

void Guide::createBasicWindow()
{
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);

    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    this->setCentralWidget(m_scrollArea);

    m_scrollContent = new QWidget(m_scrollArea);
    m_scrollArea->setWidget(m_scrollContent);

    m_mainLayout = new QVBoxLayout(m_scrollContent);
    m_gridLayout = new QGridLayout();

    m_mainLayout->addSpacing(5);
}

void Guide::createAndAdjustContent()
{
    QPushButton *goBack = new QPushButton("Back");
    goBack->setProperty("type", "back");
    goBack->setFixedSize(60, 25);
    goBack->setAttribute(Qt::WA_StyledBackground, true);

    m_mainLayout->addWidget(goBack, 0, Qt::AlignLeft);

    connect(goBack, &QPushButton::clicked, [this] {
        if (m_previousWindow)
            m_previousWindow->show();
        this->close();
    });

    QList<QString> titles = { //add titles in the future
            "Sample 1",
            "Sample 2",
            "Sample 3",
            "Sample 4",
            "Sample 5",
            "Sample 6",
    };

    QList<QString> urls = {
            "https://youtube.com/...", //add links in the future
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

        bool viewed = m_configManager.getGuideViewed(i + 1);
        btn->setProperty("viewed", viewed);
        btn->setAttribute(Qt::WA_StyledBackground, true);

        m_gridLayout->addWidget(btn, row, 0, Qt::AlignLeft);
        row++;

        connect(btn, &QPushButton::clicked, [this, btn, i, urls] {
            m_configManager.setGuideViewed(i + 1, true);
            m_configManager.save();

            btn->setProperty("viewed", true);

            btn->style()->unpolish(btn);
            btn->style()->polish(btn);

            QDesktopServices::openUrl(QUrl(urls[i]));
        });
    }

    m_mainLayout->addLayout(m_gridLayout);
    m_mainLayout->addStretch();

    this->adjustSize();
}
