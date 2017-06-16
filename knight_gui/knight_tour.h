#ifndef KNIGHT_TOUR_H
#define KNIGHT_TOUR_H

#include <QMainWindow>
#include "knight_board.h"

namespace Ui {
class knight_tour;
}

class knight_tour : public QMainWindow
{
    Q_OBJECT

public:
    explicit knight_tour(QWidget *parent = 0);
    ~knight_tour();

private:
    Ui::knight_tour *ui;
};

#endif // KNIGHT_TOUR_H
