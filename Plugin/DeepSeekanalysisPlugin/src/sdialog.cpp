#include "sdialog.h"


PathConfigDialog::PathConfigDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("新对话配置");
    setMinimumSize(600, 700); // 增加对话框大小以适应新控件

    // 创建滚动区域以容纳所有控件
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    QWidget *contentWidget = new QWidget;

    // 创建界面元素
    inputDirEdit = new QLineEdit(contentWidget);
    outputDirEdit = new QLineEdit(contentWidget);

    QPushButton *inputBrowseBtn = new QPushButton("浏览...", contentWidget);
    QPushButton *outputBrowseBtn = new QPushButton("浏览...", contentWidget);
    QPushButton *okBtn = new QPushButton("确定", contentWidget);
    QPushButton *cancelBtn = new QPushButton("取消", contentWidget);

    // ========== 单文件分析参数设置 ==========
    QGroupBox *singleFileGroup = new QGroupBox("单文件分析进程模型参数设置", contentWidget);
    singleFileGroup->setStyleSheet("QGroupBox { font-weight: bold; }");

    singleApiCombo = new QComboBox(singleFileGroup);
    singleApiCombo->addItems({"deepseek", "Localdeepseek"});

    singleModelCombo = new QComboBox(singleFileGroup);
    singleModelCombo->addItems({"deepseek-chat", "deepseek-reasoner"});

    singleTempSpin = new QDoubleSpinBox(singleFileGroup);
    singleTempSpin->setRange(0.0, 2.0);
    singleTempSpin->setSingleStep(0.1);
    singleTempSpin->setValue(0.7);

    singleTopPSpin = new QDoubleSpinBox(singleFileGroup);
    singleTopPSpin->setRange(0.0, 1.0);
    singleTopPSpin->setSingleStep(0.1);
    singleTopPSpin->setValue(0.9);

    singleFreqPenaltySpin = new QDoubleSpinBox(singleFileGroup);
    singleFreqPenaltySpin->setRange(0.0, 2.0);
    singleFreqPenaltySpin->setSingleStep(0.1);
    singleFreqPenaltySpin->setValue(0.0);

    singlePresPenaltySpin = new QDoubleSpinBox(singleFileGroup);
    singlePresPenaltySpin->setRange(0.0, 2.0);
    singlePresPenaltySpin->setSingleStep(0.1);
    singlePresPenaltySpin->setValue(0.0);

    QFormLayout *singleFileLayout = new QFormLayout(singleFileGroup);
    singleFileLayout->addRow("API 提供商:", singleApiCombo);
    singleFileLayout->addRow("模型:", singleModelCombo);
    singleFileLayout->addRow("温度 (0-2):", singleTempSpin);
    singleFileLayout->addRow("Top P (0-1):", singleTopPSpin);
    singleFileLayout->addRow("频率惩罚 (0-2):", singleFreqPenaltySpin);
    singleFileLayout->addRow("存在惩罚 (0-2):", singlePresPenaltySpin);

    // ========== 信息转化参数设置 ==========
    QGroupBox *transformGroup = new QGroupBox("信息转化生成提示词模型参数设置", contentWidget);
    transformGroup->setStyleSheet("QGroupBox { font-weight: bold; }");

    transformEnableCheck = new QCheckBox("启用信息转化", transformGroup);
    transformEnableCheck->setChecked(false);

    transformApiCombo = new QComboBox(transformGroup);
    transformApiCombo->addItems({"deepseek", "Localdeepseek"});
    transformApiCombo->setEnabled(false);

    transformModelCombo = new QComboBox(transformGroup);
    transformModelCombo->addItems({"deepseek-chat", "deepseek-reasoner"});
    transformModelCombo->setEnabled(false);

    transformTempSpin = new QDoubleSpinBox(transformGroup);
    transformTempSpin->setRange(0.0, 2.0);
    transformTempSpin->setSingleStep(0.1);
    transformTempSpin->setValue(0.5);
    transformTempSpin->setEnabled(false);

    transformTopPSpin = new QDoubleSpinBox(transformGroup);
    transformTopPSpin->setRange(0.0, 1.0);
    transformTopPSpin->setSingleStep(0.1);
    transformTopPSpin->setValue(1.0);
    transformTopPSpin->setEnabled(false);

    transformFreqPenaltySpin = new QDoubleSpinBox(transformGroup);
    transformFreqPenaltySpin->setRange(0.0, 2.0);
    transformFreqPenaltySpin->setSingleStep(0.1);
    transformFreqPenaltySpin->setValue(0.0);
    transformFreqPenaltySpin->setEnabled(false);

    transformPresPenaltySpin = new QDoubleSpinBox(transformGroup);
    transformPresPenaltySpin->setRange(0.0, 2.0);
    transformPresPenaltySpin->setSingleStep(0.1);
    transformPresPenaltySpin->setValue(0.0);
    transformPresPenaltySpin->setEnabled(false);

    QFormLayout *transformLayout = new QFormLayout(transformGroup);
    transformLayout->addRow(transformEnableCheck);
    transformLayout->addRow("API 提供商:", transformApiCombo);
    transformLayout->addRow("模型:", transformModelCombo);
    transformLayout->addRow("温度 (0-2):", transformTempSpin);
    transformLayout->addRow("Top P (0-1):", transformTopPSpin);
    transformLayout->addRow("频率惩罚 (0-2):", transformFreqPenaltySpin);
    transformLayout->addRow("存在惩罚 (0-2):", transformPresPenaltySpin);

    // 连接启用复选框的信号
    connect(transformEnableCheck, &QCheckBox::toggled, this, [=](bool checked) {
        transformApiCombo->setEnabled(checked);
        transformModelCombo->setEnabled(checked);
        transformTempSpin->setEnabled(checked);
        transformTopPSpin->setEnabled(checked);
        transformFreqPenaltySpin->setEnabled(checked);
        transformPresPenaltySpin->setEnabled(checked);
    });

    // ========== 总结文件参数设置 ==========
    QGroupBox *summaryGroup = new QGroupBox("总结文件主线程模型参数设置", contentWidget);
    summaryGroup->setStyleSheet("QGroupBox { font-weight: bold; }");

    summaryApiCombo = new QComboBox(summaryGroup);
    summaryApiCombo->addItems({"deepseek", "Localdeepseek"});

    summaryModelCombo = new QComboBox(summaryGroup);
    summaryModelCombo->addItems({"deepseek-chat", "deepseek-reasoner"});

    summaryTempSpin = new QDoubleSpinBox(summaryGroup);
    summaryTempSpin->setRange(0.0, 2.0);
    summaryTempSpin->setSingleStep(0.1);
    summaryTempSpin->setValue(0.3);

    summaryTopPSpin = new QDoubleSpinBox(summaryGroup);
    summaryTopPSpin->setRange(0.0, 1.0);
    summaryTopPSpin->setSingleStep(0.1);
    summaryTopPSpin->setValue(0.7);

    summaryFreqPenaltySpin = new QDoubleSpinBox(summaryGroup);
    summaryFreqPenaltySpin->setRange(0.0, 2.0);
    summaryFreqPenaltySpin->setSingleStep(0.1);
    summaryFreqPenaltySpin->setValue(0.2);

    summaryPresPenaltySpin = new QDoubleSpinBox(summaryGroup);
    summaryPresPenaltySpin->setRange(0.0, 2.0);
    summaryPresPenaltySpin->setSingleStep(0.1);
    summaryPresPenaltySpin->setValue(0.1);

    QFormLayout *summaryLayout = new QFormLayout(summaryGroup);
    summaryLayout->addRow("API 提供商:", summaryApiCombo);
    summaryLayout->addRow("模型:", summaryModelCombo);
    summaryLayout->addRow("温度 (0-2):", summaryTempSpin);
    summaryLayout->addRow("Top P (0-1):", summaryTopPSpin);
    summaryLayout->addRow("频率惩罚 (0-2):", summaryFreqPenaltySpin);
    summaryLayout->addRow("存在惩罚 (0-2):", summaryPresPenaltySpin);

    // 连接信号槽
    connect(inputBrowseBtn, &QPushButton::clicked, this, &PathConfigDialog::browseInputDir);
    connect(outputBrowseBtn, &QPushButton::clicked, this, &PathConfigDialog::browseOutputDir);
    connect(okBtn, &QPushButton::clicked, this, &PathConfigDialog::accept);
    connect(cancelBtn, &QPushButton::clicked, this, &PathConfigDialog::reject);

    // 布局管理
    QFormLayout *pathLayout = new QFormLayout;
    pathLayout->addRow("输入目录:", inputDirEdit);
    pathLayout->addRow("", inputBrowseBtn);
    pathLayout->addRow("输出目录:", outputDirEdit);
    pathLayout->addRow("", outputBrowseBtn);

    QVBoxLayout *mainContentLayout = new QVBoxLayout(contentWidget);
    mainContentLayout->addLayout(pathLayout);
    mainContentLayout->addSpacing(10);
    mainContentLayout->addWidget(singleFileGroup);
    mainContentLayout->addSpacing(10);
    mainContentLayout->addWidget(transformGroup);
    mainContentLayout->addSpacing(10);
    mainContentLayout->addWidget(summaryGroup);
    mainContentLayout->addStretch();

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);

    mainContentLayout->addLayout(buttonLayout);

    scrollArea->setWidget(contentWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(scrollArea);
}

