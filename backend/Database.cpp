#include "Database.h"
#include "common.h"
#include <QDebug>
#include <QVector>
#include <QtSql/QtSql>

namespace Backend {

Database::Database()
{

#ifdef __linux__
    mDatabase.setHostName("192.168.0.137");
    mDatabase.setDatabaseName("wypo");
    mDatabase.setPort(5432);
    mDatabase.setUserName("postgres");
    mDatabase.setPassword("zaq12wsx");
#elif _WIN32
    mDatabase.setDatabaseName("PostgreSQL35W");
#endif
    if (mDatabase.open())
        qDebug("Database opened!");
}

void Database::addClient(const Common::ClientDetails& client)
{
    qDebug("Database %s", __func__);
    QSqlQuery q;
    uint      last_id = 0;

    q.prepare("SELECT max(id) FROM public.klient");
    if (q.exec())
    {
        while (q.next())
        {
            last_id = q.value(0).toUInt();
        }
    }
    else
    {
        qDebug() << q.lastError();
    }

    q.prepare("INSERT INTO public.klient("
              "id,imie, nazwisko, adres)"
              "VALUES (?,?, ?, ?);");

    q.bindValue(0, last_id + 1);
    q.bindValue(1, client.name);
    q.bindValue(2, client.surname);
    q.bindValue(3, client.adress);

    q.exec();
    if (q.lastError().isValid())
        qDebug() << q.lastError();
}

QVector<Common::ClientDetails> Database::getClients()
{

    QVector<Common::ClientDetails> clients;
    QSqlQuery                      q;

    q.prepare("SELECT * FROM public.klient");

    if (q.exec())
    {
        while (q.next())
        {
            Common::ClientDetails tmp;
            tmp.name    = q.value("imie").toString();
            tmp.surname = q.value("nazwisko").toString();
            tmp.adress  = q.value("adres").toString();

            clients.push_back(tmp);
        }
    }
    else
    {
        qDebug() << q.lastError();
    }

    return clients;
}

QVector<Common::EquipmentParameters> Database::getEquipment(const Common::EquipmentType& type, uint rental)
{
    qDebug("Database::getEquipment");
    QVector<Common::EquipmentParameters> eq;
    QSqlQuery                            q;

    if (rental <= 0)
    {
        qDebug("ERROR Database::getEquipment wrong rental id");
        return eq;
    }

    q.prepare("SELECT * FROM public.view_sprzet WHERE typy_id=? AND odzialy_id=?;");

    q.bindValue(0, static_cast<uint>(type));
    q.bindValue(1, rental);

    if (q.exec())
    {
        while (q.next())
        {
            Common::EquipmentParameters tmp;
            tmp.producer = q.value("producent").toString();
            tmp.name     = q.value("nazwa").toString();
            tmp.amount   = q.value("ilosc").toUInt();
            tmp.price    = q.value("cena").toDouble();
            tmp.pledge   = q.value("zastaw").toDouble();
            tmp.type     = static_cast<Common::EquipmentType>(q.value("typy_id").toUInt());

            eq.push_back(tmp);
        }
    }
    else
    {
        qDebug() << q.lastError();
    }

    return eq;
}

QVector<Common::RentDetails> Database::getRents(const Common::EquipmentType type)
{

    QVector<Common::RentDetails> rents;
    QSqlQuery                    q;

    q.prepare("SELECT * FROM public.zamowienia INNER JOIN public.sprzet ON "
              "public.zamowienia.sprzet_id=public.sprzet.id INNER JOIN public.klient ON "
              "public.zamowienia.klient_id=public.klient.id WHERE status=1 AND typy_id=?;");

    q.bindValue(0, static_cast<uint>(type));

    if (q.exec())
    {
        while (q.next())
        {
            Common::RentDetails tmp;
            tmp.equipment.producer = q.value("producent").toString();
            tmp.equipment.name     = q.value("nazwa").toString();
            tmp.equipment.amount   = q.value("ilosc").toUInt();
            tmp.equipment.price    = q.value("cena").toDouble();
            tmp.equipment.pledge   = q.value("zastaw").toDouble();
            tmp.equipment.type     = static_cast<Common::EquipmentType>(q.value("typy_id").toUInt());
            tmp.client.name        = q.value("imie").toString();
            tmp.client.surname     = q.value("nazwisko").toString();
            tmp.client.adress      = q.value("adres").toString();
            tmp.id                 = q.value("id").toUInt();
            tmp.rentDate           = q.value("data_wypozyczenia").toDateTime();
            tmp.returnDate         = q.value("data_oddania").toDateTime();

            rents.push_back(tmp);
        }
    }
    else
    {
        qDebug() << q.lastError();
    }

    return rents;
}

void Database::removeEquipment(Common::EquipmentParameters& eq)
{
    qDebug("Database remove equipment");

    QSqlQuery q;

    q.prepare("DELETE FROM public.sprzet "
              "WHERE producent=? AND nazwa=? AND ilosc=? AND cena=? AND zastaw=? AND typy_id=? "
              "AND odzialy_id=?;");

    q.bindValue(0, eq.producer);
    q.bindValue(1, eq.name);
    q.bindValue(2, eq.amount);
    q.bindValue(3, eq.price);
    q.bindValue(4, eq.pledge);
    q.bindValue(5, static_cast<uint>(eq.type));
    q.bindValue(6, RENTAL_ID);

    q.exec();
    if (q.lastError().isValid())
        qDebug() << q.lastError();
}

void Database::addHire(Common::RentDetails& hire)
{
    qDebug("Database::addHire");

    QSqlQuery q;
    uint      clientId = 0;
    uint      eqId     = 0;
    uint      amount   = 0;

    q.prepare("SELECT id FROM public.klient WHERE imie=? AND nazwisko=? AND adres=?");

    q.bindValue(0, hire.client.name);
    q.bindValue(1, hire.client.surname);
    q.bindValue(2, hire.client.adress);

    if (q.exec())
    {
        if (q.first())
        {
            clientId = q.value("id").toUInt();
        }
        else
        {
            qDebug("Database::addHire client not found");
            return;
        }
    }
    else
    {
        qDebug() << q.lastError();
        return;
    }

    q.prepare(
        "SELECT id, ilosc FROM public.sprzet WHERE producent=? AND nazwa=? AND ilosc=? AND cena=? "
        "AND zastaw=? AND typy_id=? "
        "AND odzialy_id=?;");

    q.bindValue(0, hire.equipment.producer);
    q.bindValue(1, hire.equipment.name);
    q.bindValue(2, hire.equipment.amount);
    q.bindValue(3, hire.equipment.price);
    q.bindValue(4, hire.equipment.pledge);
    q.bindValue(5, static_cast<uint>(hire.equipment.type));
    q.bindValue(6, RENTAL_ID);

    qDebug() << hire.equipment.producer << "   " << hire.equipment.name << "   "
             << hire.equipment.amount << "   " << hire.equipment.price << "   "
             << hire.equipment.pledge << "   " << static_cast<uint>(hire.equipment.type) << "   "
             << RENTAL_ID;

    if (q.exec())
    {
        if (q.first())
        {
            eqId   = q.value("id").toUInt();
            amount = q.value("ilosc").toUInt();
        }
        else
        {
            qDebug("Database::addHire equipment not found");
            return;
        }
    }
    else
    {
        qDebug() << q.lastError();
        return;
    }

    q.prepare("INSERT INTO public.zamowienia (data_wypozyczenia, data_oddania, status, "
              "klient_id, sprzet_id ) VALUES (?, ?, ?, ?, ?);");

    q.bindValue(0, hire.rentDate);
    q.bindValue(1, hire.returnDate);
    q.bindValue(2, static_cast<uint>(Common::Status::Hired));
    q.bindValue(3, clientId);
    q.bindValue(4, eqId);

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        return;
    }

