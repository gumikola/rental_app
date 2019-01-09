#pragma once

#include <QDateTime>
#include <QString>

#define RENTAL_ID 1

class QComboBox;

namespace Common {

enum class EquipmentType
{
    None,
    Elektronarzedzia,
    SprzetBudowlany,
    SprzetOgrodowy,
    Transport
};

static const EquipmentType All[] = {EquipmentType::None, EquipmentType::Elektronarzedzia,
                                    EquipmentType::SprzetBudowlany, EquipmentType::SprzetOgrodowy,
                                    EquipmentType::Transport};

enum class Status
{
    Free,
    Hired
};

struct EquipmentParameters
{
    QString       name;
    double        price;
    double        pledge; // zastaw ;)
    uint          amount;
    QString       producer;
    EquipmentType type;
};

struct ClientDetails
{
    QString name;
    QString surname;
    QString adress;
};

struct RentDetails
{
    ClientDetails       client;
    EquipmentParameters equipment;
    QDateTime           rentDate;
    QDateTime           returnDate;
    uint                id;
};

QString equipmentTypeToString(const EquipmentType& type);
void    fillEquipmentTypeComboBox(QComboBox& equipmentType);

} // namespace Common
