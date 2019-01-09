#pragma once
#include "common.h"
#include <QString>
#include <QtSql/QtSql>

namespace Backend {

class Database
{
#ifdef __linux__
    QSqlDatabase mDatabase = QSqlDatabase::addDatabase("QPSQL");
#elif _WIN32
    QSqlDatabase mDatabase = QSqlDatabase::addDatabase("QODBC");
#endif

public:
    Database();

    void                           addClient(const Common::ClientDetails& client);
    void                           editClient(const Common::ClientDetails& prev, const Common::ClientDetails& actual);
    QVector<Common::ClientDetails> getClients();
    void                           removeClient(Common::ClientDetails& client);

    void addEquipment(const Common::EquipmentParameters& eq, const Common::EquipmentType& type);
    void editEquipment(const Common::EquipmentParameters& prev, const Common::EquipmentParameters& actual);
    QVector<Common::EquipmentParameters> getEquipment(const Common::EquipmentType& type, uint rental);
    void                                 removeEquipment(Common::EquipmentParameters& eq);

    void                         addHire(Common::RentDetails& hire);
    QVector<Common::RentDetails> getRents(const Common::EquipmentType type);
    void                         removeHire(const Common::RentDetails& hire);
    void                         markAsCompleted(const Common::RentDetails& hire);
};

} // namespace Backend
