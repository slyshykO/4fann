/*******************************************************************************
*  file    : thread_count_change_dialog.cpp
*  created : 19.03.2012
*  author  : Slyshyk Oleksiy (alex312@meta.ua)
*******************************************************************************/

#include "thread_count_change_dialog.hpp"
#include "ui_thread_count_change_dialog.h"

#include <QThreadPool>

ThreadCountChangeDialog::ThreadCountChangeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ThreadCountChangeDialog)
{
    ui->setupUi(this);
    ui->spinBox->setValue(QThreadPool::globalInstance()->maxThreadCount());
}

ThreadCountChangeDialog::~ThreadCountChangeDialog()
{
    delete ui;
}

int ThreadCountChangeDialog::newThreadCount()
{
    return ui->spinBox->value();
}
