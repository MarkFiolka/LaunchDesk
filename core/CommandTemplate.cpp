#include "CommandTemplate.h"

#include <QRegularExpression>
#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>

namespace CommandTemplate
{
    QStringList extractPlaceholders(const QString& cmdTemplate)
    {
        QStringList result;

        QRegularExpression re(R"(<([A-Za-z0-9_]+)>)");
        auto it = re.globalMatch(cmdTemplate);

        while (it.hasNext()) {
            const auto m = it.next();
            const QString name = m.captured(1);

            if (!result.contains(name))
                result.append(name);
        }

        return result;
    }

    QString applyParamsToCommand(const QString& cmdTemplate,
                                 const QMap<QString, QString>& params)
    {
        QString cmd = cmdTemplate;

        for (auto it = params.constBegin(); it != params.constEnd(); ++it) {
            cmd.replace("<" + it.key() + ">", it.value());
        }

        return cmd;
    }

    bool promptForParams(QWidget* parent,
                         const QStringList& placeholders,
                         QMap<QString, QString>& inOutParams)
    {
        if (placeholders.isEmpty())
            return true;

        QDialog dlg(parent);
        dlg.setWindowTitle("Command Parameters");

        auto* vbox = new QVBoxLayout(&dlg);
        auto* form = new QFormLayout();

        QMap<QString, QLineEdit*> edits;

        for (const QString& ph : placeholders) {
            auto* edit = new QLineEdit(&dlg);
            edit->setText(inOutParams.value(ph));
            edits.insert(ph, edit);
            form->addRow(ph + ":", edit);
        }

        vbox->addLayout(form);

        auto* buttons =
            new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                                 Qt::Horizontal,
                                 &dlg);

        vbox->addWidget(buttons);

        QObject::connect(buttons, &QDialogButtonBox::accepted,
                         &dlg, &QDialog::accept);
        QObject::connect(buttons, &QDialogButtonBox::rejected,
                         &dlg, &QDialog::reject);

        if (dlg.exec() != QDialog::Accepted)
            return false;

        for (auto it = edits.begin(); it != edits.end(); ++it) {
            inOutParams[it.key()] = it.value()->text().trimmed();
        }

        return true;
    }
}
