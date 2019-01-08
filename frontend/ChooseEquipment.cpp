#include "ChooseEquipment.h"
#include <QMessageBox>

namespace Frontend {

ChooseEquipment::ChooseEquipment()
    : mUi(new Ui::ChooseEquipment)
{
    mUi->setupUi(&mDialog);

    connect(mUi->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));
    connect(mUi->addButton, SIGNAL(pressed()), this, SLOT(addPressed()));
    connect(mUi->equipmentType, SIGNAL(activated(int)), this, SLOT(equipmentTypeChanged(int)));
    Common::fillEquipmentTypeComboBox(*mUi->equipmentType);
    printDefaultTable();
    mDialog.open();
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

        if (row <= 0)
            throw QString("Nie wybrano urządzenia/sprzętu");
        if (mUi->equipmentType->currentIndex() <= 0)
            throw QString("Nie wybrano typu sprzętu");

        Common::EquipmentParameters tmp;

        tmp.amount = 1;
        tmp.name   = mUi->table->item(row, 0)->text();
        tmp.price  = mUi->table->item(row, 1)->text().toDouble();
        tmp.pledge = mUi->table->item(row, 2)->text().toDouble();
        tmp.type   = static_cast<Common::EquipmentType>(mUi->equipmentType->currentIndex());

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
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwa"));

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Cena"));

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Kwota zastawu"));

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Ilosc"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (not i)
            mUi->table->setColumnWidth(i, mUi->table->width() / 2 - 4);
        else
            mUi->table->setColumnWidth(i, (mUi->table->width() / 2) / (columnCnt - 1));
    }
}

void ChooseEquipment::printEquipment()
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
    mUi->table->setRowCount(0);
    for (Common::EquipmentParameters eq : equipment)
    {
        int columnCnt = 0;
        mUi->table->insertRow(rowCnt);
        mUi->table->setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
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
