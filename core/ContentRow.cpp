#include "ContentRow.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTimer>
#include <QFileInfo>
#include <QFormLayout>

#include "AppPickerWindow.h"
#include "ContentStorage.h"
#include "CommandTemplate.h"

static QLabel *makeElidedLabelInternal(const QString &text, int width) {
    QLabel *l = new QLabel();
    l->setMinimumWidth(width);
    l->setMaximumWidth(width);
    l->setStyleSheet("color:#d0d0d0; font-size:14px;");
    QFontMetrics fm(l->font());
    l->setText(fm.elidedText(text, Qt::ElideMiddle, width));
    return l;
}

QLabel *ContentRow::createElidedLabel(const QString &text, int width) {
    return makeElidedLabelInternal(text, width);
}

ContentRow::ContentRow(const QString &folderPath, QWidget *parent)
    : QWidget(parent),
      m_folderPath(folderPath),
      m_folderName(QFileInfo(folderPath).fileName()) {
    m_cfg = ContentStorage::loadContentConfig(m_folderPath, m_folderName);

    buildUi();
    updateTypeUi();
    rebuildCmdParamUi(m_cfg.exePath);
    refreshMainRow();

    QTimer::singleShot(0, this, [this]() {
        m_expandBtn->setChecked(false);
        m_details->setVisible(false);
        m_expandBtn->setIcon(QIcon(":/icons/Dropdown_Down_Button.svg"));
    });
}

void ContentRow::buildUi() {
    m_rootLayout = new QVBoxLayout(this);
    m_rootLayout->setContentsMargins(0, 0, 0, 0);

    m_mainRow = new QWidget(this);
    QHBoxLayout *hl = new QHBoxLayout(m_mainRow);
    hl->setContentsMargins(4, 2, 4, 2);

    m_expandBtn = new QPushButton();
    m_expandBtn->setCheckable(true);
    m_expandBtn->setFixedSize(24, 24);
    m_expandBtn->setIcon(QIcon(":/icons/Dropdown_Down_Button.svg"));

    m_playBtn = new QPushButton();
    m_playBtn->setFixedSize(28, 28);
    m_playBtn->setIcon(QIcon(":/icons/Play_Button.svg"));

    m_statusCircle = new QLabel();
    m_statusCircle->setFixedSize(20, 20);

    m_nameLabel = createElidedLabel(
        m_cfg.name.isEmpty() ? "<no name>" : m_cfg.name, 160);

    QLabel *sep = new QLabel(" - ");
    sep->setStyleSheet("color:#999;");

    m_pathLabel = createElidedLabel(
        m_cfg.exePath.isEmpty() ? "<no command>" : m_cfg.exePath, 260);

    m_removeBtn = new QPushButton();
    m_removeBtn->setFixedSize(28, 28);
    m_removeBtn->setIcon(QIcon(":/icons/Remove_Button.svg"));

    hl->addWidget(m_expandBtn);
    hl->addWidget(m_playBtn);
    hl->addWidget(m_statusCircle);
    hl->addWidget(m_nameLabel);
    hl->addWidget(sep);
    hl->addWidget(m_pathLabel);
    hl->addStretch();
    hl->addWidget(m_removeBtn);

    m_details = new QWidget(this);
    QVBoxLayout *dl = new QVBoxLayout(m_details);
    dl->setContentsMargins(24, 4, 4, 4);

    {
        QHBoxLayout *l = new QHBoxLayout();
        l->addWidget(new QLabel("Name:"));
        m_nameEdit = new QLineEdit(m_cfg.name);
        l->addWidget(m_nameEdit);
        dl->addLayout(l);
    }

    {
        QHBoxLayout *l = new QHBoxLayout();
        l->addWidget(new QLabel("Type:"));
        m_typeCombo = new QComboBox();
        m_typeCombo->addItems({"App", "Terminal"});
        m_typeCombo->setCurrentText(m_cfg.type);
        l->addWidget(m_typeCombo);
        dl->addLayout(l);
    }

    {
        QHBoxLayout *l = new QHBoxLayout();
        m_cmdLabel = new QLabel("Executable:");
        m_cmdEdit = new QLineEdit(m_cfg.exePath);
        m_cmdBrowse = new QPushButton("Browse");
        m_cmdBrowseApps = new QPushButton("Apps");
        l->addWidget(m_cmdLabel);
        l->addWidget(m_cmdEdit);
        l->addWidget(m_cmdBrowse);
        l->addWidget(m_cmdBrowseApps);
        dl->addLayout(l);
    }

    m_paramWidget = new QWidget(this);
    m_paramLayout = new QFormLayout(m_paramWidget);
    m_paramLayout->setContentsMargins(0, 0, 0, 0);
    dl->addWidget(m_paramWidget);
    m_paramWidget->setVisible(false);

    {
        QHBoxLayout *l = new QHBoxLayout();
        m_workingDirLabel = new QLabel("Working dir:");
        m_workingDirEdit = new QLineEdit(m_cfg.workingDir);
        m_workingDirBrowse = new QPushButton("Browse");
        l->addWidget(m_workingDirLabel);
        l->addWidget(m_workingDirEdit);
        l->addWidget(m_workingDirBrowse);
        dl->addLayout(l);
    }

    m_saveBtn = new QPushButton("Save");
    dl->addWidget(m_saveBtn);

    m_rootLayout->addWidget(m_mainRow);
    m_rootLayout->addWidget(m_details);

    connect(m_expandBtn, &QPushButton::toggled, this, [this](bool c) {
        m_details->setVisible(c);
        m_expandBtn->setIcon(QIcon(
            c
                ? ":/icons/Dropdown_Up_Button.svg"
                : ":/icons/Dropdown_Down_Button.svg"));
    });

    connect(m_typeCombo, &QComboBox::currentTextChanged,
            this, [this](const QString &t) {
                m_cfg.type = t;
                ContentStorage::saveContentConfig(m_folderPath, m_cfg);

                updateTypeUi();

                if (t == "Terminal") {
                    rebuildCmdParamUi(m_cmdEdit->text());
                } else {
                    while (m_paramLayout->rowCount() > 0) {
                        m_paramLayout->removeRow(0);
                    }
                    m_paramEdits.clear();
                    m_paramWidget->setVisible(false);
                }

                refreshMainRow();
            });

    connect(m_nameEdit, &QLineEdit::textChanged,
            this, [this](const QString &t) {
                m_cfg.name = t.trimmed();
                ContentStorage::saveContentConfig(m_folderPath, m_cfg);
                refreshMainRow();
            });

    connect(m_cmdEdit, &QLineEdit::textChanged,
            this, [this](const QString &t) {
                m_cfg.exePath = t.trimmed();
                ContentStorage::saveContentConfig(m_folderPath, m_cfg);

                if (m_typeCombo->currentText() == "Terminal") {
                    rebuildCmdParamUi(t);
                } else {
                    m_paramWidget->setVisible(false);
                }

                refreshMainRow();
            });


    connect(m_workingDirEdit, &QLineEdit::textChanged,
            this, [this](const QString &t) {
                m_cfg.workingDir = t.trimmed();
                ContentStorage::saveContentConfig(m_folderPath, m_cfg);
            });

    connect(m_workingDirBrowse, &QPushButton::clicked, this, [this]() {
        QString dir = QFileDialog::getExistingDirectory(
            this,
            "Select Working Directory",
            QDir::homePath(),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );

        if (dir.isEmpty())
            return;

        m_workingDirEdit->setText(dir);

        m_cfg.workingDir = dir;
        ContentStorage::saveContentConfig(m_folderPath, m_cfg);
    });


    connect(m_cmdBrowseApps, &QPushButton::clicked, this, [this]() {
        AppPickerWindow win(window());
        win.setWindowModality(Qt::ApplicationModal);
        if (win.exec() == QDialog::Accepted) {
            QString cmd = win.selectedCommand();
            if (!cmd.isEmpty())
                m_cmdEdit->setText(cmd);
        }
    });

    connect(m_cmdBrowse, &QPushButton::clicked, this, [this]() {
        QString file = QFileDialog::getOpenFileName(
            window(),
            "Select Executable",
            QDir::homePath()
            #ifdef _WIN32
            , "Executables (*.exe)"
            #endif
        );

        if (file.isEmpty())
            return;

        m_cmdEdit->setText(file);
    });

    connect(m_playBtn, &QPushButton::clicked,
            this, [this]() { emit requestLaunch(m_folderPath, true); });

    connect(m_removeBtn, &QPushButton::clicked,
            this, [this]() { emit requestRemove(m_folderPath); });
}

