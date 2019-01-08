#include "RentsTab.h"
#include "AddHire.h"
#include "common.h"

namespace Frontend {

RentsTab::RentsTab(Ui::MainWindow* ui)
    : mUi(ui)
    , mRentalBox(*ui->RentsTabRental)
    , mEquipmentType(*ui->RentsTabEquipmentType)
    , mTable(*ui->RentsTabTable)
    , mAddHire(*ui->RentsTabAddHire)
{
    connect(ui->RentsTabRental, SIGNAL(activated(int)), this, SLOT(rentalChanged(int)));
    connect(ui->RentsTabEquipmentType, SIGNAL(activated(int)), this,
            SLOT(equipmentTypeChanged(int)));
    connect(ui->RentsTabAddHire, SIGNAL(pressed()), this, SLOT(addPressed()));
    connect(ui->RentsTabTable, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(displayMenu(QPoint)));
    connect(ui->RentalOffice, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));

    printDefaultTable();
    Common::fillEquipmentTypeComboBox(mEquipmentType);
}

void RentsTab::rentalChanged(int index)
{
    qDebug("rentalChanged to: %d", index);
    if (index)
    {
        mEquipmentType.setEnabled(true);
    }
    else
    {
        mEquipmentType.setEnabled(false);
        mEquipmentType.setCurrentIndex(0);
        printDefaultTable();
    }
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

    AddHire window;
}

void RentsTab::displayMenu(QPoint pos)
{
    qDebug("displayMenu requested");
    QMenu menu(&(mTable));

    QAction* remove = menu.addAction("Usun");
    QAction* edit   = menu.addAction("Edytuj");
    QAction* a      = menu.exec(mUi->equipmentTable->viewport()->mapToGlobal(pos));
    if (a == remove)
    {
        int row = mTable.currentRow();
        qDebug("remove choosed row index=%d", row);
    }
    if (a == edit)
    {
        int row = mTable.currentRow();
        qDebug("edit choosed row index=%d", row);
    }
}

void RentsTab::tabChanged(int)
{
    printDefaultTable();
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
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwa narzędzia"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Ilość"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Imie"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwisko"));

    for (int i = 0; i < columnCnt; i++)
    {
        mTable.setColumnWidth(i, mTable.width() / columnCnt - 2);
    }
}

void RentsTab::printRents()
{
    QVector<Common::RentDetails> rents; // mDataBaseApi.getOrdersByDate(mSelectedDate);

    for (uint i = 0; i < 7; i++)
    {
        Common::RentDetails a;
        a.amount        = i;
        a.equipmentName = QString("wiertnica");
        a.name          = QString("Jan_" + QString::number(i));
        a.surname       = QString("Kowalski_" + QString::number(i));
        rents.push_back(a);
    }

    int rowCnt = 0;
    mTable.setRowCount(0);
    for (Common::RentDetails rent : rents)
    {
        int columnCnt = 0;
        mTable.insertRow(rowCnt);
        mTable.setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(rent.equipmentName));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(rent.amount)));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString(rent.name)));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString(rent.surname)));

        rowCnt++;
    }
}

} // namespace Frontend
