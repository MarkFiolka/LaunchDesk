#include "LaunchDesk.h"

#include <QVBoxLayout>
#include <QInputDialog>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QApplication>
#include <QSettings>
#include <QTimer>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QSplitter>

#include "../../core/ContentRow.h"
#include "../helpers/UIHelpers.h"
#include "../../core/ContentStorage.h"
#include "../../core/CommandTemplate.h"
#include "../../core/WindowPlacement.h"

bool LaunchDesk::isExePath(const QString &command) {
    QFileInfo fi(command);
    return fi.exists() && fi.isFile() &&
           command.endsWith(".exe", Qt::CaseInsensitive);
}

bool LaunchDesk::isUwpCommand(const QString &command) {
    return command.startsWith("shell:AppsFolder\\", Qt::CaseInsensitive);
}

bool LaunchDesk::isUrlCommand(const QString &command) {
    return command.startsWith("http://", Qt::CaseInsensitive) ||
           command.startsWith("https://", Qt::CaseInsensitive);
}

LaunchDesk::LaunchDesk(QWidget *parent)
    : QMainWindow(parent) {
    setFixedSize(900, 550);

    leftPanel = createLeftPanel();
    rightPanel = createRightPanel();
    splitter = createSplitter(leftPanel, rightPanel);

    QWidget *c = new QWidget(this);
    QVBoxLayout *root = new QVBoxLayout(c);
    root->setContentsMargins(0, 0, 0, 0);
    root->addWidget(splitter);
    setCentralWidget(c);

    restoreWindowSettings();
    loadProfiles();

    if (!lastProfile.isEmpty()) {
        selectedProfile = lastProfile;
        loadProfileContent(selectedProfile);
    }
}

LaunchDesk::~LaunchDesk() {
    saveWindowSettings();
}

void LaunchDesk::saveWindowSettings() {
    QSettings s("LaunchDesk", "LaunchDeskApp");
    s.setValue("geometry", saveGeometry());
    s.setValue("splitter", splitter->saveState());
    s.setValue("lastProfile", selectedProfile);
}

void LaunchDesk::restoreWindowSettings() {
    QSettings s("LaunchDesk", "LaunchDeskApp");

    if (s.contains("geometry"))
        restoreGeometry(s.value("geometry").toByteArray());

    if (s.contains("splitter"))
        splitter->restoreState(s.value("splitter").toByteArray());

    lastProfile = s.value("lastProfile").toString();
}

QFrame *LaunchDesk::createLeftPanel() {
    QFrame *panel = new QFrame();
    QVBoxLayout *layout = new QVBoxLayout(panel);

    QHBoxLayout *bar = new QHBoxLayout();
    addProfileBtn = UIHelpers::createIconButton(":/icons/Add_Button.svg");
    removeProfileBtn = UIHelpers::createIconButton(":/icons/Remove_Button.svg");

    bar->addWidget(addProfileBtn);
    bar->addWidget(removeProfileBtn);
    bar->addStretch();
    layout->addLayout(bar);

    profilesLayout = new QVBoxLayout();
    layout->addLayout(profilesLayout);
    layout->addStretch();

    connect(addProfileBtn, &QPushButton::clicked, this, &LaunchDesk::onAddProfile);
    connect(removeProfileBtn, &QPushButton::clicked, this, &LaunchDesk::onRemoveProfile);

    return panel;
}

QFrame *LaunchDesk::createRightPanel() {
    QFrame *panel = new QFrame();
    QVBoxLayout *layout = new QVBoxLayout(panel);

    QHBoxLayout *bar = new QHBoxLayout();
    playAllBtn = UIHelpers::createIconButton(":/icons/Play_Button.svg");
    addContentBtn = UIHelpers::createIconButton(":/icons/Add_Button.svg");

    playAllBtn->setFixedSize(32, 32);
    addContentBtn->setFixedSize(32, 32);

    bar->addWidget(playAllBtn);
    bar->addWidget(addContentBtn);
    bar->addStretch();
    layout->addLayout(bar);

    contentLayout = new QVBoxLayout();
    layout->addLayout(contentLayout);
    layout->addStretch();

    connect(addContentBtn, &QPushButton::clicked, this, &LaunchDesk::onAddContentToProfile);
    connect(playAllBtn, &QPushButton::clicked, this, &LaunchDesk::onPlayAllPressed);

    return panel;
}

