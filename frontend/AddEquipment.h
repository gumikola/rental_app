#pragma once

#include "common.h"
#include "ui_addequipment.h"
#include <QObject>

namespace Backend {
class Database;
} // namespace Backend

namespace Frontend {

class AddEquipment : public QObject
{
    Q_OBJECT
    Ui::AddEquipment*  mUi;
    QDialog            mDialog;
    Backend::Database& mDatabase;

    Common::EquipmentType getEquipmentType();
    QString               getName();
    double                getPrice();
    double                getPledge();
    uint                  getAmount();
    QString               getProducer();

public:
    AddEquipment(Backend::Database& database);

public slots:
    void addEquipmentPressed();
    void cancelPressed();
};

} // namespace Frontend
