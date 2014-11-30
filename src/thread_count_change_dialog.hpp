/*******************************************************************************
*  file    : thread_count_change_dialog.hpp
*  created : 19.03.2012
*  author  : Slyshyk Oleksiy (alex312@meta.ua)
*******************************************************************************/

#ifndef THREAD_COUNT_CHANGE_DIALOG_HPP
#define THREAD_COUNT_CHANGE_DIALOG_HPP

#include <QDialog>

namespace Ui {
class ThreadCountChangeDialog;
}

class ThreadCountChangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ThreadCountChangeDialog(QWidget *parent = 0);
    ~ThreadCountChangeDialog();
    int newThreadCount();
private:
    Ui::ThreadCountChangeDialog *ui;
};

#endif // THREAD_COUNT_CHANGE_DIALOG_HPP
