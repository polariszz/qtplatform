#ifndef PLATFORM_H
#define PLATFORM_H

#include <QMainWindow>
#include <QtWidgets>
#include "meshfunc/smooth.h"
#include "local.h"
#include "renderarea.h"
#include "mybutton.h"


namespace Ui {
class platform;
}

class platform : public QMainWindow
{
    Q_OBJECT

public:
    explicit platform(QWidget *parent = 0);
    ~platform();
    void closeEvent(QCloseEvent *);
//methods

public slots:
    void set_path();
    void set_path_msg_text();
//slots
private slots:
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionAbout_triggered();
    void on_computeDataGenerate();
    void on_geoDataGenerate();
    void on_getLoadBoundary();
    void on_loadInterpCompute();
    void on_generateAndShowAPDL();
    void on_coldHotTranfer();
    void on_showResult();
    void on_editCutPlane();
    void on_cutPlane();
    void on_showCuttingPath();
    void on_prePath();
    void on_nextPath();
    void on_showCoordinate();

//private methods
private:
    void ui_init();
    void ui_connect_function();
    void ui_set_contraints();
    void save_path_to_file();
    void load_path_from_file();
    void enable_buttons();
    void generate_path(QString fileName);
    void setWinTitle();
    void read_project_file();
    void save_project_file();
    char* C(QString);
    //make sure ansys and icem path is set.
    bool path_is_set_or_warning();

//members
private:
    Ui::platform *ui;
    myButton* setPath;
    myButton* computeDataGenerate;
    myButton* geoDataGenerate;
    myButton* getLoadBoundary;
    myButton* loadInterpCompute;
    myButton* generateAndShowAPDL;
    myButton* coldHotTranfer;
    myButton* showResult;
    myButton* editCutPlane;
    myButton* cutPlane;
    myButton* showCuttingPath;
    myButton* prePath;
    myButton* nextPath;
    myButton* showCoordinate;
    QList<QPushButton*> btnList;
    QString icemPath;
    QString ansysPath;
    QString proName;
    QLabel* path_msg_label;
    renderArea* canvas;
    const QString configFile;

    QString prjName;
    QString prjPath;
    QString prjFullName;
    QString prjDirPath;
//    QString prjResPath;

    QString recentPrjPath;

    QString icem_file;
    QString modelName;
    QString flow_file;
    int flow_file_prompt;
    bool mesh_initialized;

    ZZ::VVVP::iterator it;
    ZZ::VVVP data;
    std::vector<Vec6d> VecPlane;
    vector<Vec6d>::iterator pit;

    QString pspad;
};

#endif // PLATFORM_H
