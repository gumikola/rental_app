#pragma once

#include <QString>

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

struct EquipmentParameters
{
    QString       name;
    double        price;
    double        pledge; // zastaw ;)
    uint          amount;
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
    QString equipmentName;
    uint    amount;
    QString name;
    QString surname;
};

QString equipmentTypeToString(const EquipmentType& type);
void    fillEquipmentTypeComboBox(QComboBox& equipmentType);

} // namespace Common
