#ifndef PLATFORM_H
#define PLATFORM_H

#include <QMainWindow>
#include <QtWidgets>

class renderArea;
class myButton;

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

private slots:
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionAbout_triggered();
    void on_computeDataGenerate();
    void on_geoDataGenerate();
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
    wchar_t* to_wchar(QString);

//members
private:
    Ui::platform *ui;
    myButton* setPath;
    myButton* computeDataGenerate;
    myButton* geoDataGenerate;
    myButton* getLoadBoundary;
    myButton* loadInterpCompute;
    myButton* coldHotTranfer;
    myButton* showResult;
    myButton* editCutPlane;
    myButton* cutPlane;
    myButton* showCuttingPath;
    myButton* prePath;
    myButton* nextPath;
    QList<QPushButton*> btnList;
    QString icemPath;
    QString ansysPath;
    QString proName;
    renderArea* canvas;
    const QString configFile;

    QString prjName;
    QString prjPath;
    QString prjFullName;
    QString prjDirPath;

    QString recentPrjPath;

    QString icem_file;
    QString modelName;
};

#endif // PLATFORM_H
