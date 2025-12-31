#include "AppPickerWindow.h"

#include <QVBoxLayout>
#include <QListWidgetItem>

#include <windows.h>
#include <shlobj.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

/// AppPickerWindow is creating, scaling and setting a title for a new Window, it will execute loadApps and refreshList to they are "ready",
/// it is also setting a textChanged connection for searchBar as well as itemDoubleClicked connection for applicationList.
AppPickerWindow::AppPickerWindow(QWidget *parent)
    : QDialog(parent) {

    this->setWindowTitle("Installed Apps");
    this->resize(600, 600);

    QVBoxLayout *boxLayout = new QVBoxLayout(this);

    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText("Search...");
    boxLayout->addWidget(m_searchBar);

    m_applicationList = new QListWidget(this);
    boxLayout->addWidget(m_applicationList);

    this->loadApps();
    this->refreshList("");

    connect(m_searchBar, &QLineEdit::textChanged,
            this, [this](const QString &changedText) {
                QString trimmedChangedText = changedText.trimmed();
                refreshList(trimmedChangedText);
            });

    connect(m_applicationList, &QListWidget::itemDoubleClicked,
            this, [this](QListWidgetItem *item) {
                m_selectedCommand = item->data(Qt::UserRole).toString();
                emit appSelected(m_selectedCommand);
                accept();
            });
}

void AppPickerWindow::loadApps() {
    m_apps.clear();
    CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    ComPtr<IShellItem> appsFolder; //get the AppsFolder shell object

    if (FAILED(SHGetKnownFolderItem( //retrieves the virtual folder: shell:AppsFolder
        FOLDERID_AppsFolder, KF_FLAG_DEFAULT, NULL,
        IID_PPV_ARGS(&appsFolder))))
        return;

    ComPtr<IEnumShellItems> enumerator; //Get an enumerator for items inside AppsFolder

    if (FAILED(appsFolder->BindToHandler( //Give me something that can enumerate what’s inside this folder.
        nullptr, BHID_EnumItems, IID_PPV_ARGS(&enumerator))))
        return; //Windows returns an IEnumShellItems interface

    ComPtr<IShellItem> item;

    while (enumerator->Next(1, &item, nullptr) == S_OK) { //loop through all contents of item one at a time

        PWSTR rawName = nullptr; //allocate memory for rawName
        item->GetDisplayName(SIGDN_NORMALDISPLAY, &rawName); //get the user-friendly app name | example: "Calculator"
        QString displayName = QString::fromWCharArray(rawName); //store "Calculator" in displayName
        CoTaskMemFree(rawName); //free memory from rawName again

        PWSTR rawParsing = nullptr; //allocate memory for rawParsing
        item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &rawParsing); //get the real app name | example: "Microsoft.WindowsCalculator_8wekyb3d8bbwe!App"
        QString parsingName = QString::fromWCharArray(rawParsing); //store "Micros..." in parsingName
        CoTaskMemFree(rawParsing); //free memory from rawParsing again

        QString launchCmd = "shell:AppsFolder\\" + parsingName; //create the launchCommand to be used in the list

        m_apps.append({displayName, launchCmd}); //store name and command in AppInfo
    }
}

void AppPickerWindow::refreshList(const QString &filter) {
    m_applicationList->clear(); //clear previous List

    for (const auto &app : m_apps) {
        if (!filter.isEmpty() && !app.name.contains(filter, Qt::CaseInsensitive)) //check if fliter is valid and contains something the list has
            continue;

        auto *listItem = new QListWidgetItem(app.name); //sets the readable name for the name
        listItem->setData(Qt::UserRole, app.cmd); //sets the command | example "shell:AppsFolder\\Microsoft.WindowsCalculator_8wekyb3d8bbwe!App"
        m_applicationList->addItem(listItem); //adds item to the applicationList
    }

}
