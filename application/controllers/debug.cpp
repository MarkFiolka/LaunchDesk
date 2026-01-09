#include "debug.h"
#include "application/controllers/DebugAppConsole.h"

void dlog(const QString &msg) {
    if (auto *dc = DebugAppConsole::instance())
        dc->log(msg);
}

void dclear() {
    if (auto *dc = DebugAppConsole::instance())
        dc->clear();
}

void dtoggle() {
    if (auto *dc = DebugAppConsole::instance())
        dc->toggle();
}
