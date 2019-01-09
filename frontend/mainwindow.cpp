#include "mainwindow.h"
#include "ClientsTab.h"
#include "Equipment.h"
#include "RentsTab.h"
#include "ui_mainwindow.h"
#include <QScopedPointer>
#include <backend/Database.h>

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
    mDatabase.reset(new Backend::Database());
    mEquipment.reset(new Frontend::Equipment(ui, *mDatabase.get()));
    mClientsTab.reset(new Frontend::ClientsTab(ui, *mDatabase.get()));
    mRentsTab.reset(new Frontend::RentsTab(ui, *mDatabase.get()));
}
