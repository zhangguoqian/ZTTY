#include <QApplication>
#include <QStyleFactory>
#include "ui/home.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    Home H;
    H.show();
    return QApplication::exec();
}
