#include "ClientsTab.h"
#include "AddClient.h"
#include "common.h"
#include <QPoint>

namespace Frontend {

ClientsTab::ClientsTab(Ui::MainWindow* ui)
    : mUi(ui)
    , mTable(*ui->ClientsTable)
{
    connect(ui->ClientsAddButton, SIGNAL(pressed()), this, SLOT(addPressed()));
    connect(ui->ClientsTable, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(displayMenu(QPoint)));
    connect(ui->RentalOffice, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    printDefaultTable();
    printClients();
}

void ClientsTab::addPressed()
{
    AddClient addClient;
    addClient.exec();
}

void ClientsTab::tabChanged(int)
{
    printDefaultTable();
    printClients();
}

void ClientsTab::displayMenu(QPoint pos)
{
    qDebug("display additional menu pressed");
    QMenu menu(&(mTable));

    QAction* remove = menu.addAction("Usun");
    QAction* edit   = menu.addAction("Edytuj");
    QAction* a      = menu.exec(mUi->equipmentTable->viewport()->mapToGlobal(pos));
    if (a == remove)
    {
        int row = mTable.currentRow();
        qDebug("remove choosed row index=%d", row); // check if index is higher then 0
                                                    // if table is empty index is -1
    }
    if (a == edit)
    {
        int row = mTable.currentRow();
        qDebug("edit choosed row index=%d", row);
    }
}

void ClientsTab::printDefaultTable(void)
{
    mTable.setSelectionBehavior(QAbstractItemView::SelectRows);
    mTable.setSelectionMode(QAbstractItemView::SingleSelection);
    mTable.setContextMenuPolicy(Qt::CustomContextMenu);

    int columnCnt = 0;
    mTable.setFont(QFont(QString("Arial"), 14));

    mTable.setColumnCount(columnCnt);
    mTable.setRowCount(0);

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Imie"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwisko"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Adres"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (i == 2)
            mTable.setColumnWidth(i, mTable.width() / 2 - 4);
        else
            mTable.setColumnWidth(i, (mTable.width() / 2) / (columnCnt - 1));
    }
}

void ClientsTab::printClients()
{
    QVector<Common::ClientDetails> clients; // mDataBaseApi.getOrdersByDate(mSelectedDate);

    for (uint i = 0; i < 7; i++)
    {
        Common::ClientDetails a;
        a.name    = QString("Adam_") + QString::number(i);
        a.surname = QString("Nowak_") + QString::number(i);
        a.adress  = QString("Lotnicza " + QString::number(i));
        clients.push_back(a);
    }

    int rowCnt = 0;
    mTable.setRowCount(0);
    for (Common::ClientDetails client : clients)
    {
        int columnCnt = 0;
        mTable.insertRow(rowCnt);
        mTable.setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.name));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.surname));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.adress));

        rowCnt++;
    }
}

} // namespace Frontend
