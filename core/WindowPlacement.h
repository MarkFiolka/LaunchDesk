#pragma once

#include "ContentConfig.h"

namespace WindowPlacement
{
    void applySavedWindowPlacement(quint64 pid, const ContentConfig& cfg);

    void saveWindowPlacementForProcess(quint64 pid, const QString& folderPath);
}
