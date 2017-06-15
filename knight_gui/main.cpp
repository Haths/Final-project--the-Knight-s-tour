#include "knight_tour.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    knight_tour w;
    w.show();

    return a.exec();
}
