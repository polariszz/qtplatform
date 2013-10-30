#include "platform.h"
#include "ui_platform.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "renderarea.h"
#include "mybutton.h"
#include "setpathdialog.h"
#include "local.h"


platform::platform(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::platform),
    configFile(tr("/config.txt"))
{
    load_path_from_file();
    ui->setupUi(this);
    ui_init();
    ui_set_contraints();
    ui_connect_function();

}

platform::~platform()
{
    delete ui;
}

void platform::ui_init(){
    QGroupBox* preProcess = new QGroupBox(QStringLiteral("前处理模块"));
    setPath = new myButton(QStringLiteral("设置路径"));
    computeDataGenerate = new myButton(QStringLiteral("计算数据生成"));
    geoDataGenerate = new myButton(QStringLiteral("几何数据生成"));
    getLoadBoundary = new myButton(QStringLiteral("载荷边界提取"));
    loadInterpCompute = new myButton(QStringLiteral("载荷插值计算"));
    QVBoxLayout *preProcessLayout = new QVBoxLayout();
    preProcessLayout->addWidget(setPath);
    preProcessLayout->addWidget(computeDataGenerate);
    preProcessLayout->addWidget(getLoadBoundary);
    preProcessLayout->addWidget(loadInterpCompute);
    preProcess->setLayout(preProcessLayout);

    QGroupBox* computingModule = new QGroupBox(QStringLiteral("计算模块"));
    coldHotTranfer = new myButton(QStringLiteral("冷热态转换"));
    showResult = new myButton(QStringLiteral("显示结果"));
    QVBoxLayout* computModuleLayout = new QVBoxLayout;
    computModuleLayout->addWidget(coldHotTranfer);
    computModuleLayout->addWidget(showResult);
    computingModule->setLayout(computModuleLayout);

    QGroupBox* postProcess = new QGroupBox(QStringLiteral("后处理模块"));
    editCutPlane = new myButton(QStringLiteral("编辑切割平面"));
    cutPlane = new myButton(QStringLiteral("切割光顺处理"));
    showCuttingPath = new myButton(QStringLiteral("显示结果"));
    prePath = new myButton(QStringLiteral("Pre"));
    nextPath = new myButton(QStringLiteral("Next"));
    prePath->setFixedWidth(myButton::MINI_WIDTH);
    nextPath->setFixedWidth(myButton::MINI_WIDTH);
    prePath->hide();
    nextPath->hide();
    QHBoxLayout *preNext = new QHBoxLayout;
    preNext->addWidget(prePath);
    preNext->addWidget(nextPath);
    QVBoxLayout *postProcessLayout = new QVBoxLayout;
    postProcessLayout->addWidget(editCutPlane);
    postProcessLayout->addWidget(cutPlane);
    postProcessLayout->addWidget(showCuttingPath);
    postProcessLayout->addLayout(preNext);
    postProcess->setLayout(postProcessLayout);

    QVBoxLayout *buttonsLayout = new QVBoxLayout;
    buttonsLayout->addWidget(preProcess);
    buttonsLayout->addWidget(computingModule);
    buttonsLayout->addWidget(postProcess);
    buttonsLayout->addSpacerItem(new QSpacerItem(0,0));
    buttonsLayout->setStretch(3, 1);
    buttonsLayout->setSizeConstraint(QLayout::SetMinimumSize);

    canvas = new renderArea(ui->centralWidget);
    QVBoxLayout *renderAreaLayout = new QVBoxLayout;
    renderAreaLayout->addWidget(canvas);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addLayout(renderAreaLayout);
    mainLayout->setStretch(1, 1);

    ui->centralWidget->setLayout(mainLayout);
    this->setWindowTitle(QStringLiteral("计算平台"));


    btnList << setPath << computeDataGenerate << geoDataGenerate << getLoadBoundary
            << loadInterpCompute << coldHotTranfer << showResult << editCutPlane
            << cutPlane << showCuttingPath;
    foreach (QWidget* w, btnList) {
        if (w != setPath)
            w->setDisabled(true);
    }
}

