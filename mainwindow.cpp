#include "mainwindow.h"
#include <string>
#include <iostream>
#include "polynomial.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_pushButton_clicked()
{
    std::string str = ui->lineEdit->text().toStdString();
    ui->label_3->setText(QString(Derivative(str).c_str()));
}
