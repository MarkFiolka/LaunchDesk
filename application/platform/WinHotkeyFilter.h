#pragma once
#include <QObject>
#include <QAbstractNativeEventFilter>

class WinHotkeyFilter : public QObject, public QAbstractNativeEventFilter {
    Q_OBJECT
public:
    explicit WinHotkeyFilter(QObject* parent = nullptr);

    bool registerHotkey();
    void unregisterHotkey();

    signals:
        void activated();

protected:
    bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;

private:
    bool m_registered = false;
    static constexpr int HOTKEY_ID = 0x4C44;
};
