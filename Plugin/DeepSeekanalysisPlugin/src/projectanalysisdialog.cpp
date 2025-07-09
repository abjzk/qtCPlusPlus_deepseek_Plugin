#include "projectanalysisdialog.h"

ProjectAnalysisDialog::ProjectAnalysisDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("项目分析参数设置");
    setMinimumSize(600, 500);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    QWidget *contentWidget = new QWidget;

    // ========== 项目扫描设置 ==========
    QGroupBox *scanGroup = new QGroupBox("项目扫描设置", contentWidget);
    scanGroup->setStyleSheet("QGroupBox { font-weight: bold; }");

    minFileLevelSpin = new QSpinBox(scanGroup);
    minFileLevelSpin->setRange(1, 10);
    minFileLevelSpin->setValue(3);

    ignoreSuffixesEdit = new QLineEdit(scanGroup);
    ignoreSuffixesEdit->setPlaceholderText("例如: .log,.tmp,.bak,默认忽略,tmp");

    showFileTreeCheck = new QCheckBox("生成文件树图", scanGroup);
    showFileTreeCheck->setChecked(true);

    showFilteredFilesCheck = new QCheckBox("显示过滤文件", scanGroup);
    showFilteredFilesCheck->setChecked(false);

    analysisLevelSpin = new QSpinBox(scanGroup);
    analysisLevelSpin->setRange(1, 5);
    analysisLevelSpin->setValue(3);

    QFormLayout *scanLayout = new QFormLayout(scanGroup);
    scanLayout->addRow("最大子文件级数 (FL):", minFileLevelSpin);
    scanLayout->addRow("忽略文件后缀 (LF):", ignoreSuffixesEdit);
    scanLayout->addRow(showFileTreeCheck);
    scanLayout->addRow(showFilteredFilesCheck);
    scanLayout->addRow("分析详细程度 (AL 1-5):", analysisLevelSpin);

    // ========== 提问参数设置 ==========
    QGroupBox *queryGroup = new QGroupBox("提问参数设置", contentWidget);
    queryGroup->setStyleSheet("QGroupBox { font-weight: bold; }");

    extraRulesEdit = new QTextEdit(queryGroup);
    extraRulesEdit->setPlaceholderText("每行输入一条额外分析规则...");

    keywordsEdit = new QTextEdit(queryGroup);
    keywordsEdit->setPlaceholderText("每行输入一个核心关键词...");

    QFormLayout *queryLayout = new QFormLayout(queryGroup);
    queryLayout->addRow("额外规则 (Rule):", extraRulesEdit);
    queryLayout->addRow("核心关键词 (KeyWord):", keywordsEdit);

    // ========== 开发者设置 ==========
    QGroupBox *devGroup = new QGroupBox("开发者设置", contentWidget);
    devGroup->setStyleSheet("QGroupBox { font-weight: bold; }");

    developerModeCheck = new QCheckBox("启用开发者模式", devGroup);
    developerModeCheck->setChecked(false);

    jsonTemplateEdit = new QLineEdit(devGroup);
    jsonTemplateEdit->setEnabled(false);

    browseJsonBtn = new QPushButton("浏览...", devGroup);
    browseJsonBtn->setEnabled(false);

    QHBoxLayout *jsonLayout = new QHBoxLayout;
    jsonLayout->addWidget(jsonTemplateEdit);
    jsonLayout->addWidget(browseJsonBtn);

    QFormLayout *devLayout = new QFormLayout(devGroup);
    devLayout->addRow(developerModeCheck);
    devLayout->addRow("JSON 模板路径:", jsonLayout);

    // 连接信号
    connect(developerModeCheck, &QCheckBox::toggled, [=](bool checked) {
        jsonTemplateEdit->setEnabled(checked);
        browseJsonBtn->setEnabled(checked);
    });

    connect(browseJsonBtn, &QPushButton::clicked, this, &ProjectAnalysisDialog::browseJsonTemplate);

    // 按钮
    QPushButton *okBtn = new QPushButton("确定", contentWidget);
    QPushButton *cancelBtn = new QPushButton("取消", contentWidget);

    connect(okBtn, &QPushButton::clicked, this, &ProjectAnalysisDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &ProjectAnalysisDialog::reject);

    // 主布局
    QVBoxLayout *mainContentLayout = new QVBoxLayout(contentWidget);
    mainContentLayout->addWidget(scanGroup);
    mainContentLayout->addWidget(queryGroup);
    mainContentLayout->addWidget(devGroup);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);

    mainContentLayout->addLayout(buttonLayout);

    scrollArea->setWidget(contentWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scrollArea);
}

void ProjectAnalysisDialog::browseJsonTemplate()
{
    QString file = QFileDialog::getOpenFileName(this, "选择JSON模板",
                                                QDir::homePath(),
                                                "JSON文件 (*.json)");
    if (!file.isEmpty()) {
        jsonTemplateEdit->setText(file);
    }
}

void ProjectAnalysisDialog::accept()
{
    // 保存扫描设置
    analysisConfig["min_file_level"] = minFileLevelSpin->value();

    // 将 QStringList 转换为 QJsonArray
    QStringList ignoreSuffixes = ignoreSuffixesEdit->text().split(",", Qt::SkipEmptyParts);

    QJsonArray ignoreSuffixesArray;
    ignoreSuffixesArray.append(".tmp");
    for (const QString& suffix : ignoreSuffixes) {
        ignoreSuffixesArray.append(suffix.trimmed());
    }
    analysisConfig["ignore_suffixes"] = ignoreSuffixesArray;

    analysisConfig["show_file_tree"] = showFileTreeCheck->isChecked();
    analysisConfig["show_filtered_files"] = showFilteredFilesCheck->isChecked();
    analysisConfig["analysis_level"] = analysisLevelSpin->value();

    // 保存提问参数
    // 将 QStringList 转换为 QJsonArray
    QStringList extraRules = extraRulesEdit->toPlainText().split("\n", Qt::SkipEmptyParts);
    QJsonArray extraRulesArray;
    for (const QString& rule : extraRules) {
        extraRulesArray.append(rule.trimmed());
    }
    analysisConfig["extra_rules"] = extraRulesArray;

    // 将 QStringList 转换为 QJsonArray
    QStringList keywords = keywordsEdit->toPlainText().split("\n", Qt::SkipEmptyParts);
    QJsonArray keywordsArray;
    for (const QString& keyword : keywords) {
        keywordsArray.append(keyword.trimmed());
    }
    analysisConfig["keywords"] = keywordsArray;

    // 保存开发者设置
    analysisConfig["json_template"]="./AnalysisProm.txt";
    if (developerModeCheck->isChecked()) {
        analysisConfig["json_template"] = jsonTemplateEdit->text();
    }

    QDialog::accept();
}

QJsonObject ProjectAnalysisDialog::getAnalysisConfig() const
{
    return analysisConfig;
}
