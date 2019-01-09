#include "EditEquipment.h"
#include "backend/Database.h"
#include <QMessageBox>

namespace Frontend {

EditEquipment::EditEquipment(Backend::Database&           database,
                             Common::EquipmentParameters& choosenEquipment)
    : mUi(new Ui::EditEquipment)
    , mDatabase(database)
    , mEquipment(choosenEquipment)
{
    mUi->setupUi(&mDialog);
    Common::fillEquipmentTypeComboBox(*mUi->equipmentType);
    connect(mUi->saveButton, SIGNAL(pressed()), this, SLOT(savePressed()));
    connect(mUi->cancelButton, SIGNAL(pressed()), this, SLOT(cancelPressed()));

    mDialog.open();

    fillWidowData();
}

Common::EquipmentType EditEquipment::getEquipmentType()
{
    if (not mUi->equipmentType->currentIndex())
        throw QString("Nie wybrano typu urządzenia!");

    return static_cast<Common::EquipmentType>(mUi->equipmentType->currentIndex());
}

QString EditEquipment::getName()
{
    QString tmp = mUi->name->text();

    if (not tmp.size())
        throw QString("Pole nazwa nie moze byc puste!");

    return tmp;
}

QString EditEquipment::getProducer()
{
    QString tmp = mUi->producer->text();

    if (not tmp.size())
        throw QString("Pole producent nie moze byc puste!");

    return tmp;
}

double EditEquipment::getPrice()
{
    QString tmp = mUi->Price->text();

    if (not tmp.size())
        throw QString("Pole cena nie moze byc puste!");

    bool   result = true;
    double price  = tmp.toDouble(&result);

    if (result == false)
        throw QString("Blędna cena! (przykład: 12.5)");

    return price;
}

double EditEquipment::getPledge()
{
    QString tmp = mUi->pledge->text();

    if (not tmp.size())
        throw QString("Pole zysk nie moze byc puste!");

    bool   result = true;
    double pledge = tmp.toDouble(&result);

    if (result == false)
        throw QString("Blędna wartość zysku! (przykład: 150.5 lub 200)");

    return pledge;
}

uint EditEquipment::getAmount()
{
    QString tmp = mUi->amount->text();

    if (not tmp.size())
        throw QString("Pole ilość nie moze byc puste!");

    bool result = true;
    uint amount = tmp.toUInt(&result);

    if (result == false)
        throw QString("Blędna ilość!");

    return amount;
}

void EditEquipment::fillWidowData()
{
    mUi->producer->setText(mEquipment.producer);
    mUi->name->setText(mEquipment.name);
    mUi->Price->setText(QString::number(mEquipment.price));
    mUi->pledge->setText(QString::number(mEquipment.pledge));
    mUi->amount->setText(QString::number(mEquipment.amount));
    mUi->equipmentType->setCurrentIndex(static_cast<int>(mEquipment.type));
}

void EditEquipment::savePressed()
{
    qDebug("addEquipmentPressed");
    try
    {
        Common::EquipmentParameters tmp;
        tmp.type     = getEquipmentType();
        tmp.name     = getName();
        tmp.price    = getPrice();
        tmp.pledge   = getPledge();
        tmp.amount   = getAmount();
        tmp.producer = getProducer();

        mDatabase.editEquipment(mEquipment, tmp);

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

void EditEquipment::cancelPressed()
{
    mDialog.close();
}

void EditEquipment::exec()
{
    mDialog.exec();
}

} // namespace Frontend
