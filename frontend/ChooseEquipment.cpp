#include "ChooseEquipment.h"
#include "backend/Database.h"
#include <QMessageBox>

namespace Frontend {

ChooseEquipment::ChooseEquipment(Backend::Database& database)
    : mUi(new Ui::ChooseEquipment)
    , mDatabase(database)
{
    mUi->setupUi(&mDialog);

    connect(mUi->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));
    connect(mUi->addButton, SIGNAL(pressed()), this, SLOT(addPressed()));
    connect(mUi->equipmentType, SIGNAL(activated(int)), this, SLOT(equipmentTypeChanged(int)));
    Common::fillEquipmentTypeComboBox(*mUi->equipmentType);
    mDialog.open();
    printDefaultTable();
}

void ChooseEquipment::exec()
{
    mDialog.exec();
}

void ChooseEquipment::addPressed()
{
    qDebug("%s", __func__);

    int row = mUi->table->currentRow();

    try
    {

        if (row < 0)
            throw QString("Nie wybrano urządzenia/sprzętu");

        if (mUi->equipmentType->currentIndex() <= 0)
            throw QString("Nie wybrano typu sprzętu");

        Common::EquipmentParameters tmp;

        tmp.amount   = 1;
        tmp.producer = mUi->table->item(row, 0)->text();
        tmp.name     = mUi->table->item(row, 1)->text();
        tmp.price    = mUi->table->item(row, 2)->text().toDouble();
        tmp.pledge   = mUi->table->item(row, 3)->text().toDouble();
        tmp.type     = static_cast<Common::EquipmentType>(mUi->equipmentType->currentIndex());

        emit equipmentChosen(tmp);
        mDialog.close();
    }
    catch (QString& e)
    {
        QMessageBox msgBox;
        msgBox.setFont(QFont(QString("Arial"), 14));
        msgBox.setText(e);
        msgBox.exec();
    }
}

void ChooseEquipment::cancelPressed()
{
    qDebug("%s", __func__);
    mDialog.close();
}

void ChooseEquipment::printDefaultTable(void)
{
    mUi->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    mUi->table->setSelectionMode(QAbstractItemView::SingleSelection);
    mUi->table->setContextMenuPolicy(Qt::CustomContextMenu);

    int columnCnt = 0;
    mUi->table->setFont(QFont(QString("Arial"), 14));

    mUi->table->setColumnCount(columnCnt);
    mUi->table->setRowCount(0);

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Producent"));

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwa"));

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Cena"));

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Kwota zastawu"));

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Ilosc"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (i == 0 or i == 1)
            mUi->table->setColumnWidth(i, mUi->table->width() / 4 - 4);
        else
            mUi->table->setColumnWidth(i, (mUi->table->width() / 2) / (columnCnt - 2));
    }
}

void ChooseEquipment::printEquipment()
{
    QVector<Common::EquipmentParameters> equipment = mDatabase.getEquipment(
        static_cast<Common::EquipmentType>(mUi->equipmentType->currentIndex()), RENTAL_ID);

    int rowCnt = 0;
    mUi->table->setRowCount(0);
    for (Common::EquipmentParameters eq : equipment)
    {
        int columnCnt = 0;
        mUi->table->insertRow(rowCnt);
        mUi->table->setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
        mUi->table->setItem(rowCnt, columnCnt++, new QTableWidgetItem(eq.producer));
        mUi->table->setItem(rowCnt, columnCnt++, new QTableWidgetItem(eq.name));
        mUi->table->setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(eq.price)));
        mUi->table->setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(eq.pledge)));
        mUi->table->setItem(rowCnt, columnCnt++, new QTableWidgetItem(QString::number(eq.amount)));

        rowCnt++;
    }
}

void ChooseEquipment::equipmentTypeChanged(int index)
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

} // namespace Frontend
