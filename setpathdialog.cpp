#include "setpathdialog.h"

setPathDialog::setPathDialog(QWidget *parent) :
    QDialog(parent)
{
    QLabel *icem_label = new QLabel(tr("ICEM:"));
    QLabel *ansys_label = new QLabel(tr("ANSYS:"));
    icem_edit = new QLineEdit();
    ansys_edit = new QLineEdit();
    icem_edit->setFixedWidth(200);
    QPushButton *icem_button = new QPushButton(QStringLiteral("设置"));
    QPushButton *ansys_button = new QPushButton(QStringLiteral("设置"));
    connect(ansys_button, SIGNAL(clicked()), this, SLOT(getAnsysPath()));
    connect(icem_button, SIGNAL(clicked()), this, SLOT(getICEMPath()));

    QGridLayout* setPathLayout = new QGridLayout;
    setPathLayout->addWidget(ansys_label, 0, 0);
    setPathLayout->addWidget(ansys_edit, 0, 1);
    setPathLayout->addWidget(ansys_button, 0, 2);
    setPathLayout->addWidget(icem_label, 1, 0);
    setPathLayout->addWidget(icem_edit, 1, 1);
    setPathLayout->addWidget(icem_button, 1, 2);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                         | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(setPathLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
    this->setWindowTitle(QStringLiteral("设置路径"));
}

void setPathDialog::getAnsysPath(){
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QStringLiteral("设置Ansys路径"),
                                                    ansys,
                                                    tr("Exec (*.exe *.bat)"));
    if(!fileName.isEmpty())
    {
        ansys_edit->setText(fileName);
        ansys = fileName;
    }

}

void setPathDialog::getICEMPath(){
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QStringLiteral("设置ICEM路径"),
                                                    icem,
                                                    tr("Exec (*.exe *.bat)"));
    if(!fileName.isEmpty())
    {
        icem_edit->setText(fileName);
        icem = fileName;
    }
}
