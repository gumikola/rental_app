#pragma once

#include "common.h"
#include "ui_chooseequipment.h"
#include <QDialog>
#include <QObject>

namespace Frontend {

class ChooseEquipment : public QObject
{
    Q_OBJECT

    Ui::ChooseEquipment* mUi;
    QDialog              mDialog;

    void printDefaultTable(void);
    void printEquipment();

public:
    explicit ChooseEquipment();
    void exec();

signals:
    void equipmentChosen(Common::EquipmentParameters& eq);

public slots:
    void addPressed();
    void cancelPressed();
    void equipmentTypeChanged(int index);
};
} // namespace Frontend
