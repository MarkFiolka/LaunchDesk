#pragma once

#include "ProfileData.h"

#include <QComboBox>
#include <QDir>
#include <QStackedWidget>
#include <QFileInfo>
#include <QJsonObject>
#include <QStandardPaths>

class QComboBox;
class QStackedWidget;
class ProfilePage;

class ProfileController : public QObject {
    Q_OBJECT
public:
    explicit ProfileController(QComboBox* combo,
                               QStackedWidget* stack,
                               QObject* parent = nullptr);
    void loadProfiles();
    void addProfile(const QString& name);
    void addActionToCurrentProfile(const QString& name,
                                   const QString& path);

    ProfilePage* currentProfilePage() const;

private:
    int currentProfileIndex() const;
    void saveProfile(const ProfileData& profile);

private:
    QComboBox* m_combo = nullptr;
    QStackedWidget* m_stack = nullptr;

    QList<ProfileData> m_profiles;
};
