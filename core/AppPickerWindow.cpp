#include "AppPickerWindow.h"

#include <QVBoxLayout>
#include <QListWidgetItem>

#include <windows.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

AppPickerWindow::AppPickerWindow(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Installed Apps");
    resize(600, 600);

    setObjectName("AppPickerWindow");

    QVBoxLayout *layout = new QVBoxLayout(this);

    search = new QLineEdit();
    search->setObjectName("SearchField");
    search->setPlaceholderText("Search...");
    layout->addWidget(search);

    list = new QListWidget();
    list->setObjectName("AppList");
    list->setIconSize(QSize(32, 32));
    layout->addWidget(list);

    loadApps();
    refreshList("");

    connect(search, &QLineEdit::textChanged,
            this, [this](const QString &t){ refreshList(t.trimmed()); });

    connect(list, &QListWidget::itemDoubleClicked,
            this, [this](QListWidgetItem *item)
    {
        m_selectedCommand = item->data(Qt::UserRole).toString();
        emit appSelected(m_selectedCommand);
        accept();
    });
}


void AppPickerWindow::loadApps()
{
    apps.clear();
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    ComPtr<IShellItem> appsFolder;

    if (FAILED(SHGetKnownFolderItem(
                   FOLDERID_AppsFolder, KF_FLAG_DEFAULT, NULL,
                   IID_PPV_ARGS(&appsFolder))))
        return;

    ComPtr<IEnumShellItems> e;
    if (FAILED(appsFolder->BindToHandler(
                   nullptr, BHID_EnumItems, IID_PPV_ARGS(&e))))
        return;

    ComPtr<IShellItem> item;
    while (e->Next(1, &item, nullptr) == S_OK)
    {
        PWSTR rawName = nullptr;
        item->GetDisplayName(SIGDN_NORMALDISPLAY, &rawName);
        QString displayName = QString::fromWCharArray(rawName);
        CoTaskMemFree(rawName);

        PWSTR rawParsing = nullptr;
        item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &rawParsing);
        QString parsingName = QString::fromWCharArray(rawParsing);
        CoTaskMemFree(rawParsing);

        QString launchCmd = "shell:AppsFolder\\" + parsingName;

        apps.append({ displayName, launchCmd });
    }
}

void AppPickerWindow::refreshList(const QString &filter)
{
    list->clear();

    for (const auto &a : apps)
    {
        if (!filter.isEmpty() &&
            !a.name.contains(filter, Qt::CaseInsensitive))
            continue;

        auto *i = new QListWidgetItem(a.name);
        i->setData(Qt::UserRole, a.cmd);
        list->addItem(i);
    }
}
