#include "mainwindow.h"
#include "Equipment.h"
#include "ui_mainwindow.h"
#include <QScopedPointer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::makeObjects()
{
    mEquipment.reset(new Frontend::Equipment(ui));
}