void ContentRow::rebuildCmdParamUi(const QString &command) {
    QStringList vars = CommandTemplate::extractPlaceholders(command);

    while (m_paramLayout->rowCount() > 0) {
        m_paramLayout->removeRow(0);
    }
    m_paramEdits.clear();

    if (vars.isEmpty()) {
        m_paramWidget->setVisible(false);
        return;
    }

    for (const QString &v: vars) {
        QLineEdit *edit = new QLineEdit(this);
        edit->setText(m_cfg.params.value(v));

        connect(edit, &QLineEdit::textChanged,
                this, [this, v](const QString &val) {
                    m_cfg.params[v] = val;
                    ContentStorage::saveContentConfig(m_folderPath, m_cfg);
                });

        m_paramLayout->addRow(v + ":", edit);
        m_paramEdits[v] = edit;
    }

    m_paramWidget->setVisible(true);
}


void ContentRow::updateTypeUi() {
    bool isApp = (m_typeCombo->currentText() == "App");
    m_cmdLabel->setText(isApp ? "Executable:" : "Command:");
    m_cmdBrowse->setVisible(isApp);
    m_cmdBrowseApps->setVisible(isApp);
    m_workingDirLabel->setVisible(!isApp);
    m_workingDirEdit->setVisible(!isApp);
    m_workingDirBrowse->setVisible(!isApp);
}

void ContentRow::refreshMainRow() {
    m_nameLabel->setText(m_cfg.name.isEmpty() ? "<no name>" : m_cfg.name);
    m_pathLabel->setText(m_cfg.exePath.isEmpty() ? "<no command>" : m_cfg.exePath);
    updateStatusCircle();
}

bool ContentRow::isConfigValid() const {
    QString path = m_cmdEdit->text().trimmed();
    if (path.isEmpty())
        return false;
    if (path.startsWith("shell:AppsFolder\\", Qt::CaseInsensitive))
        return true;
    if (m_typeCombo->currentText() == "App")
        return QFileInfo(path).exists();
    return true;
}

void ContentRow::updateStatusCircle() {
    m_statusCircle->setStyleSheet(
        isConfigValid()
            ? "border-radius:10px; background:green;"
            : "border-radius:10px; background:red;");
}
