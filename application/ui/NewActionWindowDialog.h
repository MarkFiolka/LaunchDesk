#pragma once

#include <QMainWindow>
#include <QDialog>

class QLineEdit;

class NewActionWindowDialog : public QDialog {
    Q_OBJECT
public:
    explicit NewActionWindowDialog(QWidget *parent = nullptr);
    QString actionName() const;
    QString actionPath() const;

private:
    QLineEdit* m_nameEdit = nullptr;
    QLineEdit* m_pathEdit = nullptr;
};
