#include "WinHotkeyFilter.h"

#ifdef _WIN32
    #include <windows.h>
#endif

WinHotkeyFilter::WinHotkeyFilter(QObject* parent)
    : QObject(parent)
{
}

bool WinHotkeyFilter::registerHotkey()
{
#ifdef _WIN32
    if (m_registered) return true;

    m_registered =
        RegisterHotKey(nullptr, HOTKEY_ID,
                       MOD_CONTROL | MOD_SHIFT | MOD_ALT,
                       0x4C /* L */) != 0;

    return m_registered;
#else
    return false;
#endif
}

void WinHotkeyFilter::unregisterHotkey()
{
#ifdef _WIN32
    if (!m_registered) return;
    UnregisterHotKey(nullptr, HOTKEY_ID);
    m_registered = false;
#endif
}

bool WinHotkeyFilter::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result)
{
#ifdef _WIN32
    if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG") {
        auto* msg = static_cast<MSG*>(message);
        if (msg && msg->message == WM_HOTKEY && (int)msg->wParam == HOTKEY_ID) {
            emit activated();
            if (result) *result = 0;
            return true;
        }
    }
#else
    Q_UNUSED(eventType)
    Q_UNUSED(message)
    Q_UNUSED(result)
#endif
    return false;
}
