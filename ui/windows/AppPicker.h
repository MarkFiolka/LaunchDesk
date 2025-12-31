#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QStandardItemModel>

struct AppInfo {
    QString name;
    QString cmd;
    QIcon icon;
};

class AppPicker : public QDialog {
    Q_OBJECT

public:
    explicit AppPicker(QWidget *parent = nullptr);

    QString selectedCommand() const {
        return m_selectedCommand;
    }

signals:
    void appSelected(const QString &cmd);

private:
    QLineEdit *m_searchBar;
    QListView *m_applicationView;
    QStandardItemModel *m_model;
    QList<AppInfo> m_apps;
    QString m_selectedCommand;

    void loadApps();
    void refreshList(const QString &filter);
};