QSplitter *LaunchDesk::createSplitter(QFrame *left, QFrame *right) {
    QSplitter *s = new QSplitter(Qt::Horizontal);
    s->setHandleWidth(5);
    s->addWidget(left);
    s->addWidget(right);
    s->setSizes({150, 750});
    return s;
}

void LaunchDesk::onAddProfile() {
    QString name = QInputDialog::getText(this, "Create profile", "Enter a Profile name:");
    if (name.trimmed().isEmpty()) return;

    QString root = QCoreApplication::applicationDirPath() + "/profiles";
    QDir().mkpath(root);

    QString folder = root + "/" + name;
    if (QDir(folder).exists()) {
        QMessageBox::warning(this, "Exists", "Profile already exists.");
        return;
    }

    QDir().mkdir(folder);
    addProfileItem(name);

    selectedProfile = name;
    loadProfileContent(name);
}

void LaunchDesk::onRemoveProfile() {
    if (selectedProfile.isEmpty()) return;

    QString folder = QCoreApplication::applicationDirPath() + "/profiles/" + selectedProfile;
    QDir(folder).removeRecursively();

    selectedProfile.clear();
    loadProfiles();

    while (QLayoutItem *i = contentLayout->takeAt(0))
        delete i->widget();
}

void LaunchDesk::addProfileItem(const QString &name) {
    QPushButton *btn = new QPushButton(name);
    btn->setFixedHeight(32);
    btn->setStyleSheet("text-align: left; padding-left: 8px;");
    profilesLayout->addWidget(btn);

    connect(btn, &QPushButton::clicked, this, [this, name]() {
        selectedProfile = name;
        loadProfileContent(name);
    });
}

void LaunchDesk::loadProfiles() {
    while (QLayoutItem *i = profilesLayout->takeAt(0))
        delete i->widget();

    QString base = QCoreApplication::applicationDirPath() + "/profiles";
    QDir dir(base);

    if (!dir.exists()) return;

    for (QString p: dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot))
        addProfileItem(p);
}

void LaunchDesk::loadProfileContent(const QString &profileName) {
    while (QLayoutItem *i = contentLayout->takeAt(0))
        delete i->widget();

    QString base = QCoreApplication::applicationDirPath() + "/profiles/" + profileName;
    QDir dir(base);

    for (QString f: dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString folderPath = dir.absoluteFilePath(f);
        addContentItem(f, folderPath);
    }
}

void LaunchDesk::onAddContentToProfile() {
    if (selectedProfile.isEmpty()) return;

    QString name = QInputDialog::getText(this, "Add content", "Folder name:");
    if (name.trimmed().isEmpty()) return;

    QString base = QCoreApplication::applicationDirPath() + "/profiles/" + selectedProfile;
    QString folder = base + "/" + name;

    if (QDir(folder).exists()) {
        QMessageBox::warning(this, "Exists", "Content already exists.");
        return;
    }

    QDir().mkdir(folder);

    ContentConfig cfg;
    cfg.name = name;
    cfg.exePath = "";
    cfg.type = "App";
    ContentStorage::saveContentConfig(folder, cfg);

    addContentItem(name, folder);
}

void LaunchDesk::addContentItem(const QString &folderName,
                                const QString &folderPath) {
    Q_UNUSED(folderName)

    auto *row = new ContentRow(folderPath, this);
    contentLayout->addWidget(row);

    connect(row, &ContentRow::requestLaunch,
            this, [this](const QString &folderPath, bool interactive) {
                QString folderName = QFileInfo(folderPath).fileName();
                ContentConfig cfg = ContentStorage::loadContentConfig(folderPath, folderName);
                if (cfg.exePath.isEmpty()) {
                    QMessageBox::warning(this, "No command", "No command/executable set.");
                    return;
                }
                launchContentProcess(folderPath, cfg, false);
            });

    connect(row, &ContentRow::requestRemove,
            this, [this, row](const QString &folderPath) {
                if (QMessageBox::question(this, "Remove",
                                          "Delete this content including folder?",
                                          QMessageBox::Yes | QMessageBox::No)
                    == QMessageBox::Yes) {
                    QDir(folderPath).removeRecursively();
                    contentLayout->removeWidget(row);
                    row->deleteLater();
                }
            });
}

