#include "EditClientDetails.h"
#include "backend/Database.h"
#include <QDialog>
#include <QMessageBox>

namespace Frontend {

EditClientDetails::EditClientDetails(const Common::ClientDetails& client,
                                     Backend::Database&           database)
    : mUi(new Ui::EditClient)
    , mPreClient(client)
    , mDatabase(database)
{
    mUi->setupUi(&mDialog);
    connect(mUi->saveButton, SIGNAL(pressed()), this, SLOT(savePressed()));
    connect(mUi->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));
    fillWindowClientData(client);
    mDialog.open();
}

void EditClientDetails::exec()
{
    mDialog.exec();
}

void EditClientDetails::savePressed()
{
    qDebug("addPressed()");
    try
    {
        Common::ClientDetails tmp;
        tmp.name    = getName();
        tmp.surname = getSurname();
        tmp.adress  = getAddress();

        mDatabase.editClient(mPreClient, tmp);

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

void EditClientDetails::cancelPressed()
{
    mDialog.close();
}

QString EditClientDetails::getName()
{
    QString tmp = mUi->name->text();

    if (not tmp.size())
        throw QString("Pole imie nie moze byc puste!");

    return tmp;
}

QString EditClientDetails::getSurname()
{
    QString tmp = mUi->surname->text();

    if (not tmp.size())
        throw QString("Pole nazwisko nie moze byc puste!");

    return tmp;
}

QString EditClientDetails::getAddress()
{
    QString tmp = mUi->adress->text();

    if (not tmp.size())
        throw QString("Pole adres nie moze byc puste!");

    return tmp;
}

void EditClientDetails::fillWindowClientData(const Common::ClientDetails& client)
{
    mUi->name->setText(client.name);
    mUi->surname->setText(client.surname);
    mUi->adress->setText(client.adress);
}
} // namespace Frontend
