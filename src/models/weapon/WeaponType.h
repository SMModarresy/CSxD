#ifndef CSXD_WEAPONTYPE_H
#define CSXD_WEAPONTYPE_H

#include "nlohmann/json.hpp"

enum WeaponType {
    MELEE = 1,
    PISTOL = 2,
    HEAVY = 4
};

NLOHMANN_JSON_SERIALIZE_ENUM(WeaponType, {
    {MELEE, "melee"},
    {PISTOL, "pistol"},
    {HEAVY, "heavy"},
})

#endif //CSXD_WEAPONTYPE_H
