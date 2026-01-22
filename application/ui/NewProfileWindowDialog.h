#pragma once

#include <QMainWindow>
#include <QDialog>

class QLineEdit;

class NewProfileWindowDialog : public QDialog {
    Q_OBJECT
public:
    explicit NewProfileWindowDialog(QWidget *parent = nullptr);
    QString profileName() const;

private:
    QLineEdit* m_nameEdit = nullptr;
};
