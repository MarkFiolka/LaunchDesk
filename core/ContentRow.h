#pragma once

#include <QWidget>
#include <QMap>
#include "ContentConfig.h"

class QFormLayout;
class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QVBoxLayout;

class ContentRow : public QWidget
{
    Q_OBJECT

public:
    explicit ContentRow(const QString &folderPath,
                        QWidget *parent = nullptr);

    QString folderPath() const { return m_folderPath; }

    signals:
        void requestLaunch(const QString &folderPath, bool interactive);
    void requestRemove(const QString &folderPath);

private:
    QString       m_folderPath;
    QString       m_folderName;
    ContentConfig m_cfg;

    QVBoxLayout *m_rootLayout = nullptr;
    QWidget     *m_mainRow    = nullptr;
    QWidget     *m_details    = nullptr;

    QPushButton *m_expandBtn    = nullptr;
    QPushButton *m_playBtn      = nullptr;
    QLabel      *m_statusCircle = nullptr;
    QLabel      *m_nameLabel    = nullptr;
    QLabel      *m_pathLabel    = nullptr;
    QPushButton *m_removeBtn    = nullptr;

    QLineEdit   *m_nameEdit      = nullptr;
    QComboBox   *m_typeCombo     = nullptr;

    QLabel      *m_cmdLabel      = nullptr;
    QLineEdit   *m_cmdEdit       = nullptr;
    QPushButton *m_cmdBrowse     = nullptr;
    QPushButton *m_cmdBrowseApps = nullptr;

    QLabel      *m_workingDirLabel  = nullptr;
    QLineEdit   *m_workingDirEdit   = nullptr;
    QPushButton *m_workingDirBrowse = nullptr;

    QPushButton *m_saveBtn = nullptr;

    QWidget* m_paramWidget = nullptr;
    QFormLayout* m_paramLayout = nullptr;
    QMap<QString, QLineEdit*> m_paramEdits;

private:
    void buildUi();
    void updateTypeUi();
    void refreshMainRow();
    void rebuildCmdParamUi(const QString& command);
    void updateStatusCircle();
    bool isConfigValid() const;

    static QLabel *createElidedLabel(const QString &text, int width);
};
