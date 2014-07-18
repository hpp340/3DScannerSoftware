#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionSave_S, SIGNAL(triggered()), this, SLOT(testClick()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testClick()
{
    std::cout << "Clicked!!!" << std::endl;
}
