#include "ChooseClient.h"
#include "backend/Database.h"
#include "ui_chooseclientwindow.h"
#include <QDialog>
#include <QMessageBox>

namespace Frontend {

ChooseClient::ChooseClient(Backend::Database& database)
    : mUi(new Ui::ChooseClientWindow)
    , mDatabase(database)

{
    mUi->setupUi(&mDialog);
    connect(mUi->addButton, SIGNAL(pressed()), this, SLOT(addPressed()));
    connect(mUi->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));
    connect(mUi->surnameFiltr, SIGNAL(textChanged(const QString&)), this,
            SLOT(filterChanged(const QString&)));
    mDialog.open();

    printDefaultTable();
    printFiltredClients(QString());
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
    QVector<Common::ClientDetails> mClients = mDatabase.getClients();

    int rowCnt = 0;

    mUi->table->setRowCount(0);
    for (Common::ClientDetails client : mClients)
    {
        if (client.surname.contains(
                QRegularExpression(filter, QRegularExpression::CaseInsensitiveOption)))
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
