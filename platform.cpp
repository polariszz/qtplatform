#include "platform.h"
#include "ui_platform.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "setpathdialog.h"
#include "addition/addi.h"
#include <exception>

#define TEMP(fileName) ( C(prjDirPath + tr("/") + tr(fileName)) )
#define RES(fix)  ( C(prjDirPath + tr("/") + modelName + tr(fix)))
#define REQUIRE_ANSYS_AND_ICEM  if (!path_is_set_or_warning()) {return;}
#define REQUIRE_MODEL if (modelName.isEmpty()) { \
                         QMessageBox::critical(this, tr("Error"), R("请先生成计算数据。")); \
                         return; }

platform::platform(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::platform),
    configFile(tr("/path.conf")),
    flow_file_prompt(1),
    mesh_initialized(false),
    pspad(qApp->applicationDirPath() + tr("/pspad/pspad.exe"))
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
    /*
    QString aboutMsg = tr(" Title   :\tQt Computing Platform \n") +
            tr("Author:\tpolariszz \n") +
            tr("Code  :\thttps://github.com/polariszz/qtplatform \n\n") +
            tr("If you have any question or suggestion about this platform, feel free to contact me by ") +
            tr("beijixing266@gmail.com . :)");
    */
    QString aboutMsg = R("北京大学工学院力学与工程科学系\n") +
            R("指导老师： 孙树立");
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
    generateAndShowAPDL = new myButton(R("生成并显示APDL"));
    coldHotTranfer = new myButton(QStringLiteral("冷热态转换"));
    showResult = new myButton(QStringLiteral("显示冷态坐标"));
    QVBoxLayout* computModuleLayout = new QVBoxLayout;
    computModuleLayout->addWidget(generateAndShowAPDL);
    computModuleLayout->addWidget(coldHotTranfer);
    computModuleLayout->addWidget(showResult);
    computingModule->setLayout(computModuleLayout);

    QGroupBox* postProcess = new QGroupBox(QStringLiteral("后处理模块"));
    editCutPlane = new myButton(QStringLiteral("编辑切割平面"));
    cutPlane = new myButton(QStringLiteral("切割光顺处理"));
    showCuttingPath = new myButton(QStringLiteral("显示切面形状"));
    showCoordinate = new myButton(R("显示切片坐标"));
    //TODO 显示切片坐标
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
    postProcessLayout->addWidget(showCoordinate);
    postProcessLayout->addLayout(preNext);
    postProcess->setLayout(postProcessLayout);

    QVBoxLayout *buttonsLayout = new QVBoxLayout;
    buttonsLayout->addWidget(preProcess);
    buttonsLayout->addWidget(computingModule);
    buttonsLayout->addWidget(postProcess);
    buttonsLayout->addSpacerItem(new QSpacerItem(0,0));
    buttonsLayout->setStretch(3, 1);
    buttonsLayout->setSizeConstraint(QLayout::SetMinimumSize);

    path_msg_label = new QLabel();
    canvas = new renderArea(ui->centralWidget);
    QVBoxLayout *renderAreaLayout = new QVBoxLayout;
    renderAreaLayout->addWidget(path_msg_label);
    path_msg_label->hide();
    renderAreaLayout->addWidget(canvas);
    renderAreaLayout->setStretch(1,1);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addLayout(renderAreaLayout);
    mainLayout->setStretch(1, 1);

    ui->centralWidget->setLayout(mainLayout);
    this->setWindowTitle(QStringLiteral("计算平台"));


    btnList << setPath << computeDataGenerate << geoDataGenerate << getLoadBoundary
            << loadInterpCompute << generateAndShowAPDL <<coldHotTranfer << showResult << editCutPlane
            << cutPlane << showCuttingPath << showCoordinate;
    foreach (QWidget* w, btnList) {
        if (w != setPath)
            w->setDisabled(true);
    }
}

