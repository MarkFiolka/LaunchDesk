#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>

struct AppInfo {
    QString name;
    QString cmd;
};

class AppPickerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AppPickerWindow(QWidget *parent = nullptr);

    QString selectedCommand() const { return m_selectedCommand; }

    signals:
        void appSelected(const QString &cmd);

private:
    QLineEdit     *search;
    QListWidget   *list;

    QList<AppInfo> apps;
    QString        m_selectedCommand;

    void loadApps();
    void refreshList(const QString &filter);
};