void platform::ui_connect_function(){
    connect(setPath, SIGNAL(clicked()), this, SLOT(set_path()));
}

void platform::closeEvent(QCloseEvent *){
    if (!prjName.isEmpty()) {
        save_project_file();
        save_path_to_file();
    }
}

void platform::ui_set_contraints(){
    this->setMaximumWidth(700);
    this->setMaximumHeight(500);
}


void platform::set_path(){
    setPathDialog dlg(this);
    dlg.ansys = ansysPath;
    dlg.icem = icemPath;
    dlg.ansys_edit->setText(ansysPath);
    dlg.icem_edit->setText(icemPath);
    if(dlg.exec() == QDialog::Accepted){
        ansysPath = dlg.ansys;
        icemPath = dlg.icem;
    }
    //Todo: save config
    save_path_to_file();
}

void platform::load_path_from_file(){
    QFile file(qApp->applicationDirPath() + configFile);
    if(file.exists()){
        if (!file.open(QIODevice::ReadOnly)) {
            //error message
            QMessageBox::critical(this, tr("Read Error"),
                                  tr("Error when reading config!"));
        }
        else {
            QTextStream fin(&file);
            ansysPath = fin.readLine();
            icemPath = fin.readLine();
            recentPrjPath = fin.readLine();
            file.close();
        }
    }
}

void platform::save_path_to_file(){
    QFile file(qApp->applicationDirPath() + configFile);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QTextStream fout(&file);
        fout << ansysPath << endl;
        fout << icemPath << endl;
        fout << prjPath << endl;
        fout.flush();
        file.close();
    }
}

void platform::generate_path(QString fileName){
    if (!fileName.isEmpty()){
        prjFullName = fileName;
        QStringList qlist = prjFullName.split('/');
        prjName = qlist.back().split('.').at(0);
        qlist.pop_back();
        prjPath = qlist.join('/');
        prjDirPath = prjPath + tr("/dir_") + prjName;
        if (!QDir(prjDirPath).exists()) {
            QDir(prjPath).mkdir(tr("dir_") + prjName);
        }
        qDebug() << "prjDirPath exists:" << QDir(prjDirPath).exists();
        QDir::setCurrent(prjDirPath);
    }
}

void platform::on_actionNew_triggered()
{
    QString path = prjFullName.isEmpty()? recentPrjPath : prjFullName;
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    QStringLiteral("新建文件"),
                                                    path,
                                                    LOCAL()->fileFilter
                                                    );
    if (!fileName.isEmpty()) {
        generate_path(fileName);
        on_actionSave_triggered();
        setWinTitle();
        enable_buttons();
    }
    qDebug() << "current Path" << QDir::currentPath();

}


void platform::on_actionSave_triggered()
{
    if (prjName.isEmpty()) {
        qDebug() << "prjName empty, ready to open newFile Dialog";
        on_actionNew_triggered();
    }
    else {
        save_project_file();
    }
}

void platform::save_project_file() {
    qDebug() << "saveing project file ...";
    QFile file(prjFullName);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QTextStream fout(&file);
        fout << icem_file << endl;
        file.close();
    }
}

void platform::read_project_file() {
    QFile file(prjFullName);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream fin(&file);
        icem_file = fin.readLine();
        file.close();
    }
}

void platform::enable_buttons()
{
    foreach (QWidget* w, btnList) {
        w->setEnabled(true);
    }
}

void platform::setWinTitle(){
    if(prjName.isEmpty()){
        this->setWindowTitle(QStringLiteral("计算平台"));
    }
    else {
        this->setWindowTitle(QStringLiteral("计算平台 project:") + prjName);
    }
}

void platform::on_actionOpen_triggered()
{
    QString path = prjPath.isEmpty() ? recentPrjPath : prjPath;
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QStringLiteral("打开文件"),
                                                    path,
                                                    LOCAL()->fileFilter
                                                    );
    if (!fileName.isEmpty()) {
        generate_path(fileName);
        read_project_file();
        setWinTitle();
        enable_buttons();
    }
}
