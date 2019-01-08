#include "common.h"
#include <QComboBox>

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

    case EquipmentType::SprzetOgrodowy:
        return QString("Sprzęt ogrodowy");

    case EquipmentType::Transport:
        return QString("Sprzęt transportowy");
    }
    return QString();
}

void fillEquipmentTypeComboBox(QComboBox& equipmentType)
{
    equipmentType.clear();

    for (const auto e : Common::All)
        equipmentType.addItem(Common::equipmentTypeToString(e));
}

} // namespace Common
