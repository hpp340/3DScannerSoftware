#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionSave_S, SIGNAL(triggered()), this, SLOT(testClick()));
    showMesh *gl = new showMesh();
    gl->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::testClick()
{
    std::cout << "Clicked!!!" << std::endl;
}

void MainWindow::on_actionRemeshing_triggered()
{
    //do_operate_remeshing();
    QWidget *wdt1 =new QWidget;
    wdt1->show();
    this->hide();
}