void platform::ui_connect_function(){
    connect(setPath, SIGNAL(clicked()), this, SLOT(set_path()));
    connect(computeDataGenerate, SIGNAL(clicked()), this, SLOT(on_computeDataGenerate()));
    connect(geoDataGenerate, SIGNAL(clicked()), this, SLOT(on_geoDataGenerate()));
    connect(getLoadBoundary, SIGNAL(clicked()), this, SLOT(on_getLoadBoundary()));
    connect(loadInterpCompute, SIGNAL(clicked()), this, SLOT(on_loadInterpCompute()));
    connect(coldHotTranfer, SIGNAL(clicked()), this, SLOT(on_coldHotTranfer()));
    connect(showResult, SIGNAL(clicked()), this, SLOT(on_showResult()));
    connect(editCutPlane, SIGNAL(clicked()), this, SLOT(on_editCutPlane()));
    connect(cutPlane, SIGNAL(clicked()), this, SLOT(on_cutPlane()));
    connect(showCuttingPath, SIGNAL(clicked()), this, SLOT(on_showCuttingPath()));
    connect(prePath, SIGNAL(clicked()), this, SLOT(on_prePath()));
    connect(nextPath, SIGNAL(clicked()), this, SLOT(on_nextPath()));
    connect(generateAndShowAPDL, SIGNAL(clicked()), this, SLOT(on_generateAndShowAPDL()));
    connect(showCoordinate, SIGNAL(clicked()), this, SLOT(on_showCoordinate()));
}

void platform::closeEvent(QCloseEvent *){
    if (!prjName.isEmpty()) {
        save_project_file();
        save_path_to_file();
    }
}

void platform::ui_set_contraints(){
    this->setMaximumWidth(800);
    this->setMaximumHeight(800);
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
        /*
        prjResPath = prjPath + tr("/res_") + prjName;
        if (!QDir(prjResPath).exists()) {
            QDir(prjPath).mkdir(tr("res_") + prjName);
        }
        */
        qDebug() << "prjDirPath exists:" << QDir(prjDirPath).exists();
        //qDebug() << "prjResPath exists:" << QDir(prjResPath).exists();
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
        fout << tr("ICEM_FILE =") << icem_file << endl;
        fout << tr("FLOW_FILE =") << flow_file << endl;
        fout << tr("FLOW_FILE_PROMPT =") << flow_file_prompt << endl;
        file.close();
    }
}

void platform::read_project_file() {
    QTextCodec *code = QTextCodec::codecForName("utf8");
    QFile file(prjFullName);
    if (file.open(QIODevice::ReadOnly)) {
        QTextStream fin(&file);
        fin.setCodec(code);
        icem_file = fin.readLine().split('=').back();
        flow_file = fin.readLine().split('=').back();
        flow_file_prompt = fin.readLine().split("=").back().toInt();
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


char* platform::C(QString s){
    wchar_t* ret = new wchar_t[s.size()+1];
    int temp = s.toWCharArray(ret);
    ret[temp] = 0;
    return WcharToChar(ret);
}

void platform::on_computeDataGenerate()
{
    REQUIRE_ANSYS_AND_ICEM

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
    REQUIRE_ANSYS_AND_ICEM

    qDebug() << "geoDataGenerate()";

    REQUIRE_MODEL

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

            //writing file.
            if (ansys_APDL_file.open(QIODevice::WriteOnly|QIODevice::Text)) {

                QTextStream fout(&ansys_APDL_file);
                fout.setCodec(code);
                while (!in.atEnd()){
                    QString t = in.readLine().replace("MODELNAMEPLACEHOLDER", modelName);
                    fout << t << endl;
                }
                ansys_APDL_file.close();
            }
            apdl_local.close();
        }
        else {
            QMessageBox::critical(this, tr("Error: open file"),
                                  tr("Error happens when loading local_apdl_get.txt.")
                                  );
        }

    }
    qDebug() << "apdl file generated...";

    QString in_file = RES(".in");
    if (!QFile(in_file).exists()) {
        QMessageBox::information(this, R("指定Ansys输入文件"), R("未在工程目录下找到Ansys输入文件(myPrj.in)，请指定。"));
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        QStringLiteral("Ansys输入文件*.in"),
                                                        icem_file,
                                                        tr("in Files (*.in)"));
        if (fileName.isEmpty()){
            return;
        }
        else {
            QFile(fileName).copy(in_file);
        }
    }

    canvas->showComputing();

    QProcess::execute(ansysPath, QStringList()<<"-b" << "-i" <<
                      prjDirPath + tr("/") + modelName + tr("_ansys_get.txt")
                      << "-o" << "log.txt");
    //QProcess::execute(ansysPath, QStringList() << "-g" << "-dir" << prjDirPath);
    QMessageBox::information(this, tr("Done"), tr("Computing completely."));
    canvas->showText();
}

