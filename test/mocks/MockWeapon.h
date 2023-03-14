#ifndef CSXD_MOCKWEAPON_H
#define CSXD_MOCKWEAPON_H


#include <utility>

#include "gmock/gmock.h"

#include "models/weapon/Weapon.h"

class MockWeapon : public Weapon {
public:
    MockWeapon() : Weapon("", 0, 0, 0, MELEE, ALL) { }

    MOCK_METHOD(string, get_name, (), (const, override));
    MOCK_METHOD(uint, get_price, (), (const, override));
    MOCK_METHOD(uint, get_damage_per_hit, (), (const, override));
    MOCK_METHOD(uint, get_money_per_kill, (), (const, override));
    MOCK_METHOD(WeaponType, get_type, (), (const, override));
    MOCK_METHOD(bool, is_available_for, (Side side), (const, override));
};


#endif //CSXD_MOCKWEAPON_H
