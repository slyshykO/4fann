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
    QApplication a(argc, argv);
    try
        {
            QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
#ifdef Q_OS_WIN32
            QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP-866"));
#endif
            setlocale( LC_NUMERIC, "C" );
            MainWindow w;
            w.show();
            app_res =  a.exec();
        }
    catch(...)
        {
            app_res = 42;
        }

    return app_res;
}