    //#TODO
    q.prepare("UPDATE public.sprzet SET ilosc=?"
              "WHERE producent=? AND nazwa=? AND ilosc=? AND cena=? AND zastaw=? AND "
              "typy_id=? AND odzialy_id=?;");

    q.bindValue(0, amount - 1);
    q.bindValue(1, hire.equipment.producer);
    q.bindValue(2, hire.equipment.name);
    q.bindValue(3, hire.equipment.amount);
    q.bindValue(4, hire.equipment.price);
    q.bindValue(5, hire.equipment.pledge);
    q.bindValue(6, static_cast<uint>(hire.equipment.type));
    q.bindValue(7, RENTAL_ID);

    q.exec();
    if (q.lastError().isValid())
        qDebug() << q.lastError();
}

void Database::editEquipment(const Common::EquipmentParameters& prev, const Common::EquipmentParameters& actual)
{
    qDebug("Database %s", __func__);

    QSqlQuery q;

    q.prepare("UPDATE public.sprzet SET producent=?, nazwa=?, ilosc=?, cena=?, zastaw=?, "
              "typy_id=?, odzialy_id=? "
              "WHERE producent=? AND nazwa=? AND ilosc=? AND cena=? AND zastaw=? AND "
              "typy_id=? AND odzialy_id=?;");

    q.bindValue(0, actual.producer);
    q.bindValue(1, actual.name);
    q.bindValue(2, actual.amount);
    q.bindValue(3, actual.price);
    q.bindValue(4, actual.pledge);
    q.bindValue(5, static_cast<uint>(actual.type));
    q.bindValue(6, RENTAL_ID);

    q.bindValue(7, prev.producer);
    q.bindValue(8, prev.name);
    q.bindValue(9, prev.amount);
    q.bindValue(10, prev.price);
    q.bindValue(11, prev.pledge);
    q.bindValue(12, static_cast<uint>(prev.type));
    q.bindValue(13, RENTAL_ID);

    q.exec();
    if (q.lastError().isValid())
        qDebug() << q.lastError();
}

