/*
* file   : start_train_ann_dialog.hpp created 2012.3.13
* class  : StartTrainAnnDialog
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#ifndef START_TRAIN_ANN_DIALOG_HPP
#define START_TRAIN_ANN_DIALOG_HPP

#include <QDialog>

class NetPanel;

class StartTrainAnnDialog : public QDialog
{
    Q_OBJECT
public:
    explicit StartTrainAnnDialog(QWidget *parent = 0);

    const NetPanel * panel() const ;
private:
    NetPanel * m_panel;

};

#endif // START_TRAIN_ANN_DIALOG_HPP
