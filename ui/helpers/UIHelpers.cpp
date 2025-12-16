#include "UIHelpers.h"

#include <QIcon>
#include <QStyle>

namespace UIHelpers {

    QPushButton* createIconButton(const QString& iconPath)
    {
        QPushButton* btn = new QPushButton();

        btn->setProperty("type", "icons");
        btn->setAttribute(Qt::WA_StyledBackground, true);

        btn->setIcon(QIcon(iconPath));
        btn->setIconSize(QSize(24, 24));

        btn->setFlat(true);

        btn->style()->unpolish(btn);
        btn->style()->polish(btn);

        return btn;
    }

}
