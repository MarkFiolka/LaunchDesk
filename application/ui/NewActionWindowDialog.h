#pragma once

#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>

class NewActionWindowDialog : public QDialog {
    Q_OBJECT
public:
    explicit NewActionWindowDialog(QWidget *parent = nullptr);
    QString actionName() const;
    QString actionPath() const;

private:
    QLineEdit* m_nameEdit;
    QLineEdit* m_pathEdit;
};
