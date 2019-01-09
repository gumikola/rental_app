#pragma once

#include "ui_mainwindow.h"
#include <QComboBox>
#include <QObject>
#include <QPushButton>
#include <QTableWidget>

namespace Backend {
class Database;
} // namespace Backend

namespace Frontend {

class RentsTab : public QObject
{
    Q_OBJECT

    Ui::MainWindow*    mUi;
    QComboBox&         mEquipmentType;
    QTableWidget&      mTable;
    QPushButton&       mAddHire;
    Backend::Database& mDatabase;

    void printDefaultTable(void);
    void printRents();

public:
    explicit RentsTab(Ui::MainWindow* ui, Backend::Database& database);

signals:

public slots:
    void equipmentTypeChanged(int index);
    void addPressed();
    void displayMenu(QPoint pos);
    void tabChanged(int);
};

} // namespace Frontend
