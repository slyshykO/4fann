/*
* file   : start_train_ann_dialog.cpp created 2012.3.13
* class  : StartTrainAnnDialog
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#include "start_train_ann_dialog.hpp"
#include <QDialogButtonBox>
#include <QBoxLayout>
#include "netpanel.hpp"

StartTrainAnnDialog::StartTrainAnnDialog(QWidget *parent) :
    QDialog(parent)
{
    m_panel = new NetPanel(this);
    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                          | QDialogButtonBox::Cancel, Qt::Horizontal, this);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout * layout = new QVBoxLayout(this);
    layout->addWidget(m_panel);
    layout->addWidget(buttonBox);
}

NetPanel *StartTrainAnnDialog::panel()
{
    return m_panel;
}
