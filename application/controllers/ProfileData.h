#pragma once
#include <QString>
#include <QList>

struct ActionData {
    QString name;
    QString path;
};

struct ProfileData {
    QString name;
    QList<ActionData> actions;
    QString filePath;
};
