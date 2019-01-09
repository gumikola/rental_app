#pragma once
#include "common.h"
#include "ui_mainwindow.h"
#include <QComboBox>
#include <QObject>
#include <QPushButton>
#include <QTableWidget>

namespace Backend {
class Database;
} // namespace Backend

namespace Frontend {

class Equipment : public QObject
{
    Q_OBJECT

    Ui::MainWindow*    mUi;
    QComboBox&         mRentalBox;
    QComboBox&         mEquipmentType;
    QTableWidget&      mTable;
    QPushButton&       mAddEquipment;
    Backend::Database& mDatabase;

    void printDefaultTable(void);
    void printEquipment(const Common::EquipmentType& type, uint rental_id);
    void printFiltredEquipment(const QString& filter);

public:
    explicit Equipment(Ui::MainWindow* ui, Backend::Database& database);

signals:

public slots:
    void rentalChanged(int);
    void equipmentTypeChanged(int);
    void displayMenu(QPoint pos);
    void addEqPressed();
    void tabChanged(int);
    void filterChanged(const QString& text);
};
} // namespace Frontend