void platform::on_getLoadBoundary() {
    QString error = "";
    if (!QFile(prjDirPath + tr("/FIX.txt")).exists())
        error += tr("FIX.txt ");
    if (!QFile(prjDirPath + tr("/surface.txt")).exists())
        error += tr("surface.txt ");
    if (!QFile(prjDirPath + tr("/blade.txt")).exists())
        error += tr("blade.txt ");
    if (!QFile(prjDirPath + tr("/SYM1.txt")).exists())
        error += tr("SYM1.txt ");
    if (!QFile(prjDirPath + tr("/SYM2.txt")).exists())
        error += tr("SYM2.txt ");
    if (!error.isEmpty()) {
        QMessageBox::critical(this, tr("Error"),
                              R("中间文件") + error + R("未找到\n") + R("请确保已经完正确生成几何数据"));
        return;
    }
    qDebug() << "getLoadBoudary...";
    qDebug() << (prjDirPath + tr("/FIX.txt"));
    transfer(TEMP("FIX.txt"), TEMP("FIX_C.txt"), RES(".fix"));
    transfer(TEMP("surface.txt"), TEMP("surf.txt"), TEMP("surf_num.txt"));
    transfer(TEMP("blade.txt"), TEMP("blade_c.txt"), RES(".sur"));
    couple(TEMP("SYM1.txt"), TEMP("SYM2.txt"), RES(".sym"), TEMP("more1.txt"));

    QMessageBox::information(this, QString(), R("载荷边界提取完成"));
}

void platform::on_loadInterpCompute() {
    qDebug() << "on_loadInterpComput()...";
    qDebug() << flow_file;
    if (flow_file.isEmpty() || !QFile(flow_file).exists()) {
        if (QMessageBox::information(this, tr("information"),R("计算前需要设置流场源文件(flow.txt)\n点击确认进行设置。"))){
            QString fileName = QFileDialog::getOpenFileName(this, R("流场源文件"), prjDirPath, tr("All (*.*)"));
            if (!fileName.isEmpty()) {
                flow_file = fileName;
            }
            else return;
        }
    }
    else if (flow_file_prompt) {
        QMessageBox msgBox(QMessageBox::Warning, R("是否重新设置"),
                           R("您已经设置过流场源文件，是否重新设置？"),
                           0, this);
        msgBox.addButton(R("是"), QMessageBox::AcceptRole);
        msgBox.addButton(R("否"), QMessageBox::RejectRole);
        msgBox.addButton(R("不再提示"), QMessageBox::DestructiveRole);
        int ret = msgBox.exec();
        if (ret == QMessageBox::AcceptRole) {
            QString fileName = QFileDialog::getOpenFileName(this, R("流场源文件"), flow_file, tr("Alll (*.*)"));
            if (!fileName.isEmpty())
                flow_file = fileName;
        }
        else if (ret == QMessageBox::DestructiveRole) {
            flow_file_prompt = 0;
        }
    }
    canvas->showComputing();
    loadcal(C(flow_file), TEMP("surf.txt"), RES(".load"));
    QMessageBox::information(this, tr("completed"), R("载荷插值计算完成"));
    canvas->showText();
}

void platform::on_generateAndShowAPDL() {
    qDebug() << "generate and show APDL";

    QString apdl_new = prjDirPath + tr("/") + modelName + tr("_APDL.txt");
    if (!QFile(apdl_new).exists()) {
        REQUIRE_MODEL ;
        QFile apdl_file(qApp->applicationDirPath() + tr("/local/APDL.txt"));
        if (!apdl_file.exists()) {
            QMessageBox::critical(this, tr("Error"), R("文件local/APDL.txt不存在。"));
            return;
        }

        QTextCodec *code = QTextCodec::codecForName("GBK");
        if (apdl_file.open(QIODevice::ReadOnly)) {
            QTextStream fin(&apdl_file);
            fin.setCodec(code);

            QFile file(apdl_new);
            if (file.open(QIODevice::WriteOnly|QIODevice::Text)) {
                QTextStream fout(&file);
                fout.setCodec(code);
                while (!fin.atEnd()){
                    QString t = fin.readLine().replace("MODELNAMEPLACEHOLDER", modelName);
                    fout << t << endl;
                }
                file.close();
            }
            apdl_file.close();
        }
    }

    QProcess *p = new QProcess(this);
    QString editor = QFile(pspad).exists()? pspad : tr("notepad.exe");
    p->start(editor, QStringList() << apdl_new);

}

