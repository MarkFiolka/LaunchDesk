#ifdef _WIN32

#include "WindowsConsole.h"
#include <windows.h>
#include <cstdio>
#include <iostream>

namespace WinConsole {

    void attach()
    {
        AllocConsole();

        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$",  "r", stdin);

        std::cout.clear();
        std::cerr.clear();
    }

    void detach()
    {
        FreeConsole();
    }

}

#endif