void Database::removeClient(Common::ClientDetails& client)
{
    qDebug() << "Database remove client: imie=" << client.name << " nazwisko=" << client.surname
             << " adres=" << client.adress;
    QSqlQuery q;

    q.prepare("DELETE FROM public.klient "
              "WHERE imie=? AND nazwisko=? AND adres=?;");

    q.bindValue(0, client.name);
    q.bindValue(1, client.surname);
    q.bindValue(2, client.adress);

    q.exec();
    if (q.lastError().isValid())
        qDebug() << q.lastError();
}

void Database::editClient(const Common::ClientDetails& prev, const Common::ClientDetails& actual)
{
    qDebug("Database edit client");
    QSqlQuery q;

    q.prepare("UPDATE public.klient "
              "SET imie=?, nazwisko=?, adres=? "
              "WHERE imie=? AND nazwisko=? AND adres=?;");

    q.bindValue(0, actual.name);
    q.bindValue(1, actual.surname);
    q.bindValue(2, actual.adress);

    q.bindValue(3, prev.name);
    q.bindValue(4, prev.surname);
    q.bindValue(5, prev.adress);

    q.exec();
    if (q.lastError().isValid())
        qDebug() << q.lastError();
}

void Database::addEquipment(const Common::EquipmentParameters& eq, const Common::EquipmentType& type)
{
    qDebug("Database add equipment");

    QSqlQuery q;

    q.prepare("INSERT INTO public.sprzet (producent, nazwa, ilosc, cena, zastaw, typy_id, "
              "odzialy_id) VALUES (?, ?, ?, ?, ?, ?, ?);");

    q.bindValue(0, eq.producer);
    q.bindValue(1, eq.name);
    q.bindValue(2, eq.amount);
    q.bindValue(3, eq.price);
    q.bindValue(4, eq.pledge);
    q.bindValue(5, static_cast<uint>(type));
    q.bindValue(6, RENTAL_ID);

    q.exec();
    if (q.lastError().isValid())
        qDebug() << q.lastError();
}

void Database::removeHire(const Common::RentDetails& hire)
{
    qDebug("Database remove hire");

    QSqlQuery q;

    q.prepare("DELETE FROM public.zamowienia WHERE id=?;");

    q.bindValue(0, hire.id);

    q.exec();
    if (q.lastError().isValid())
        qDebug() << q.lastError();
}

void Database::markAsCompleted(const Common::RentDetails& hire)
{
    qDebug("Database mark as completed");
    QSqlQuery q;

    q.prepare("UPDATE public.zamowienia SET status=0 WHERE id=?;");

    q.bindValue(0, hire.id);

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        return;
    }

    q.prepare("SELECT sprzet_id FROM public.zamowienia WHERE id=?;");

    q.bindValue(0, hire.id);

    uint sprzet_id = 0;
    if (q.exec())
    {
        if (q.first())
        {
            sprzet_id = q.value("sprzet_id").toUInt();
        }
        else
        {
            qDebug("Database::addHire sprzet_id not found");
            return;
        }
    }
    else
    {
        qDebug() << q.lastError();
        return;
    }

    q.prepare("SELECT ilosc FROM public.sprzet WHERE id=?;");

    q.bindValue(0, sprzet_id);

    uint ilosc = 0;

    if (q.exec())
    {
        if (q.first())
        {
            ilosc = q.value("ilosc").toUInt();
        }
        else
        {
            qDebug("Database::addHire ilosc not found");
            return;
        }
    }
    else
    {
        qDebug() << q.lastError();
        return;
    }

    q.prepare("UPDATE public.sprzet SET ilosc=? WHERE id=?;");

    q.bindValue(0, ilosc + 1);
    q.bindValue(1, sprzet_id);

    q.exec();
    if (q.lastError().isValid())
    {
        qDebug() << q.lastError();
        return;
    }
}
} // namespace Backend