void platform::on_coldHotTranfer() {
    qDebug() << "on_coldHotTranfer...";
    REQUIRE_ANSYS_AND_ICEM ;

    QString error = "";
    if (!QFile(RES(".xyz")).exists())
        error += modelName + tr(".xyz ");
    if (!QFile(RES(".ele")).exists())
        error += modelName + tr(".ele ");
    if (!QFile(RES(".sur")).exists())
        error += modelName + tr(".sur ");
    if (!QFile(RES(".load")).exists())
        error += modelName + tr(".load ");
    if (!QFile(RES(".sym")).exists())
        error += modelName + tr(".sym ");
    if (!QFile(RES(".fix")).exists())
        error += modelName + tr(".fix ");
    if (!error.isEmpty()) {
        QMessageBox::critical(this, tr("Error"),
                              R("中间文件") + error + R("未找到\n") + R("请确保已经完正确生成几何数据"));
        return;
    }
    QString apdl_new = prjDirPath + tr("/") + modelName + tr("_APDL.txt");

    QFile para_file(qApp->applicationDirPath() + tr("/local/parameters.txt"));
    if (!para_file.exists() && !QFile(prjDirPath+tr("/parameters.txt")).exists()) {
        QMessageBox::critical(this, tr("Error"), R("文件local/parameters.txt 没有找到。"));
        return;
    }
    para_file.copy(prjDirPath + tr("/parameters.txt"));

    canvas->showComputing();


    QProcess::execute(ansysPath, QStringList()<<"-b"<<"-p"<<"ane3fl"<<"-i"<<
                      apdl_new<<"-o"<<(tr("RES_") + modelName + tr(".txt"))
                      );
    QMessageBox::information(this, QString("completed"), R("冷热态转换计算完成"));
    canvas->showText();
    mesh_initialized = false;
}

void platform::on_showResult() {
    qDebug() << "on_showResult...";
    if (!QFile(tr("RES_") + modelName + tr(".txt")).exists()) {
        QMessageBox::critical(this, tr("Error"), R("没有找到结果文件。"));
        return;
    }
    QString editor = QFile(pspad).exists()? pspad : tr("notepad.exe");
    QProcess *q = new QProcess(this);
    q->start(editor, QStringList() << RES(".fin"));
}

void platform::on_editCutPlane() {
    qDebug() << "edit cut plane...";
    QString editor = QFile(pspad).exists()? pspad : tr("notepad.exe");

    QFile planeFile(prjDirPath + tr("/planes.txt"));
    QTextCodec *code = QTextCodec::codecForName("gbk");
    if (!planeFile.exists())
    {
        if (planeFile.open(QIODevice::WriteOnly|QIODevice::Text)) {
            QTextStream fout(&planeFile);
            fout.setCodec(code);
            fout << R("!请在BEGIN后开始一行编辑切割平面") << endl
                 << R("!切割平面格式为平面法向加上基准点位置") << endl
                 << R("!如 1 1 1 0 0 0代表过0 0 0点的法向为1 1 1的平面") << endl
                 << R("BEGIN:") << endl
                 << R("1 1 1 0 0 0") << endl;
            planeFile.close();
        }
    }

    QProcess::execute(editor, QStringList() << prjDirPath + tr("/planes.txt"));
}

