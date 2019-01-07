#include "Equipment.h"
#include "AddEquipment.h"
#include "backend/Database.h"
#include "common.h"
#include "ui_addequipment.h"
#include <QFont>
#include <QMenu>

namespace Frontend {

Equipment::Equipment(Ui::MainWindow* ui)
    : mUi(ui)
    , mRentalBox(*ui->equipmentRentalBox)
    , mEquipmentType(*ui->equipmentType)
    , mTable(*ui->equipmentTable)
    , mAddEquipment(*ui->equipmentTabAddButton)
{
    connect(ui->equipmentRentalBox, SIGNAL(activated(int)), this, SLOT(rentalChanged(int)));
    connect(ui->equipmentType, SIGNAL(activated(int)), this, SLOT(equipmentTypeChanged(int)));
    connect(ui->equipmentTabAddButton, SIGNAL(pressed()), this, SLOT(addEqPressed()));
    connect(ui->equipmentTable, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(displayMenu(QPoint)));
    connect(ui->Rental, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    printDefaultTable();
    fillEquipmentTypeComboBox();
}

void Equipment::rentalChanged(int index)
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

void Equipment::equipmentTypeChanged(int index)
{
    qDebug("equipmentTypeChanged to: %d", index);
    if (index)
    {
        // get data from database and display table
        printEquipment();
    }
    else
    {
        printDefaultTable();
    }
}

void Equipment::displayMenu(QPoint pos)
{
    qDebug("No siem jestem");
    QMenu menu(&(mTable));

    QAction* remove       = menu.addAction("Usun");
    QAction* changeAmount = menu.addAction("Zmien ilosc");
    QAction* a            = menu.exec(mUi->equipmentTable->viewport()->mapToGlobal(pos));
    if (a == remove)
    {
        int row = mTable.currentRow();
        qDebug("remove choosed row index=%d", row);
    }
    if (a == changeAmount)
    {
        int row = mTable.currentRow();
        qDebug("changeAmount choosed row index=%d", row);
    }
}

void Equipment::addEqPressed()
{
    qDebug("addEqPressed()");

    AddEquipment window;
}

void Equipment::tabChanged(int)
{
    printDefaultTable();
}

void Equipment::printDefaultTable(void)
{
    mTable.setSelectionBehavior(QAbstractItemView::SelectRows);
    mTable.setSelectionMode(QAbstractItemView::SingleSelection);
    mTable.setContextMenuPolicy(Qt::CustomContextMenu);

    int columnCnt = 0;
    mTable.setFont(QFont(QString("Arial"), 14));

    mTable.setColumnCount(columnCnt);
    mTable.setRowCount(0);

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwa"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Cena"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Kwota zastawu"));

    mTable.insertColumn(columnCnt);
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Ilosc"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (not i)
            mTable.setColumnWidth(i, mTable.width() / 2 - 4);
        else
            mTable.setColumnWidth(i, (mTable.width() / 2) / (columnCnt - 1));
    }
}

void Equipment::printEquipment()
{
    QVector<Common::EquipmentParameters> equipment; // mDataBaseApi.getOrdersByDate(mSelectedDate);

    for (uint i = 0; i < 7; i++)
    {
        Common::EquipmentParameters a;
        a.name   = QString("narzedzie") + QString::number(i);
        a.price  = 12;
        a.pledge = 100;
        a.amount = i + 2;
        equipment.push_back(a);
    }

    int rowCnt = 0;
    mTable.setRowCount(0);
    for (Common::EquipmentParameters eq : equipment)
    {
        int columnCnt = 0;
        mTable.insertRow(rowCnt);
        mTable.setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(eq.name));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(eq.price)));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(eq.pledge)));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(eq.amount)));

        rowCnt++;
    }
}

void Equipment::fillEquipmentTypeComboBox()
{
    mEquipmentType.clear();

    for (const auto e : Common::All)
        mEquipmentType.addItem(Common::equipmentTypeToString(e));
}
} // namespace Frontend
