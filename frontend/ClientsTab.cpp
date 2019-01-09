#include "ClientsTab.h"
#include "AddClient.h"
#include "EditClientDetails.h"
#include "backend/Database.h"
#include "common.h"
#include <QPoint>

namespace Frontend {

ClientsTab::ClientsTab(Ui::MainWindow* ui, Backend::Database& database)
    : mUi(ui)
    , mTable(*ui->ClientsTable)
    , mDatabase(database)
{
    connect(ui->ClientsAddButton, SIGNAL(pressed()), this, SLOT(addPressed()));
    connect(ui->ClientsTable, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(displayMenu(QPoint)));
    connect(ui->RentalOffice, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    connect(mUi->ClientsTabSurnameFiltr, SIGNAL(textChanged(const QString&)), this,
            SLOT(filterChanged(const QString&)));

    printDefaultTable();
    printClients(mDatabase.getClients());
}

void ClientsTab::addPressed()
{
    AddClient addClient(mDatabase);
    addClient.exec();
    printClients(mDatabase.getClients());
}

void ClientsTab::tabChanged(int)
{
    printDefaultTable();
    printClients(mDatabase.getClients());
    mUi->ClientsTabSurnameFiltr->clear();
}

void ClientsTab::filterChanged(const QString& text)
{
    qDebug("%s zmieniono na: %s", __func__, text.toStdString().c_str());
    printFiltredClients(text, mDatabase.getClients());
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
        Common::ClientDetails tmp;
        tmp.name    = mTable.item(row, 0)->text();
        tmp.surname = mTable.item(row, 1)->text();
        tmp.adress  = mTable.item(row, 2)->text();

        mDatabase.removeClient(tmp);
        printClients(mDatabase.getClients());
    }
    if (a == edit)
    {
        int row = mTable.currentRow();
        qDebug("edit choosed row index=%d", row);

        Common::ClientDetails tmp;
        tmp.name    = mTable.item(row, 0)->text();
        tmp.surname = mTable.item(row, 1)->text();
        tmp.adress  = mTable.item(row, 2)->text();

        EditClientDetails window(tmp, mDatabase);
        window.exec();

        printClients(mDatabase.getClients());
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
            mTable.setColumnWidth(i, mTable.width() / 2 - 22);
        else
            mTable.setColumnWidth(i, (mTable.width() / 2) / (columnCnt - 1));
    }
}

void ClientsTab::printFiltredClients(const QString& filter, QVector<Common::ClientDetails> clients)
{
    int rowCnt = 0;
    mTable.setRowCount(0);
    for (Common::ClientDetails client : clients)
    {
        if (client.surname.contains(
                QRegularExpression(filter, QRegularExpression::CaseInsensitiveOption)))
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
}

void ClientsTab::printClients(QVector<Common::ClientDetails> clients)
{
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