void PathConfigDialog::browseInputDir() {
    QString dir = QFileDialog::getExistingDirectory(this, "选择输入目录", QDir::homePath());
    if (!dir.isEmpty()) {
        inputDirEdit->setText(dir);
    }
}

void PathConfigDialog::browseOutputDir() {
    QString dir = QFileDialog::getExistingDirectory(this, "选择输出目录", QDir::homePath());
    if (!dir.isEmpty()) {
        outputDirEdit->setText(dir);
    }
}

void PathConfigDialog::accept() {
    // 保存路径配置
    config["inputDir"] = inputDirEdit->text();
    config["outputDir"] = outputDirEdit->text();

    // 保存单文件分析参数
    QJsonObject singleFileParams;
    singleFileParams["api"] = singleApiCombo->currentText();
    singleFileParams["model"] = singleModelCombo->currentText();
    singleFileParams["temperature"] = singleTempSpin->value();
    singleFileParams["top_p"] = singleTopPSpin->value();
    singleFileParams["frequency_penalty"] = singleFreqPenaltySpin->value();
    singleFileParams["presence_penalty"] = singlePresPenaltySpin->value();
    config["single_file_analysis"] = singleFileParams;

    // 保存信息转化参数（如果启用）
    if (transformEnableCheck->isChecked()) {
        QJsonObject transformParams;
        transformParams["api"] = transformApiCombo->currentText();
        transformParams["model"] = transformModelCombo->currentText();
        transformParams["temperature"] = transformTempSpin->value();
        transformParams["top_p"] = transformTopPSpin->value();
        transformParams["frequency_penalty"] = transformFreqPenaltySpin->value();
        transformParams["presence_penalty"] = transformPresPenaltySpin->value();
        config["transform_prompt"] = transformParams;
    }

    // 保存总结文件参数
    QJsonObject summaryParams;
    summaryParams["api"] = summaryApiCombo->currentText();
    summaryParams["model"] = summaryModelCombo->currentText();
    summaryParams["temperature"] = summaryTempSpin->value();
    summaryParams["top_p"] = summaryTopPSpin->value();
    summaryParams["frequency_penalty"] = summaryFreqPenaltySpin->value();
    summaryParams["presence_penalty"] = summaryPresPenaltySpin->value();
    config["summary"] = summaryParams;
    // 2. 创建并显示项目分析参数对话框
    projectAnalysisDialog = new ProjectAnalysisDialog(this);
    if (projectAnalysisDialog->exec() == QDialog::Accepted) {
        // 3. 如果用户接受，合并配置
        QJsonObject analysisConfig = projectAnalysisDialog->getAnalysisConfig();

        // 将项目分析配置合并到主配置中
        for (auto it = analysisConfig.begin(); it != analysisConfig.end(); ++it) {
            config[it.key()] = it.value();
        }

        // 4. 关闭主对话框
    QDialog::accept();
    }
}
QJsonObject PathConfigDialog::getConfig() const {
    return config;
}
