#include "AppPicker.h"

#include <QVBoxLayout>
#include <QListWidgetItem>

#include <windows.h>    // Core Windows API
#include <shlobj.h>     // Shell API
#include <wrl/client.h> // ComPtr smart pointer

#include <shobjidl.h>   // IShellItemImageFactory for icon extraction
#include <wingdi.h>     // GDI functions

using Microsoft::WRL::ComPtr;

/// AppPicker creates a dialog window that lists all installed Windows applications.
/// The UI consists of:
/// - a search bar at the top
/// - an icon grid below (similar to Windows Explorer / Start Menu)
///
/// On creation:
/// - the window layout is built
/// - all apps are loaded from the Windows Shell
/// - the list is initially populated
/// - signal connections for search + double click are registered
AppPicker::AppPicker(QWidget *parent)
    : QDialog(parent)
{
    this->setWindowTitle("Installed Apps");
    this->resize(600, 600);

    QVBoxLayout *boxLayout = new QVBoxLayout(this);

    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText("Search...");
    boxLayout->addWidget(m_searchBar);

    m_applicationView = new QListView(this);
    m_model = new QStandardItemModel(this);

    m_applicationView->setModel(m_model);
    m_applicationView->setViewMode(QListView::IconMode);
    m_applicationView->setResizeMode(QListView::Adjust);
    m_applicationView->setMovement(QListView::Static);
    m_applicationView->setSpacing(12);
    m_applicationView->setIconSize(QSize(32, 32));
    m_applicationView->setGridSize(QSize(64, 64));
    m_applicationView->setWordWrap(true);
    m_applicationView->setUniformItemSizes(true);

    boxLayout->addWidget(m_applicationView);

    this->loadApps();
    this->refreshList("");

    // Re-filter app list whenever search text changes
    connect(m_searchBar, &QLineEdit::textChanged,
            this, [this](const QString &changedText) {
                QString trimmedChangedText = changedText.trimmed();
                refreshList(trimmedChangedText);
            });

    // Handle double-click on an app icon
    connect(m_applicationView, &QListView::doubleClicked,
        this, [this](const QModelIndex &index) {
            m_selectedCommand = index.data(Qt::UserRole).toString();
            emit appSelected(m_selectedCommand);
            accept();
        });
}

/// Extracts the application icon for a given IShellItem.
/// Uses the Windows Shell image factory to retrieve a bitmap,
/// then converts it into a QIcon that Qt can display.
///
/// shellItem : Shell object representing an application
/// iconSize  : Requested icon size in pixels
static QIcon iconFromShellItem(IShellItem* shellItem, int iconSize = 32)
{
    if (!shellItem)
        return {};

    // Interface used by Windows Shell to generate icons/thumbnails
    ComPtr<IShellItemImageFactory> imageFactory;
    if (FAILED(shellItem->QueryInterface(IID_PPV_ARGS(&imageFactory))))
        return {};

    // Requested icon dimensions
    SIZE requestedSize{
        iconSize,
        iconSize
    };

    // Windows bitmap handle that will receive the icon image
    HBITMAP iconBitmapHandle = nullptr;

    HRESULT result = imageFactory->GetImage(
        requestedSize,
        SIIGBF_ICONONLY | SIIGBF_BIGGERSIZEOK, // request icon, allow larger source scaled down
        &iconBitmapHandle
    );

    if (FAILED(result) || !iconBitmapHandle)
        return {}; // icon could not be retrieved

    // Qt image buffer that will receive raw pixel data
    QImage qtImage(
        iconSize,
        iconSize,
        QImage::Format_ARGB32_Premultiplied
    );
    qtImage.fill(Qt::transparent); // ensure transparent background

    // Describe the bitmap format so Windows knows how to copy pixels
    BITMAPINFO bitmapInfo{};
    bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo.bmiHeader.biWidth = iconSize;
    bitmapInfo.bmiHeader.biHeight = -iconSize; // negative height = top-down bitmap
    bitmapInfo.bmiHeader.biPlanes = 1;
    bitmapInfo.bmiHeader.biBitCount = 32;
    bitmapInfo.bmiHeader.biCompression = BI_RGB;

    // Device context required by GetDIBits
    HDC deviceContext = GetDC(nullptr);

    // Copy pixel data from HBITMAP into Qt image buffer
    GetDIBits(
        deviceContext,
        iconBitmapHandle,
        0,
        iconSize,
        qtImage.bits(),
        &bitmapInfo,
        DIB_RGB_COLORS
    );

    ReleaseDC(nullptr, deviceContext); // release device context
    DeleteObject(iconBitmapHandle); //free memory

    // Convert Qt image into a QIcon usable by the view
    return QIcon(QPixmap::fromImage(qtImage));
}

void AppPicker::loadApps()
{
    m_apps.clear(); // clear previously loaded apps
    CoInitializeEx(nullptr, COINIT_MULTITHREADED); // initialize COM for Shell APIs

    ComPtr<IShellItem> appsFolder; // Shell object representing "shell:AppsFolder"

    // Retrieve the virtual AppsFolder (contains all installed apps)
    if (FAILED(SHGetKnownFolderItem(
        FOLDERID_AppsFolder,
        KF_FLAG_DEFAULT,
        NULL,
        IID_PPV_ARGS(&appsFolder))))
        return;

    ComPtr<IEnumShellItems> enumerator; // enumerator for items inside AppsFolder

    // Bind AppsFolder to an enumerator interface
    if (FAILED(appsFolder->BindToHandler(
        nullptr,
        BHID_EnumItems,
        IID_PPV_ARGS(&enumerator))))
        return;

    ComPtr<IShellItem> item; // individual app entry

    // Iterate over all applications one by one
    while (enumerator->Next(1, &item, nullptr) == S_OK)
    {
        PWSTR rawName = nullptr;
        item->GetDisplayName(SIGDN_NORMALDISPLAY, &rawName); // human-readable name
        QString displayName = QString::fromWCharArray(rawName);
        CoTaskMemFree(rawName); // free Shell-allocated memory

        PWSTR rawParsing = nullptr;
        item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &rawParsing); // unique app identifier
        QString parsingName = QString::fromWCharArray(rawParsing);
        CoTaskMemFree(rawParsing);

        QString launchCmd = "shell:AppsFolder\\" + parsingName; // command used to launch app

        QIcon icon = iconFromShellItem(item.Get(), 64); // extract real Windows icon

        m_apps.append({displayName, launchCmd, icon}); // store app info
    }
}

void AppPicker::refreshList(const QString &filter)
{
    m_model->clear(); // remove all currently displayed items

    for (const auto &app : m_apps)
    {
        // Skip apps that do not match the search filter
        if (!filter.isEmpty() &&
            !app.name.contains(filter, Qt::CaseInsensitive))
            continue;

        auto *modelItem = new QStandardItem();
        modelItem->setText(app.name); // text shown under icon
        modelItem->setToolTip(app.name); // tooltip on hover
        modelItem->setIcon(app.icon); // Windows app icon
        modelItem->setData(app.cmd, Qt::UserRole); // store launch command

        m_model->appendRow(modelItem); // add item to the model
    }
}
