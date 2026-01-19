#include "ProfileController.h"
#include "ProfilePage.h"

#include <QList>
#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QDebug>

ProfileController::ProfileController(QComboBox *combo,
                                     QStackedWidget *stack,
                                     QObject *parent)
    : QObject(parent)
      , m_combo(combo)
      , m_stack(stack) {
}

void ProfileController::addProfile(const QString &name) {
    if (m_combo->count() == 1 && m_combo->itemText(0) == "No Profile") {
        QWidget *stateView = m_stack->widget(0);
        m_combo->removeItem(0);
        m_stack->removeWidget(stateView);
        stateView->deleteLater();
    }

    ProfileData profile;
    profile.name = name;

    const QString baseDir =
            QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    profile.filePath =
            baseDir + "/profiles/" + name.toLower() + ".json";


    m_profiles.append(profile);

    auto *page = new ProfilePage(m_stack);
    m_stack->addWidget(page);
    m_combo->addItem(name);
    m_combo->setCurrentIndex(m_combo->count() - 1);

    saveProfile(m_profiles.last());
}

void ProfileController::addActionToCurrentProfile(
    const QString &name,
    const QString &path
) {
    int index = currentProfileIndex();
    if (index < 0)
        return;

    auto *page = currentProfilePage();
    if (!page)
        return;

    auto &profile = m_profiles[index];

    profile.actions.append({name, path});

    page->addAction(name, path);

    saveProfile(profile);
}

ProfilePage *ProfileController::currentProfilePage() const {
    return qobject_cast<ProfilePage *>(m_stack->currentWidget());
}

int ProfileController::currentProfileIndex() const {
    return m_combo->currentIndex();
}

void ProfileController::loadProfiles() {
    QDir dir("profiles");

    if (!dir.exists())
        return;

    const QStringList files = dir.entryList(
        QStringList() << "*.json",
        QDir::Files
    );

    if (files.isEmpty())
        return;

    if (m_combo->count() == 1 && m_combo->itemText(0) == "No Profile") {
        QWidget *stateView = m_stack->widget(0);
        m_combo->removeItem(0);
        m_stack->removeWidget(stateView);
        stateView->deleteLater();
    }

    for (const QString &fileName: files) {
        const QString filePath = dir.filePath(fileName);

        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly))
            continue;

        const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isObject())
            continue;

        const QJsonObject root = doc.object();

        ProfileData profile;
        profile.name = root["name"].toString();
        profile.filePath = filePath;

        const QJsonArray actions = root["actions"].toArray();
        for (const QJsonValue &v: actions) {
            const QJsonObject obj = v.toObject();
            profile.actions.append({
                obj["name"].toString(),
                obj["path"].toString()
            });
        }

        m_profiles.append(profile);

        auto *page = new ProfilePage(m_stack);
        for (const auto &action: profile.actions) {
            page->addAction(action.name, action.path);
        }

        m_stack->addWidget(page);
        m_combo->addItem(profile.name);
    }

    if (!m_profiles.isEmpty()) {
        m_combo->setCurrentIndex(0);
    }
}


void ProfileController::saveProfile(const ProfileData &profile) {
    const QString dirPath = QFileInfo(profile.filePath).absolutePath();
    QDir().mkpath(dirPath);

    QJsonObject root;
    root["name"] = profile.name;

    QJsonArray actions;
    for (const auto &a: profile.actions) {
        QJsonObject obj;
        obj["name"] = a.name;
        obj["path"] = a.path;
        actions.append(obj);
    }
    root["actions"] = actions;

    QFile file(profile.filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "Failed to write profile:" << file.fileName();
        return;
    }

    file.write(
        QJsonDocument(root).toJson(QJsonDocument::Indented)
    );

    qDebug() << "Profile saved to:"
            << QFileInfo(profile.filePath).absoluteFilePath();
}
