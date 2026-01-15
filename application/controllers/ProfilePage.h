#pragma once
#include <QWidget>

class QTableWidget;

class ProfilePage : public QWidget {
    Q_OBJECT
public:
    explicit ProfilePage(QWidget* parent = nullptr);

    void addAction(const QString& name,
                   const QString& path);

private:
    QTableWidget* m_table = nullptr;
};
