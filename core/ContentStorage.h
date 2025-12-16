#pragma once

#include "ContentConfig.h"

namespace ContentStorage
{
    QString configFileForFolder(const QString& folderPath);

    ContentConfig loadContentConfig(const QString& folderPath,
                                    const QString& folderName);

    void saveContentConfig(const QString& folderPath,
                           const ContentConfig& cfg);
}
