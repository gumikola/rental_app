#include "ChooseClient.h"
#include "ui_chooseclientwindow.h"
#include <QDialog>
#include <QMessageBox>

namespace Frontend {

ChooseClient::ChooseClient()
    : mUi(new Ui::ChooseClientWindow)
{
    mUi->setupUi(&mDialog);
    connect(mUi->addButton, SIGNAL(pressed()), this, SLOT(addPressed()));
    connect(mUi->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));
    connect(mUi->surnameFiltr, SIGNAL(textChanged(const QString&)), this,
            SLOT(filterChanged(const QString&)));
    mDialog.open();

    printDefaultTable();
    fillClientsList();
    printFiltredClients(QString());
}

void ChooseClient::fillClientsList()
{
    for (uint i = 0; i < 7; i++)
    {
        Common::ClientDetails a;
        a.name    = QString("Adam_") + QString::number(i);
        a.surname = QString("Nowak_") + QString::number(i);
        a.adress  = QString("Lotnicza " + QString::number(i));
        mClients.push_back(a);
    }
}

void ChooseClient::printDefaultTable()
{
    mUi->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    mUi->table->setSelectionMode(QAbstractItemView::SingleSelection);
    mUi->table->setContextMenuPolicy(Qt::CustomContextMenu);

    int columnCnt = 0;
    mUi->table->setFont(QFont(QString("Arial"), 14));

    mUi->table->setColumnCount(columnCnt);
    mUi->table->setRowCount(0);

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Imie"));

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Nazwisko"));

    mUi->table->insertColumn(columnCnt);
    mUi->table->setHorizontalHeaderItem(columnCnt++, new QTableWidgetItem("Adres"));

    for (int i = 0; i < columnCnt; i++)
    {
        if (i == 2)
            mUi->table->setColumnWidth(i, mUi->table->width() / 2 - 4);
        else
            mUi->table->setColumnWidth(i, (mUi->table->width() / 2) / (columnCnt - 1));
    }
}

void ChooseClient::printFiltredClients(const QString& filter)
{
    int rowCnt = 0;
    mUi->table->setRowCount(0);
    for (Common::ClientDetails client : mClients)
    {
        if (client.surname.contains(filter))
        {
            int columnCnt = 0;
            mUi->table->insertRow(rowCnt);
            mUi->table->setVerticalHeaderItem(rowCnt, new QTableWidgetItem());
            mUi->table->setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.name));
            mUi->table->setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.surname));
            mUi->table->setItem(rowCnt, columnCnt++, new QTableWidgetItem(client.adress));

            rowCnt++;
        }
    }
}

void ChooseClient::exec()
{
    mDialog.exec();
}

void ChooseClient::addPressed()
{
    qDebug("%s", __func__);

    try
    {
        int row = mUi->table->currentRow();

        if (row < 0)
            throw QString("Nie wybrano clienta!");

        Common::ClientDetails tmp;
        tmp.name    = mUi->table->item(row, 0)->text();
        tmp.surname = mUi->table->item(row, 1)->text();
        tmp.adress  = mUi->table->item(row, 2)->text();

        // #TODO send struct to database

        emit clientChosed(tmp);
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

void ChooseClient::cancelPressed()
{
    mDialog.close();
}

void ChooseClient::filterChanged(const QString& text)
{
    qDebug("%s zmieniono na: %s", __func__, text.toStdString().c_str());
    printFiltredClients(text);
}

} // namespace Frontend
