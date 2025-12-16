#pragma once

#include <QString>
#include <QMap>

struct ContentConfig {
    QString name;
    QString exePath;
    QString type;

    int x = -1;
    int y = -1;
    int width = -1;
    int height = -1;

    QString workingDir;

    QMap<QString, QString> params;
};
