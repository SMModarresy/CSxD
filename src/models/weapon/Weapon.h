#ifndef CSXD_WEAPON_H
#define CSXD_WEAPON_H


#include <string>

#include "WeaponType.h"
#include "models/player/Side.h"

using namespace std;

class Weapon {
public:
    Weapon(string name, uint price, uint damage_per_hit, uint money_per_kill, WeaponType type, Side available_for);
    virtual ~Weapon() = default;
    virtual string get_name() const;
    virtual uint get_price() const;
    virtual uint get_damage_per_hit() const;
    virtual uint get_money_per_kill() const;
    virtual WeaponType get_type() const;
    virtual bool is_available_for(Side side) const;

protected:
    string name;
    uint price;
    uint damage_per_hit;
    uint money_per_kill;
    WeaponType type;
    Side available_for;
};


#endif //CSXD_WEAPON_H
