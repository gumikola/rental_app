#pragma once

#include "common.h"
#include "ui_addhire.h"
#include <QObject>

namespace Frontend {

class AddHire : public QObject
{
    Q_OBJECT

    Ui::hireEquipment* mUi;
    QDialog            mDialog;

    void printDefaultEquipmentTable(void);
    void printDefaultClientTable(void);
    void printChoosenClient(Common::ClientDetails& client);
    void printEquipment(Common::EquipmentParameters& eq);

public:
    explicit AddHire();

signals:

public slots:
    void chooseClientFromDatabasePressed();
    void addNewClientPressed();
    void chooseEquipmentPressed();
    void hirePressed();
    void cancelPressed();
    void clientChoosed(Common::ClientDetails& client);
    void equipmentChosen(Common::EquipmentParameters& eq);
};

} // namespace Frontend
