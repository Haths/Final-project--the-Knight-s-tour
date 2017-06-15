#include "knight_tour.h"
#include "ui_knight_tour.h"

knight_tour::knight_tour(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::knight_tour)
{
    ui->setupUi(this);
}

knight_tour::~knight_tour()
{
    delete ui;
}
