#pragma once

#include "ui_mainwindow.h"
#include <QComboBox>
#include <QObject>
#include <QPushButton>
#include <QTableWidget>

namespace Frontend {

class RentsTab : public QObject
{
    Q_OBJECT

    Ui::MainWindow* mUi;
    QComboBox&      mRentalBox;
    QComboBox&      mEquipmentType;
    QTableWidget&   mTable;
    QPushButton&    mAddHire;

    void printDefaultTable(void);
    void printRents();

public:
    explicit RentsTab(Ui::MainWindow* ui);

signals:

public slots:
    void rentalChanged(int index);
    void equipmentTypeChanged(int index);
    void addPressed();
    void displayMenu(QPoint pos);
    void tabChanged(int);
};

} // namespace Frontend