void LaunchDesk::onPlayAllPressed() {
    for (int i = 0; i < contentLayout->count(); ++i) {
        auto *row = qobject_cast<ContentRow *>(contentLayout->itemAt(i)->widget());
        if (!row)
            continue;

        QString folderPath = row->folderPath();
        QString folderName = QFileInfo(folderPath).fileName();
        ContentConfig cfg = ContentStorage::loadContentConfig(folderPath, folderName);

        if (cfg.exePath.isEmpty())
            continue;

        launchContentProcess(folderPath, cfg, false);
    }
}


void LaunchDesk::launchContentProcess(const QString &folderPath,
                                      const ContentConfig &cfg,
                                      bool interactive) {
    QString cmd = cfg.exePath.trimmed();
    if (cmd.isEmpty())
        return;

    const QString typeLower = cfg.type.toLower();

    if (isUrlCommand(cmd)) {
        QDesktopServices::openUrl(QUrl(cmd));
        return;
    }

    if (isUwpCommand(cmd)) {
        QProcess::startDetached("explorer.exe", {cmd});
        return;
    }

    if (typeLower == "terminal") {
        ContentConfig cfgCopy = cfg;

        QString finalCmd = cfgCopy.exePath;
        if (!cfgCopy.params.isEmpty()) {
            finalCmd = CommandTemplate::applyParamsToCommand(
                cfgCopy.exePath, cfgCopy.params);
        }

#ifdef _WIN32
        QString program = "powershell.exe";
        QStringList args = {
            "-NoProfile",
            "-ExecutionPolicy", "Bypass",
            "-Command", finalCmd
        };
#else
        QString program = "/bin/sh";
        QStringList args = {"-c", finalCmd};
#endif

        QProcess *proc = new QProcess(this);
        proc->setProgram(program);
        proc->setArguments(args);

        if (!cfgCopy.workingDir.isEmpty())
            proc->setWorkingDirectory(cfgCopy.workingDir);

        connect(proc,
                QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this,
                &LaunchDesk::onTrackedProcessFinished);

        m_processToFolder[proc] = folderPath;

        proc->start();
        if (!proc->waitForStarted(5000)) {
            QMessageBox::warning(
                this,
                "Error",
                "Failed to start terminal command:\n" + finalCmd
            );
            m_processToFolder.remove(proc);
            proc->deleteLater();
        }

        return;
    }

    if (isExePath(cmd)) {
        QProcess *proc = new QProcess(this);
        proc->setProgram(cmd);
        proc->setWorkingDirectory(
            cfg.workingDir.isEmpty()
                ? QFileInfo(cmd).absolutePath()
                : cfg.workingDir
        );

        connect(proc,
                QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                this,
                &LaunchDesk::onTrackedProcessFinished);

        m_processToFolder[proc] = folderPath;

        proc->start();

        if (!proc->waitForStarted(5000)) {
            QMessageBox::warning(this, "Error", "Failed to start: " + cmd);
            m_processToFolder.remove(proc);
            proc->deleteLater();
            return;
        }

        quint64 pid = proc->processId();
        if (cfg.x >= 0 && cfg.y >= 0 && cfg.width > 0 && cfg.height > 0) {
            QTimer::singleShot(
                1000, this,
                [pid, cfg]() {
                    WindowPlacement::applySavedWindowPlacement(pid, cfg);
                }
            );
        }
        return;
    }

#ifdef _WIN32
    QProcess::startDetached("cmd.exe", {"/C", cmd});
#else
    QProcess::startDetached(cmd);
#endif
}

void LaunchDesk::onTrackedProcessFinished(int, QProcess::ExitStatus) {
    QProcess *proc = qobject_cast<QProcess *>(sender());
    if (!proc)
        return;

    QString folderPath = m_processToFolder.take(proc);

    quint64 pid = proc->processId();
    if (!folderPath.isEmpty())
        WindowPlacement::saveWindowPlacementForProcess(pid, folderPath);

    proc->deleteLater();
}
