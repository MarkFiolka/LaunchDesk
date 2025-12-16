#pragma once

#include <QWidget>

namespace CommandTemplate
{
    QStringList extractPlaceholders(const QString& cmdTemplate);

    QString applyParamsToCommand(const QString& cmdTemplate,
                                 const QMap<QString, QString>& params);

    bool promptForParams(QWidget* parent,
                         const QStringList& placeholders,
                         QMap<QString, QString>& inOutParams);
}
