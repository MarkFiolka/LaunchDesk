#include "NewActionWindowDialog.h"

#include <QVBoxLayout>
#include <QLabel>

NewActionWindowDialog::NewActionWindowDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Create new Action in selected Profile");

    m_nameEdit = new QLineEdit(this);
    m_pathEdit = new QLineEdit(this);

    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        this);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Profile name:"));
    layout->addWidget(m_nameEdit);
    layout->addWidget(new QLabel("Action path:"));
    layout->addWidget(m_pathEdit);
    layout->addWidget(buttons);
}

QString NewActionWindowDialog::actionName() const {
    return m_nameEdit->text().trimmed();
}

QString NewActionWindowDialog::actionPath() const {
    return m_pathEdit->text().trimmed();
}
