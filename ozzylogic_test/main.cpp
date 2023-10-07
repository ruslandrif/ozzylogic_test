#include "ozzylogic_test.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ozzylogic_test w;
    w.show();
    return a.exec();
}
