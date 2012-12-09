#include "main_window.hpp"
#include "ui_main_window.h"

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::teste(QImage img)
{
    ui->label->setPixmap(QPixmap::fromImage(img));
    ui->label->update();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    emit closed();
    QMainWindow::closeEvent(event);
}
