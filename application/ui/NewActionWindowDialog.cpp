#include "NewActionWindowDialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>

NewActionWindowDialog::NewActionWindowDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Create new Action in selected Profile"));

    m_nameEdit = new QLineEdit(this);
    m_pathEdit = new QLineEdit(this);

    auto* buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        this);

    QPushButton* okButton = buttons->button(QDialogButtonBox::Ok);

    okButton->setEnabled(false);

    auto updateOkButton = [this, okButton]() {
        bool nameOk = !m_nameEdit->text().trimmed().isEmpty();
        bool pathOk = !m_pathEdit->text().trimmed().isEmpty();

        okButton->setEnabled(nameOk && pathOk);
    };

    connect(m_nameEdit, &QLineEdit::textChanged, this, updateOkButton);
    connect(m_pathEdit, &QLineEdit::textChanged, this, updateOkButton);

    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(tr("Profile name:")));
    layout->addWidget(m_nameEdit);
    layout->addWidget(new QLabel(tr("Action path:")));
    layout->addWidget(m_pathEdit);
    layout->addWidget(buttons);
}

QString NewActionWindowDialog::actionName() const {
    return m_nameEdit->text().trimmed();
}

QString NewActionWindowDialog::actionPath() const {
    return m_pathEdit->text().trimmed();
}
