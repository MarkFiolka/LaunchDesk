#pragma once

#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>

class NewProfileWindowDialog : public QDialog {
    Q_OBJECT
public:
    explicit NewProfileWindowDialog(QWidget *parent = nullptr);
    QString profileName() const;

private:
    QLineEdit* m_nameEdit;
};