void platform::on_cutPlane() {
    qDebug() << "on_cutPlane...";
    QString mesh = RES(".ele");
    QString pt = RES(".fin");
    QString planes = TEMP("planes.txt");

    if (!QFile(mesh).exists()) {
        QMessageBox::critical(this, tr("Error"), R("网格文件不存在，请先生成网格数据"));
        return;
    }

    if (!QFile(planes).exists()) {
        QMessageBox::critical(this, tr("Error"), R("planes.txt文件不存在，请确认已经生成切割平面文件。"));
        return;
    }
    if (canvas->getState() == renderArea::SHOW_PATH){
        on_showCuttingPath();
    }

    qDebug() << C(pt) << C(mesh);
    canvas->showComputing();

    if (!mesh_initialized) {
        init(C(mesh), C(pt));
        buildBoundary();
        buildBoundaryEdge();
        mesh_initialized = true;

    }

    QFile cutSurface(TEMP("cut_surface_coordinates.txt"));
    using namespace std;
    using namespace ZZ;
    ifstream input(C(planes));
    string s;
    while(input >> s) if(s == "BEGIN:") break;
    Vec3d o;
    Vec3d n(0,0,0);
    data.clear();
    VecPlane.clear();
    cutSurface.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream fout(&cutSurface);
    int planeNum = 0;
    while(input>>n>>o){
        if (n == Vec3d(0,0,0))
            continue;
        fout << "plane " << planeNum++ << " :";
        Vec6d v6d(n[0],n[1],n[2],o[0],o[1],o[2]);
        for (int i = 0; i < 6; ++i)
            fout << " " << v6d[i];
        fout << endl;
        VecPlane.push_back(v6d);
        planeCross(n,o);
        VVP vvcp;
        forit(pit,SplitPoints){
            VP vcp;
            forit(it,*pit)
            {
                vcp.push_back(pt2d(it->v[0],it->v[1]));
            }
            vvcp.push_back(vcp);
        }
        data.push_back(vvcp);
        int size = 0;
        int count = 1;
        forit(pit, Split3dPoints){
            size += (*pit).size();
        }
        fout << size << endl;
        forit(pit, Split3dPoints) {
            forit(it, *pit) {
                fout << count++ <<" "<< it->v[0] <<" " << it->v[1]<<" " << it->v[2] << endl;
            }
        }
        fout << endl;
    }
    cutSurface.close();
    QMessageBox::information(this, tr("completed"), R("切割平面完成，点击显示结果进行查看。"));
    canvas->showText();
}

void platform::set_path_msg_text() {
    QString msg = tr("plane ") + QString::number(pit-VecPlane.begin()+1) + tr(": ");
    for (int i = 0; i < 6; ++i)
        msg += QString::number((*pit)[i]) + tr(" ");
    path_msg_label->setText(msg);

}

void platform::on_showCuttingPath() {
    qDebug() << "on_showCuttingPath...";
    if (!mesh_initialized) {
        QMessageBox::critical(this, tr("Warning"), R("未进行网格切割初始化处理，无法显示"));
        return;
    }

    if (canvas->getState() == renderArea::SHOW_PATH) {
        showCuttingPath->setText(R("显示切片形状"));
        prePath->hide();
        nextPath->hide();
        path_msg_label->hide();
        canvas->showText();
    }
    else {
        showCuttingPath->setText(R("隐藏切片形状"));
        prePath->show();
        nextPath->show();
        path_msg_label->show();
        prePath->setEnabled(false);
        it = data.begin();
        pit = VecPlane.begin();
        canvas->setPaintPath(&(*it));
        if (it == data.end() || it + 1 == data.end())
            nextPath->setEnabled(false);
        else nextPath->setEnabled(true);
        canvas->showPath();
        set_path_msg_text();
    }
}

void platform::on_prePath() {
    qDebug() << "on_prePath...";
    it--;
    pit--;
    canvas->setPaintPath(&(*it));
    if (it == data.begin())
        prePath->setEnabled(false);
    else prePath->setEnabled(true);

    if (it+1 == data.end())
        nextPath->setEnabled(false);
    else nextPath->setEnabled(true);
    set_path_msg_text();
}

void platform::on_nextPath() {
    qDebug() << "on_nextPath...";
    it++;
    pit++;
    canvas->setPaintPath(&(*it));
    if (it == data.begin())
        prePath->setEnabled(false);
    else prePath->setEnabled(true);

    if (it+1 == data.end())
        nextPath->setEnabled(false);
    else nextPath->setEnabled(true);
    set_path_msg_text();
}

void platform::on_showCoordinate() {
    qDebug() << "on_showCoordinate...";
    QString file = TEMP("cut_surface_coordinates.txt");
    if (!QFile(file).exists()) {
        QMessageBox::information(this, tr("Error"), R("需要先进行切割平面。"));
        return;
    }

    QString editor = QFile(pspad).exists()? pspad : tr("notepad.exe");
    QProcess *q = new QProcess(this);
    q->start(editor, QStringList() << file);
}
