#include "mainwindow.h"
#include "XMLer.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    XMLer c ;
    c.setWindowTitle("XMLer");
    // Assuming you have a QMainWindow instance called 'mainWindow'
    QString windowStyle = "background-color: #272727;";

    // Setting the style sheet for the main window
    c.setStyleSheet(windowStyle);
    c.show();
    return a.exec();
}
