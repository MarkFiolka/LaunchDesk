#pragma once

#include <QMainWindow>
#include <QHash>
#include <QVector>
#include <QProcess>

struct ContentConfig;
class QFrame;
class QPushButton;
class QSplitter;
class QVBoxLayout;
class QProcess;

class LaunchDesk : public QMainWindow
{
    Q_OBJECT
public:
    explicit LaunchDesk(QWidget* parent = nullptr);
    ~LaunchDesk() override;

private slots:
    void onAddProfile();
    void onRemoveProfile();

    void onAddContentToProfile();
    void onPlayAllPressed();

    void onTrackedProcessFinished(int, QProcess::ExitStatus);

private:
    QFrame*       leftPanel    = nullptr;
    QFrame*       rightPanel   = nullptr;
    QSplitter*    splitter     = nullptr;

    QPushButton*  addProfileBtn    = nullptr;
    QPushButton*  removeProfileBtn = nullptr;

    QPushButton*  playAllBtn    = nullptr;
    QPushButton*  addContentBtn = nullptr;

    QVBoxLayout*  profilesLayout = nullptr;
    QVBoxLayout*  contentLayout  = nullptr;

    QString       selectedProfile;
    QString       lastProfile;

    QHash<QProcess*, QString> m_processToFolder;

private:
    QFrame*    createLeftPanel();
    QFrame*    createRightPanel();
    QSplitter* createSplitter(QFrame* left, QFrame* right);

    void saveWindowSettings();
    void restoreWindowSettings();

    void loadProfiles();
    void loadProfileContent(const QString& profileName);
    void addProfileItem(const QString& name);

    void addContentItem(const QString& folderName,
                        const QString& folderPath);


    void launchContentProcess(const QString& folderPath,
                          const ContentConfig& cfg,
                          bool interactive);

    static bool isExePath(const QString& command);
    static bool isUwpCommand(const QString& command);
    static bool isUrlCommand(const QString& command);
};
