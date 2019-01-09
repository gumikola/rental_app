#include "Equipment.h"
#include "AddEquipment.h"
#include "EditEquipment.h"
#include "backend/Database.h"
#include "common.h"
#include "ui_addequipment.h"
#include <QFont>
#include <QMenu>

namespace Frontend {

Equipment::Equipment(Ui::MainWindow* ui, Backend::Database& database)
    : mUi(ui)
    , mRentalBox(*ui->equipmentRentalBox)
    , mEquipmentType(*ui->equipmentType)
    , mTable(*ui->equipmentTable)
    , mAddEquipment(*ui->equipmentTabAddButton)
    , mDatabase(database)
{
    connect(ui->equipmentRentalBox, SIGNAL(activated(int)), this, SLOT(rentalChanged(int)));
    connect(ui->equipmentType, SIGNAL(activated(int)), this, SLOT(equipmentTypeChanged(int)));
    connect(ui->equipmentTabAddButton, SIGNAL(pressed()), this, SLOT(addEqPressed()));
    connect(ui->equipmentTable, SIGNAL(customContextMenuRequested(QPoint)), this,
            SLOT(displayMenu(QPoint)));
    connect(ui->RentalOffice, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    printDefaultTable();
    connect(mUi->EquipmentTabNameFiltr, SIGNAL(textChanged(const QString&)), this,
            SLOT(filterChanged(const QString&)));
    Common::fillEquipmentTypeComboBox(mEquipmentType);
}

void Equipment::rentalChanged(int index)
{
    qDebug("rentalChanged to: %d", index);
    if (index)
    {
        mEquipmentType.setEnabled(true);
        if (mRentalBox.currentIndex() >= 0)
            printEquipment(static_cast<Common::EquipmentType>(mEquipmentType.currentIndex()),
                           static_cast<uint>(mUi->equipmentRentalBox->currentIndex()));
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
        printEquipment(static_cast<Common::EquipmentType>(index),
                       static_cast<uint>(mUi->equipmentRentalBox->currentIndex()));
    }
    else
    {
        printDefaultTable();
    }
}

void Equipment::displayMenu(QPoint pos)
{
    QMenu menu(&(mTable));

    QAction* remove = menu.addAction("Usun");
    QAction* edit   = menu.addAction("Edytuj");
    QAction* a      = menu.exec(mUi->equipmentTable->viewport()->mapToGlobal(pos));
    if (a == remove)
    {
        int row = mTable.currentRow();
        qDebug("remove choosed row index=%d", row);

        Common::EquipmentParameters tmp;
        tmp.producer = mTable.item(row, 0)->text();
        tmp.name     = mTable.item(row, 1)->text();
        tmp.price    = mTable.item(row, 2)->text().toDouble();
        tmp.pledge   = mTable.item(row, 3)->text().toDouble();
        tmp.amount   = mTable.item(row, 4)->text().toUInt();
        tmp.type     = static_cast<Common::EquipmentType>(mUi->equipmentType->currentIndex());
        mDatabase.removeEquipment(tmp);
        printEquipment(static_cast<Common::EquipmentType>(mUi->equipmentType->currentIndex()),
                       static_cast<uint>(mUi->equipmentRentalBox->currentIndex()));
    }
    if (a == edit)
    {
        int row = mTable.currentRow();
        qDebug("edit choosed row index=%d", row);

        Common::EquipmentParameters tmp;
        tmp.producer = mTable.item(row, 0)->text();
        tmp.name     = mTable.item(row, 1)->text();
        tmp.price    = mTable.item(row, 2)->text().toDouble();
        tmp.pledge   = mTable.item(row, 3)->text().toDouble();
        tmp.amount   = mTable.item(row, 4)->text().toUInt();
        tmp.type     = static_cast<Common::EquipmentType>(mUi->equipmentType->currentIndex());

        EditEquipment window(mDatabase, tmp);
        window.exec();
        printEquipment(static_cast<Common::EquipmentType>(mUi->equipmentType->currentIndex()),
                       static_cast<uint>(mUi->equipmentRentalBox->currentIndex()));
    }
}

void Equipment::addEqPressed()
{
    qDebug("addEqPressed()");
    AddEquipment window(mDatabase);

    printEquipment(static_cast<Common::EquipmentType>(mUi->equipmentType->currentIndex()),
                   static_cast<uint>(mUi->equipmentRentalBox->currentIndex()));
}

void Equipment::tabChanged(int)
{
    printDefaultTable();
    printFiltredEquipment(mUi->EquipmentTabNameFiltr->text());
}

void Equipment::filterChanged(const QString& text)
{
    qDebug("%s zmieniono na: %s", __func__, text.toStdString().c_str());
    printFiltredEquipment(text);
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
    mTable.setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Producent"));

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
        if (i == 0 or i == 1)
            mTable.setColumnWidth(i, mTable.width() / 4 - 4);
        else
            mTable.setColumnWidth(i, (mTable.width() / 2) / (columnCnt - 2));
    }
}

void Equipment::printEquipment(const Common::EquipmentType& type, uint rental_id)
{
    if (not rental_id)
    {
        printDefaultTable();
        return;
    }

    QVector<Common::EquipmentParameters> equipment = mDatabase.getEquipment(type, rental_id);

    int rowCnt = 0;
    mTable.setRowCount(0);
    for (Common::EquipmentParameters eq : equipment)
    {
        int columnCnt = 0;
        mTable.insertRow(rowCnt);
        mTable.setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(eq.producer));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(eq.name));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(eq.price)));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(eq.pledge)));
        mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(eq.amount)));

        rowCnt++;
    }
}

void Equipment::printFiltredEquipment(const QString& filter)
{
    if (mEquipmentType.currentIndex() <= 0 and mUi->equipmentRentalBox->currentIndex() <= 0)
    {
        printDefaultTable();
        return;
    }

    QVector<Common::EquipmentParameters> equipment =
        mDatabase.getEquipment(static_cast<Common::EquipmentType>(mEquipmentType.currentIndex()),
                               static_cast<uint>(mUi->equipmentRentalBox->currentIndex()));

    int rowCnt = 0;
    mTable.setRowCount(0);
    for (Common::EquipmentParameters eq : equipment)
    {
        if (eq.name.contains(QRegularExpression(filter, QRegularExpression::CaseInsensitiveOption)))
        {
            int columnCnt = 0;
            mTable.insertRow(rowCnt);
            mTable.setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
            mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(eq.producer));
            mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(eq.name));
            mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(eq.price)));
            mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(eq.pledge)));
            mTable.setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(eq.amount)));

            rowCnt++;
        }
    }
}

} // namespace Frontend
