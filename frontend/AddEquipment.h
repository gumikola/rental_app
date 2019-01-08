#pragma once

#include "common.h"
#include "ui_addequipment.h"
#include <QObject>

namespace Frontend {

class AddEquipment : public QObject
{
    Q_OBJECT
    Ui::AddEquipment* mUi;
    QDialog           mDialog;

    Common::EquipmentType getEquipmentType();
    QString               getName();
    double                getPrice();
    double                getPledge();
    uint                  getAmount();

public:
    AddEquipment();

public slots:
    void addEquipmentPressed();
    void cancelPressed();
};

} // namespace Frontend
