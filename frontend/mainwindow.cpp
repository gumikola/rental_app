#include "mainwindow.h"
#include "ClientsTab.h"
#include "Equipment.h"
#include "RentsTab.h"
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
    mClientsTab.reset(new Frontend::ClientsTab(ui));
    mRentsTab.reset(new Frontend::RentsTab(ui));
}
