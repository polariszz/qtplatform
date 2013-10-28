#include "platform.h"
#include "ui_platform.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "renderArea.h"


platform::platform(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::platform)
{
    ui->setupUi(this);

    QGroupBox* preProcess = new QGroupBox(QStringLiteral("前处理模块"));
    setPath = new QPushButton(QStringLiteral("设置路径"));
    computeDataGenerate = new QPushButton(QStringLiteral("计算数据生成"));
    geoDataGenerate = new QPushButton(QStringLiteral("几何数据生成"));
    getLoadBoundary = new QPushButton(QStringLiteral("载荷边界提取"));
    loadInterpCompute = new QPushButton(QStringLiteral("载荷插值计算"));
    QVBoxLayout *preProcessLayout = new QVBoxLayout();
    preProcessLayout->addWidget(setPath);
    preProcessLayout->addWidget(computeDataGenerate);
    preProcessLayout->addWidget(getLoadBoundary);
    preProcessLayout->addWidget(loadInterpCompute);
    preProcess->setLayout(preProcessLayout);

    QVBoxLayout *buttonsLayout = new QVBoxLayout;
    buttonsLayout->addWidget(preProcess);

    QGroupBox* process = new QGroupBox(tr("test"));

    buttonsLayout->addWidget(process);

    ui->centralWidget->setLayout(buttonsLayout);

}

platform::~platform()
{
    delete ui;
}
