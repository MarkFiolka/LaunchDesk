#include "NewProfileWindowDialog.h"

#include <QVBoxLayout>
#include <QLabel>

NewProfileWindowDialog::NewProfileWindowDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Create new Profile");

    m_nameEdit = new QLineEdit(this);

    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        this);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Profile name:"));
    layout->addWidget(m_nameEdit);
    layout->addWidget(buttons);
}

QString NewProfileWindowDialog::profileName() const {
    return m_nameEdit->text().trimmed();
}
