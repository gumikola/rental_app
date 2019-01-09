#include "RentsTab.h"
#include "AddHire.h"
#include "backend/Database.h"
#include "common.h"

namespace Frontend {

RentsTab::RentsTab(Ui::MainWindow* ui, Backend::Database& database)
    : mUi(ui)
    , mEquipmentType(*ui->RentsTabEquipmentType)
    , mTable(*ui->RentsTabTable)
    , mAddHire(*ui->RentsTabAddHire)
    , mDatabase(database)
{
    connect(ui->RentsTabEquipmentType, SIGNAL(activated(int)), this,
            SLOT(equipmentTypeChanged(int)));
    connect(ui->RentsTabAddHire, SIGNAL(pressed()), this, SLOT(addPressed()));
    connect(ui->RentsTabTable, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(displayMenu(QPoint)));
    connect(ui->RentalOffice, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    printDefaultTable();
    Common::fillEquipmentTypeComboBox(mEquipmentType);
}

void RentsTab::equipmentTypeChanged(int index)
{
    qDebug("equipmentTypeChanged to: %d", index);
    if (index)
    {
        // get data from database and display table
        printRents();
    }
    else
    {
        printDefaultTable();
    }
}

void RentsTab::addPressed()
{
    qDebug("addPressed()");

    AddHire window(mDatabase);

    printRents();
}

void RentsTab::displayMenu(QPoint pos)
{
    qDebug("displayMenu requested");
    QMenu menu(&(mTable));

    QAction* completed = menu.addAction("Oznacz jako zakonczone");
    QAction* remove    = menu.addAction("Usun");
    QAction* a         = menu.exec(mUi->equipmentTable->viewport()->mapToGlobal(pos));
    if (a == remove)
    {
        int row = mTable.currentRow();
        qDebug("remove choosed row index=%d", row);

        mDatabase.removeHire(mRents.at(row));

        printRents();
    }
    if (a == completed)
    {
        int row = mTable.currentRow();
        qDebug("closed choosed row index=%d", row);

        mDatabase.markAsCompleted(mRents.at(row));

        printRents();
    }
}

void RentsTab::tabChanged(int)
{
    printDefaultTable();
    printRents();
}

void RentsTab::printDefaultTable(void)
{
    mTable.setSelectionBehavior(QAbstractItemView::SelectRows);
    mTable.setSelectionMode(QAbstractItemView::SingleSelection);
    mTable.setContextMenuPolicy(Qt::CustomContextMenu);

    int columnCnt = 0;
    mTable.setFont(QFont(QString("Arial"), 14));

    mTable.setColumnCount(columnCnt);
    mTable.setRowCount(0);

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Data\nwypożyczenia"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Typ\nurządzenia"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwa\nurządzenia"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Imię"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwisko"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (not i)
            mTable.setColumnWidth(i, mTable.width() / 3 - 6);
        else
            mTable.setColumnWidth(i, (mTable.width() - mTable.width() / 3) / (columnCnt - 1));
    }
}

void RentsTab::printRents()
{
    if (mEquipmentType.currentIndex() <= 0)
    {
        printDefaultTable();
        return;
    }

    mRents = mDatabase.getRents(static_cast<Common::EquipmentType>(mEquipmentType.currentIndex()));

    int rowCnt = 0;
    mTable.setRowCount(0);
    for (Common::RentDetails rent : mRents)
    {
        int columnCnt = 0;
        mTable.insertRow(rowCnt);
        mTable.setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
        mTable.setItem(rowCnt, columnCnt++,
                       new QTableWidgetItem(rent.rentDate.toString("hh:mm     dd.MM.yyyy")));
        mTable.setItem(rowCnt, columnCnt++,
                       new QTableWidgetItem(Common::equipmentTypeToString(rent.equipment.type)));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(rent.equipment.name));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(rent.client.name));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(rent.client.surname));

        rowCnt++;
    }
}

} // namespace Frontend
