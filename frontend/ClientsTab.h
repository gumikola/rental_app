#pragma once

#include "common.h"
#include "ui_mainwindow.h"
#include <QObject>

namespace Backend {
class Database;
} // namespace Backend

namespace Frontend {

class ClientsTab : public QObject
{
    Q_OBJECT

    Ui::MainWindow*    mUi;
    QTableWidget&      mTable;
    Backend::Database& mDatabase;

    void printDefaultTable(void);
    void printClients(QVector<Common::ClientDetails> clients);
    void printFiltredClients(const QString& filter, QVector<Common::ClientDetails> clients);

public:
    ClientsTab(Ui::MainWindow* ui, Backend::Database& database);

public slots:
    void addPressed();
    void displayMenu(QPoint pos);
    void tabChanged(int);
    void filterChanged(const QString& text);
};

} // namespace Frontend
