#pragma once

#include <QList>
#include <QString>

struct ActionData {
    QString name;
    QString path;
};

struct ProfileData {
    QString name;
    QList<ActionData> actions;
    QString filePath;
};
