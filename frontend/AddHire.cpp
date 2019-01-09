#include "AddHire.h"
#include "AddClient.h"
#include "ChooseClient.h"
#include "ChooseEquipment.h"
#include "backend/Database.h"
#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include <QMessageBox>

namespace Frontend {

AddHire::AddHire(Backend::Database& database)
    : mUi(new Ui::hireEquipment)
    , mDatabase(database)
{
    mUi->setupUi(&mDialog);

    mDialog.open();

    printDefaultClientTable();
    printDefaultEquipmentTable();

    connect(mUi->chooseClientFromDatabase, SIGNAL(pressed()), this,
            SLOT(chooseClientFromDatabasePressed()));
    connect(mUi->addNewClient, SIGNAL(pressed()), this, SLOT(addNewClientPressed()));
    connect(mUi->chooseEquipment, SIGNAL(pressed()), this, SLOT(chooseEquipmentPressed()));
    connect(mUi->hireButton, SIGNAL(pressed()), this, SLOT(hirePressed()));
    connect(mUi->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));

    mDialog.exec();
}

void AddHire::chooseClientFromDatabasePressed()
{
    qDebug("%s", __func__);
    ChooseClient window(mDatabase);
    connect(&window, SIGNAL(clientChosed(Common::ClientDetails&)), this,
            SLOT(clientChoosed(Common::ClientDetails&)));
    window.exec();
}

void AddHire::addNewClientPressed()
{
    qDebug("%s", __func__);
    AddClient window(mDatabase);

    connect(&window, SIGNAL(clientAdded(Common::ClientDetails&)), this,
            SLOT(clientChoosed(Common::ClientDetails&)));

    window.exec();
}

void AddHire::chooseEquipmentPressed()
{
    qDebug("%s", __func__);
    ChooseEquipment window(mDatabase);
    connect(&window, SIGNAL(equipmentChosen(const Common::EquipmentParameters&)), this,
            SLOT(equipmentChosen(const Common::EquipmentParameters&)));
    window.exec();
}

void AddHire::hirePressed()
{
    qDebug("%s", __func__);
    try
    {
        if (mUi->choosenClientTable->rowCount() == 0)
            throw QString("Nie wybrano klienta");
        if (mUi->choosenEquipmentTable->rowCount() == 0)
            throw QString("Nie wybrano urządzenia");

        Common::RentDetails tmp;
        tmp.client     = mChosenClient;
        tmp.equipment  = mChosenEq;
        tmp.rentDate   = QDateTime::currentDateTime();
        tmp.returnDate = QDateTime();

        mDatabase.addHire(tmp);

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

void AddHire::cancelPressed()
{
    qDebug("%s", __func__);
    mDialog.close();
}

void AddHire::clientChoosed(Common::ClientDetails& client)
{
    qDebug("%s", __func__);
    mChosenClient = client;
    printChoosenClient(client);
}

void AddHire::printEquipment(const Common::EquipmentParameters& eq)
{
    int rowCnt = 0;
    mUi->choosenEquipmentTable->setRowCount(0);

    int columnCnt = 0;
    mUi->choosenEquipmentTable->insertRow(rowCnt);
    mUi->choosenEquipmentTable->setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
    mUi->choosenEquipmentTable->setItem(rowCnt, columnCnt++, new QTableWidgetItem(eq.producer));
    mUi->choosenEquipmentTable->setItem(rowCnt, columnCnt++, new QTableWidgetItem(eq.name));
    mUi->choosenEquipmentTable->setItem(rowCnt, columnCnt++,
                                        new QTableWidgetItem(QString::number(eq.price)));
    mUi->choosenEquipmentTable->setItem(rowCnt, columnCnt++,
                                        new QTableWidgetItem(QString::number(eq.pledge)));
}

void AddHire::equipmentChosen(const Common::EquipmentParameters& eq)
{
    qDebug("%s", __func__);

    mChosenEq = eq;

    printEquipment(eq);
}

void AddHire::printChoosenClient(Common::ClientDetails& client)
{
    int rowCnt = 0;
    mUi->choosenClientTable->setRowCount(0);

    int columnCnt = 0;
    mUi->choosenClientTable->insertRow(rowCnt);
    mUi->choosenClientTable->setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
    mUi->choosenClientTable->setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.name));
    mUi->choosenClientTable->setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.surname));
    mUi->choosenClientTable->setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.adress));

    rowCnt++;
}

void AddHire::printDefaultEquipmentTable(void)
{
    mUi->choosenEquipmentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    mUi->choosenEquipmentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mUi->choosenEquipmentTable->setContextMenuPolicy(Qt::CustomContextMenu);

    int columnCnt = 0;
    mUi->choosenEquipmentTable->setFont(QFont(QString("Arial"), 14));

    mUi->choosenEquipmentTable->setColumnCount(columnCnt);
    mUi->choosenEquipmentTable->setRowCount(0);

    mUi->choosenEquipmentTable->insertColumn(columnCnt);
    mUi->choosenEquipmentTable->setHorizontalHeaderItem(columnCnt++,
                                                        new QTableWidgetItem("Producent"));

    mUi->choosenEquipmentTable->insertColumn(columnCnt);
    mUi->choosenEquipmentTable->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwa"));

    mUi->choosenEquipmentTable->insertColumn(columnCnt);
    mUi->choosenEquipmentTable->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Cena"));

    mUi->choosenEquipmentTable->insertColumn(columnCnt);
    mUi->choosenEquipmentTable->setHorizontalHeaderItem(columnCnt++,
                                                        new QTableWidgetItem("Zastaw"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (i == 0 or i == 1)
            mUi->choosenEquipmentTable->setColumnWidth(i, mUi->choosenEquipmentTable->width() / 4 -
                                                              24);
        else
            mUi->choosenEquipmentTable->setColumnWidth(
                i, (mUi->choosenEquipmentTable->width() / 2) / (columnCnt - 2));
    }
}

void AddHire::printDefaultClientTable(void)
{
    mUi->choosenClientTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    mUi->choosenClientTable->setSelectionMode(QAbstractItemView::SingleSelection);
    mUi->choosenClientTable->setContextMenuPolicy(Qt::CustomContextMenu);

    int columnCnt = 0;
    mUi->choosenClientTable->setFont(QFont(QString("Arial"), 14));

    mUi->choosenClientTable->setColumnCount(columnCnt);
    mUi->choosenClientTable->setRowCount(0);

    mUi->choosenClientTable->insertColumn(columnCnt);
    mUi->choosenClientTable->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Imie"));

    mUi->choosenClientTable->insertColumn(columnCnt);
    mUi->choosenClientTable->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwisko"));

    mUi->choosenClientTable->insertColumn(columnCnt);
    mUi->choosenClientTable->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Adres"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (i == 2)
            mUi->choosenClientTable->setColumnWidth(i, mUi->choosenClientTable->width() / 2 - 24);
        else
            mUi->choosenClientTable->setColumnWidth(i, (mUi->choosenClientTable->width() / 2) /
                                                           (columnCnt - 1));
    }
}
} // namespace Frontend
