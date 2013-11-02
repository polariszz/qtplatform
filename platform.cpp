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
    configFile(tr("/path.conf"))
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
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

void platform::on_actionAbout_triggered()
{
    QString aboutMsg = tr(" Title   :\tQt Computing Platform \n") +
            tr("Author:\tpolariszz \n") +
            tr("Code  :\thttps://github.com/polariszz/qtplatform \n\n") +
            tr("If you have any question or suggestion about this platform, feel free to contact me by ") +
            tr("beijixing266@gmail.com . :)");

    QMessageBox::about(this, QStringLiteral("关于..."), aboutMsg);
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
    preProcessLayout->addWidget(geoDataGenerate);
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
    connect(computeDataGenerate, SIGNAL(clicked()), this, SLOT(on_computeDataGenerate()));
    connect(geoDataGenerate, SIGNAL(clicked()), this, SLOT(on_geoDataGenerate()));
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

bool platform::path_is_set_or_warning(){
    if (ansysPath.isEmpty() || icemPath.isEmpty()) {
        QMessageBox::critical(this, tr("Warning"), R("计算前请先设置Ansys及ICEM路径"));
        return false;
    }
    return true;
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
    QTextCodec *code = QTextCodec::codecForName("utf8");
    QFile file(qApp->applicationDirPath() + configFile);
    if(file.exists()){
        if (!file.open(QIODevice::ReadOnly)) {
            //error message
            QMessageBox::critical(this, tr("Read Error"),
                                  tr("Error when reading config!"));
        }
        else {
            QTextStream fin(&file);
            fin.setCodec(code);
            ansysPath = fin.readLine();
            icemPath = fin.readLine();
            recentPrjPath = fin.readLine();
            file.close();
        }
    }
}

void platform::save_path_to_file(){
    QTextCodec *code = QTextCodec::codecForName("utf8");
    QFile file(qApp->applicationDirPath() + configFile);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QTextStream fout(&file);
        fout.setCodec(code);
        fout << ansysPath << endl;
        fout << icemPath << endl;
        if (!prjPath.isEmpty()) recentPrjPath = prjPath;
        fout << recentPrjPath << endl;
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
    qDebug() << "loading path:" << path;
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
    QTextCodec *code = QTextCodec::codecForName("utf8");
    QFile file(prjFullName);
    if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
        QTextStream fout(&file);
        fout.setCodec(code);
        fout << icem_file << endl;
        file.close();
    }
}

void platform::read_project_file() {
    QTextCodec *code = QTextCodec::codecForName("utf8");
    QFile file(prjFullName);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream fin(&file);
        fin.setCodec(code);
        icem_file = fin.readLine();
        modelName = icem_file.split('/').back().split('.').front();
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



wchar_t* platform::to_wchar(QString s){
    wchar_t* ret = new wchar_t[s.size()+1];
    int temp = s.toWCharArray(ret);
    ret[temp] = 0;
    return ret;
}

void platform::on_computeDataGenerate()
{
    if (!path_is_set_or_warning())
        return;
    qDebug() << "icem_file: " << icem_file;
    //it don't need to check whether it is empty.
    //otherwise we cannot change the icem_file if choosen.
    //if (icem_file.isEmpty()){

    {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        QStringLiteral("ICEM文件 .prj"),
                                                        icem_file,
                                                        tr("prj Files (*.prj)"));
        qDebug() << "modelName: " << modelName;
        if(!fileName.isEmpty()){
            canvas->showComputing();
            icem_file = fileName;
            modelName = fileName.split('/').back().split('.').front();
            qDebug() << "icem_path: " << icemPath;
            qDebug() << "icem_file: " << icem_file;

            QProcess *p = new QProcess(this);
            p->start(icemPath, QStringList() << icem_file);
            p->waitForFinished();
            canvas->showText();
        }
    }
}

void platform::on_geoDataGenerate(){
    if (!path_is_set_or_warning())
        return;
    qDebug() << "geoDataGenerate()";

    if (modelName.isEmpty()) {
        QMessageBox::critical(this, tr("Error"), R("请先生成计算数据。"));
        return;
    }
    QTextCodec *code = QTextCodec::codecForName("gbk");
    QFile ansys_APDL_file(prjDirPath + tr("/") + modelName + tr("_ansys_get.txt"));
//    if (!ansys_APDL_file.exists()) {
    {
        QFile apdl_local(qApp->applicationDirPath() + tr("/local/local_ansys_get.txt"));

        if (!apdl_local.exists()){
            QMessageBox::critical(this, tr("Error: file lost"),tr("File local/local_ansys_get.txt lost."));
            return;
        }
        if (apdl_local.open(QIODevice::ReadOnly)) {
            //read and replace
            QTextStream in(&apdl_local);
            in.setCodec(code);
            QString script = in.readAll().replace(tr("MODELNAMEPLACEHOLDER"), modelName);
            apdl_local.close();

            //writing file.
            if (ansys_APDL_file.open(QIODevice::WriteOnly|QIODevice::Text)) {

                QTextStream fout(&ansys_APDL_file);
                fout.setCodec(code);
                fout << script;
                fout.flush();
                ansys_APDL_file.close();
            }
        }
        else {
            QMessageBox::critical(this, tr("Error: open file"),
                                  tr("Error happens when loading local_apdl_get.txt.")
                                  );
        }

    }
    qDebug() << "apdl file generated...";

    canvas->showComputing();
    QProcess *p = new QProcess(this);
    p->start(ansysPath, QStringList() << "-g" << "-dir" << prjDirPath);
    p->waitForFinished();
    canvas->showText();
    QMessageBox::information(this, tr("Done"), tr("Computing completely."));

}
