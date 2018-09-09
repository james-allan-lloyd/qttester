#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setObjectName("main_window");
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
