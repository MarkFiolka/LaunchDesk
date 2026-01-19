#include "DebugAppConsole.h"

#include <QApplication>
#include <QEvent>

DebugAppConsole* DebugAppConsole::s_instance = nullptr;

DebugAppConsole::DebugAppConsole(QWidget* overlay,
                                 QPlainTextEdit* view,
                                 QObject* parent)
    : QObject(parent)
    , m_overlay(overlay)
    , m_view(view)
{
    if (m_overlay && m_overlay->parentWidget())
        m_overlay->parentWidget()->installEventFilter(this);
}

DebugAppConsole* DebugAppConsole::instance()
{
    return s_instance;
}

void DebugAppConsole::setInstance(DebugAppConsole* inst)
{
    s_instance = inst;
}

void DebugAppConsole::log(const QString& msg)
{
    if (!m_view)
        return;

    const QString ts = QDateTime::currentDateTime().toString("HH:mm:ss");
    m_view->appendPlainText(QString("[%1] %2").arg(ts, msg));
}

void DebugAppConsole::clear()
{
    if (m_view)
        m_view->clear();
}

void DebugAppConsole::copyAll()
{
    if (m_view)
        QApplication::clipboard()->setText(m_view->toPlainText());
}

void DebugAppConsole::placeAtBottom()
{
    if (!m_overlay)
        return;

    QWidget* host = m_overlay->parentWidget();
    if (!host)
        return;

    const int h = m_overlay->height();
    m_overlay->setGeometry(
        0,
        host->height() - h,
        host->width(),
        h
    );
}

void DebugAppConsole::toggle()
{
    if (!m_overlay)
        return;

    if (m_overlay->isVisible()) {
        m_overlay->hide();
        return;
    }

    placeAtBottom();
    m_overlay->raise();
    m_overlay->show();
}

bool DebugAppConsole::eventFilter(QObject* obj, QEvent* ev)
{
    if (m_overlay && obj == m_overlay->parentWidget() && m_overlay->isVisible()) {
        if (ev->type() == QEvent::Resize || ev->type() == QEvent::Move) {
            placeAtBottom();
        }
    }
    return QObject::eventFilter(obj, ev);
}
