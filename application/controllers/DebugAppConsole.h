#pragma once
#include <QObject>
#include <QString>

class QWidget;
class QPlainTextEdit;

class DebugAppConsole : public QObject {
    Q_OBJECT
public:
    DebugAppConsole(QWidget* overlay,
                    QPlainTextEdit* view,
                    QObject* parent = nullptr);

    void log(const QString& msg);
    void clear();
    void copyAll();
    void toggle();

    static DebugAppConsole* instance();
    static void setInstance(DebugAppConsole* inst);

protected:
    bool eventFilter(QObject* obj, QEvent* ev) override;

private:
    void placeAtBottom();

private:
    QWidget* m_overlay = nullptr;
    QPlainTextEdit* m_view = nullptr;

    static DebugAppConsole* s_instance;
};
