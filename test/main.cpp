#include <qapplication.h>

#include "page2.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    auto page2 = new Page2;
    page2->show();

    return a.exec();
}