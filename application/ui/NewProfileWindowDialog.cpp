#include "NewProfileWindowDialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QPushButton>

NewProfileWindowDialog::NewProfileWindowDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle(tr("Create new Profile"));

    m_nameEdit = new QLineEdit(this);

    auto *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        this);

    buttons->button(QDialogButtonBox::Ok)->setEnabled(false);

    connect(m_nameEdit, &QLineEdit::textChanged, this, [buttons](const QString &text) {
        buttons->button(QDialogButtonBox::Ok)->setEnabled(!text.trimmed().isEmpty());
    });

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Profile name:")));
    layout->addWidget(m_nameEdit);
    layout->addWidget(buttons);
}

QString NewProfileWindowDialog::profileName() const {
    return m_nameEdit->text().trimmed();
}
