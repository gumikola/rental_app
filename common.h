#pragma once

#include <QString>

namespace Common {

enum class EquipmentType
{
    None,
    Elektronarzedzia,
    SprzetBudowlany
};

static const EquipmentType All[] = {EquipmentType::None, EquipmentType::Elektronarzedzia,
                                    EquipmentType::SprzetBudowlany};

struct EquipmentParameters
{
    QString       name;
    double        price;
    double        pledge; // zastaw ;)
    uint          amount;
    EquipmentType type;
};

QString equipmentTypeToString(const EquipmentType& type);

} // namespace Common
