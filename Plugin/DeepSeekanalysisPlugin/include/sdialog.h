#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QJsonObject>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QCheckBox>
#include <QScrollArea>
#include <QFileDialog>
#include <QJsonDocument>
#include <QGroupBox>
#include <QCheckBox>
#include <QScrollArea>
#include <QLabel>
#include "projectanalysisdialog.h"
class PathConfigDialog : public QDialog {
    Q_OBJECT
public:
    explicit PathConfigDialog(QWidget *parent = nullptr);

    QJsonObject getConfig() const;

private slots:
    void browseInputDir();
    void browseOutputDir();
    void accept() override;

private:
    // 路径配置控件
    QLineEdit *inputDirEdit;
    QLineEdit *outputDirEdit;
    QJsonObject config;

    // 单文件分析参数控件
    QComboBox *singleApiCombo;
    QComboBox *singleModelCombo;
    QDoubleSpinBox *singleTempSpin;
    QDoubleSpinBox *singleTopPSpin;
    QDoubleSpinBox *singleFreqPenaltySpin;
    QDoubleSpinBox *singlePresPenaltySpin;

    // 信息转化参数控件
    QCheckBox *transformEnableCheck;
    QComboBox *transformApiCombo;
    QComboBox *transformModelCombo;
    QDoubleSpinBox *transformTempSpin;
    QDoubleSpinBox *transformTopPSpin;
    QDoubleSpinBox *transformFreqPenaltySpin;
    QDoubleSpinBox *transformPresPenaltySpin;

    // 总结文件参数控件
    QComboBox *summaryApiCombo;
    QComboBox *summaryModelCombo;
    QDoubleSpinBox *summaryTempSpin;
    QDoubleSpinBox *summaryTopPSpin;
    QDoubleSpinBox *summaryFreqPenaltySpin;
    QDoubleSpinBox *summaryPresPenaltySpin;
    // 添加项目分析对话框指针
    ProjectAnalysisDialog *projectAnalysisDialog = nullptr;
};
