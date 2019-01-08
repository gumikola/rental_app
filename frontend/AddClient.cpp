#include "AddClient.h"
#include <QDialog>
#include <QMessageBox>

namespace Frontend {

AddClient::AddClient()
    : mUi(new Ui::AddClientWindow)
{
    mUi->setupUi(&mDialog);
    connect(mUi->addButton, SIGNAL(pressed()), this, SLOT(addPressed()));
    connect(mUi->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));
    mDialog.open();
}

void AddClient::exec()
{
    mDialog.exec();
}

void AddClient::addPressed()
{
    qDebug("addPressed()");
    try
    {
        Common::ClientDetails tmp;
        tmp.name    = getName();
        tmp.surname = getSurname();
        tmp.adress  = getAddress();

        // #TODO send struct to database

        emit clientAdded(tmp);
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

void AddClient::cancelPressed()
{
    mDialog.close();
}

QString AddClient::getName()
{
    QString tmp = mUi->name->text();

    if (not tmp.size())
        throw QString("Pole imie nie moze byc puste!");

    return tmp;
}

QString AddClient::getSurname()
{
    QString tmp = mUi->surname->text();

    if (not tmp.size())
        throw QString("Pole nazwisko nie moze byc puste!");

    return tmp;
}

QString AddClient::getAddress()
{
    QString tmp = mUi->adress->text();

    if (not tmp.size())
        throw QString("Pole adres nie moze byc puste!");

    return tmp;
}

} // namespace Frontend
