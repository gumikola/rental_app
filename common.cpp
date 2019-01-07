#include "common.h"

namespace Common {

QString equipmentTypeToString(const EquipmentType& type)
{
    switch (type)
    {
    case EquipmentType::None:
        return QString("Wybierz typ elektronarzędzia");

    case EquipmentType::Elektronarzedzia:
        return QString("Elektronarzędzia");

    case EquipmentType::SprzetBudowlany:
        return QString("Sprzęt budowlany");
    }
    return QString();
}
} // namespace Common
