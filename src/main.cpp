/*
* file   : main.cpp created 2012.3.11
* class  :
* author : Slyshyk Oleksiy ( alexSlyshyk@gmail.com )
*/

#include <QApplication>
#include <QTextCodec>
#include <QThreadPool>
#include <locale.h>

#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    int app_res;
    QApplication::setOrganizationName(QStringLiteral("4fann"));
    QApplication::setApplicationName (QStringLiteral("4fann_gui"));
    QApplication a(argc, argv);
    try
        {
            setlocale( LC_NUMERIC, "C" );
            MainWindow w;
            w.show();
            a.setQuitOnLastWindowClosed(true);
            app_res =  a.exec();
        }
    catch(...)
        {
            app_res = 42;
        }

    return app_res;
}


