#include "ProfilePage.h"

#include <QHeaderView>
#include <QVBoxLayout>
#include <QApplication>
#include <QStyle>
#include <QTableWidgetItem>
#include <QFileIconProvider>
#include <QWidget>

ProfilePage::ProfilePage(QWidget* parent)
    : QWidget(parent)
{
    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_table = new QTableWidget(this);

    m_table->setColumnCount(1);
    m_table->setRowCount(0);

    m_table->horizontalHeader()->hide();
    m_table->verticalHeader()->hide();
    m_table->setShowGrid(false);
    m_table->setFrameStyle(QFrame::NoFrame);

    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_table->setIconSize(QSize(24, 24));
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_table->verticalHeader()->setDefaultSectionSize(32);

    layout->addWidget(m_table);
}

void ProfilePage::addAction(const QString& name,
                            const QString& path)
{
    const int row = m_table->rowCount();
    m_table->insertRow(row);

    QIcon icon;
    QFileInfo info(path);

    if (!path.isEmpty() && info.exists()) {
        QFileIconProvider provider;
        icon = provider.icon(info);
    }

    if (icon.isNull()) {
        icon = QApplication::style()->standardIcon(
            QStyle::SP_DesktopIcon
        );
    }

    auto* item = new QTableWidgetItem(name);
    item->setIcon(icon);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    item->setData(Qt::UserRole, path);

    m_table->setItem(row, 0, item);
}

