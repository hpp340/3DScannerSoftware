#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void testClick();
private slots:
    void on_actionRemeshing_triggered();
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
