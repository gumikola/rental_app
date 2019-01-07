#pragma once
#include "ui_mainwindow.h"
#include <QComboBox>
#include <QObject>
#include <QPushButton>
#include <QTableWidget>

namespace Frontend {

class Equipment : public QObject
{
    Q_OBJECT

    Ui::MainWindow* mUi;
    QComboBox&      mRentalBox;
    QComboBox&      mEquipmentType;
    QTableWidget&   mTable;
    QPushButton&    mAddEquipment;

    void printDefaultTable(void);
    void printEquipment();
    void fillEquipmentTypeComboBox();

public:
    explicit Equipment(Ui::MainWindow* ui);

signals:

public slots:
    void rentalChanged(int);
    void equipmentTypeChanged(int);
    void displayMenu(QPoint pos);
    void addEqPressed();
    void tabChanged(int);
};
} // namespace Frontend
