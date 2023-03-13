#ifndef CSXD_MOCKPLAYER_H
#define CSXD_MOCKPLAYER_H


#include <utility>

#include "gmock/gmock.h"

#include "models/player/Player.h"

class MockPlayer : public Player {
public:
    MockPlayer() : Player("", 0, 0, 0, COUNTER_TERRORIST, 0) { }
    MOCK_METHOD(uint, get_hp, (), (const, override));
    MOCK_METHOD(void, add_hp, (uint added_hp), (override));
    MOCK_METHOD(void, take_damage, (uint damage), (override));
    MOCK_METHOD(void, reset_hp, (), (override));
    MOCK_METHOD(bool, is_alive, (), (const, override));
    MOCK_METHOD(uint, get_kills, (), (const, override));
    MOCK_METHOD(void, add_kill, (), (override));
    MOCK_METHOD(uint, get_deaths, (), (const, override));
    MOCK_METHOD(uint, get_max_money, (), (const, override));
    MOCK_METHOD(uint, get_money, (), (const, override));
    MOCK_METHOD(void, add_money, (uint amount), (override));
    MOCK_METHOD(void, subtract_money, (uint amount), (override));
    MOCK_METHOD(ull, get_entry_time, (), (const, override));
    MOCK_METHOD(Side, get_side, (), (const, override));
    MOCK_METHOD(string, get_name, (), (const, override));
    MOCK_METHOD(shared_ptr<Weapon>, get_weapon, (WeaponType type), (override));
    MOCK_METHOD(void, equip_weapon, (shared_ptr<Weapon> weapon), (override));
    MOCK_METHOD(void, drop_weapon, (WeaponType type), (override));
};


#endif //CSXD_MOCKPLAYER_H
