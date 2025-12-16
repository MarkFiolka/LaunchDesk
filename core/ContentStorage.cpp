#include "ContentStorage.h"

#include <QFile>
#include <QJsonObject>

namespace ContentStorage
{
    QString configFileForFolder(const QString& folderPath)
    {
        return folderPath + "/config.json";
    }

    ContentConfig loadContentConfig(const QString& folderPath,
                                    const QString& folderName)
    {
        ContentConfig cfg;
        cfg.name         = folderName;
        cfg.exePath      = "";
        cfg.type         = "App";
        cfg.workingDir    = "";
        cfg.params.clear();

        QFile file(configFileForFolder(folderPath));

        if (!file.exists()) {
            if (file.open(QIODevice::WriteOnly)) {
                QJsonObject obj;
                obj["name"]          = cfg.name;
                obj["exePath"]       = "";
                obj["type"]          = cfg.type;
                obj["workingDir"]    = cfg.workingDir;
                obj["params"]        = QJsonObject();
                file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
            }
            return cfg;
        }

        if (!file.open(QIODevice::ReadOnly))
            return cfg;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (!doc.isObject())
            return cfg;

        QJsonObject obj = doc.object();

        cfg.name    = obj.value("name").toString(folderName);
        cfg.exePath = obj.value("exePath").toString("");
        cfg.type    = obj.value("type").toString("App");
        cfg.workingDir    = obj.value("workingDir").toString("");

        cfg.params.clear();
        if (obj.contains("params") && obj.value("params").isObject()) {
            QJsonObject pObj = obj.value("params").toObject();
            const auto keys  = pObj.keys();
            for (const QString& k : keys) {
                cfg.params.insert(k, pObj.value(k).toString(""));
            }
        }

        return cfg;
    }

    void saveContentConfig(const QString& folderPath,
                           const ContentConfig& cfg)
    {
        QFile file(configFileForFolder(folderPath));
        if (!file.open(QIODevice::WriteOnly))
            return;

        QJsonObject obj;
        obj["name"]    = cfg.name;
        obj["exePath"] = cfg.exePath;
        obj["type"]    = cfg.type;
        obj["workingDir"]    = cfg.workingDir;

        QJsonObject pObj;
        for (auto it = cfg.params.constBegin(); it != cfg.params.constEnd(); ++it) {
            pObj[it.key()] = it.value();
        }
        obj["params"] = pObj;

        file.write(QJsonDocument(obj).toJson(QJsonDocument::Indented));
    }
}
