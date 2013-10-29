#include "setpathdialog.h"

setPathDialog::setPathDialog(QWidget *parent) :
    QDialog(parent)
{
    QLabel *icem_label = new QLabel(tr("ICEM:"));
    QLabel *ansys_label = new QLabel(tr("ANSYS:"));
    icem_edit = new QLineEdit();
    ansys_edit = new QLineEdit();
    QPushButton *icem_button = new QPushButton(QStringLiteral("设置"));
    QPushButton *ansys_button = new QPushButton(QStringLiteral("设置"));

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
}
