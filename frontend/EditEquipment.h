#pragma once

#include "common.h"
#include "ui_editequipment.h"
#include <QObject>

namespace Backend {
class Database;
} // namespace Backend

namespace Frontend {

class EditEquipment : public QObject
{
    Q_OBJECT
    Ui::EditEquipment*                 mUi;
    QDialog                            mDialog;
    Backend::Database&                 mDatabase;
    const Common::EquipmentParameters& mEquipment;

    Common::EquipmentType getEquipmentType();
    QString               getName();
    QString               getProducer();
    double                getPrice();
    double                getPledge();
    uint                  getAmount();
    void                  fillWidowData();

public:
    explicit EditEquipment(Backend::Database&           database,
                           Common::EquipmentParameters& choosenEquipment);

signals:

public slots:
    void savePressed();
    void cancelPressed();
    void exec();
};

} // namespace Frontend
