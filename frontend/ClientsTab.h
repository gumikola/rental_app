#pragma once
#include "ui_mainwindow.h"
#include <QObject>

namespace Frontend {

class ClientsTab : public QObject
{
    Q_OBJECT

    Ui::MainWindow* mUi;
    QTableWidget&   mTable;

    void printDefaultTable(void);
    void printClients();

public:
    ClientsTab(Ui::MainWindow* ui);

public slots:
    void addPressed();
    void displayMenu(QPoint pos);
    void tabChanged(int);
};

} // namespace Frontend
