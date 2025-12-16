#include "WindowPlacement.h"

#include <QFileInfo>

#include "ContentStorage.h"

#ifdef _WIN32
    #include <windows.h>
    #include <tlhelp32.h>
#endif

namespace WindowPlacement
{
#ifdef _WIN32

    struct WindowSearchContext {
        DWORD pid;
        HWND  hwnd;
    };

    static BOOL CALLBACK EnumWindowsProcFindMain(HWND hwnd, LPARAM lParam)
    {
        auto* ctx = reinterpret_cast<WindowSearchContext*>(lParam);
        DWORD winPid = 0;
        GetWindowThreadProcessId(hwnd, &winPid);

        if (winPid != ctx->pid)
            return TRUE;

        if (!IsWindowVisible(hwnd))
            return TRUE;

        if (GetWindow(hwnd, GW_OWNER) != nullptr)
            return TRUE;

        ctx->hwnd = hwnd;
        return FALSE;
    }

    static HWND findMainWindowByPid(DWORD pid)
    {
        WindowSearchContext ctx;
        ctx.pid  = pid;
        ctx.hwnd = nullptr;
        EnumWindows(EnumWindowsProcFindMain, reinterpret_cast<LPARAM>(&ctx));
        return ctx.hwnd;
    }

#endif

    void applySavedWindowPlacement(quint64 pid, const ContentConfig& cfg)
    {
#ifdef _WIN32
        if (cfg.x < 0 || cfg.y < 0 || cfg.width <= 0 || cfg.height <= 0)
            return;

        HWND hwnd = findMainWindowByPid(static_cast<DWORD>(pid));
        if (!hwnd)
            return;

        MoveWindow(hwnd, cfg.x, cfg.y, cfg.width, cfg.height, TRUE);
#else
        Q_UNUSED(pid)
        Q_UNUSED(cfg)
#endif
    }

    void saveWindowPlacementForProcess(quint64 pid, const QString& folderPath)
    {
#ifdef _WIN32
        HWND hwnd = findMainWindowByPid(static_cast<DWORD>(pid));
        if (!hwnd)
            return;

        RECT rc{};
        if (!GetWindowRect(hwnd, &rc))
            return;

        QString folderName = QFileInfo(folderPath).fileName();
        ContentConfig cfg  = ContentStorage::loadContentConfig(folderPath, folderName);

        cfg.x      = rc.left;
        cfg.y      = rc.top;
        cfg.width  = rc.right  - rc.left;
        cfg.height = rc.bottom - rc.top;

        ContentStorage::saveContentConfig(folderPath, cfg);
#else
        Q_UNUSED(pid)
        Q_UNUSED(folderPath)
#endif
    }
}
