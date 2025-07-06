#ifndef PROJECTANALYSISDIALOG_H
#define PROJECTANALYSISDIALOG_H
#pragma once

#include <QDialog>
#include <QJsonObject>
#include <QSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QTextEdit>
#include <QFormLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QGroupBox>
#include <QScrollArea>
#include<QJsonArray>
class ProjectAnalysisDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProjectAnalysisDialog(QWidget *parent = nullptr);
    QJsonObject getAnalysisConfig() const;

private slots:
    void browseJsonTemplate();
    void accept() override;

private:
    // 项目扫描设置
    QSpinBox *minFileLevelSpin;
    QLineEdit *ignoreSuffixesEdit;
    QCheckBox *showFileTreeCheck;
    QCheckBox *showFilteredFilesCheck;
    QSpinBox *analysisLevelSpin;  // 1-5级

    // 提问参数设置
    QTextEdit *extraRulesEdit;
    QTextEdit *keywordsEdit;

    // 开发者设置
    QCheckBox *developerModeCheck;
    QLineEdit *jsonTemplateEdit;
    QPushButton *browseJsonBtn;

    QJsonObject analysisConfig;
    // 添加项目分析对话框指针
    ProjectAnalysisDialog *projectAnalysisDialog = nullptr;
};
#endif // PROJECTANALYSISDIALOG_H
