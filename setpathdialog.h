#ifndef SETPATHDIALOG_H
#define SETPATHDIALOG_H

#include <QDialog>
#include <QtWidgets>

class setPathDialog : public QDialog
{
    Q_OBJECT
public:
    explicit setPathDialog(QWidget *parent = 0);

signals:

public slots:
private:
    QLineEdit *icem_edit;
    QLineEdit *ansys_edit;
    QString icem;
    QString ansys;

};

#endif // SETPATHDIALOG_H
