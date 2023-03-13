#include <utility>

#include "Weapon.h"

Weapon::Weapon(string name, uint price, uint damage_per_hit, uint money_per_kill, WeaponType type, Side available_for) : name(std::move(name)), price(price), damage_per_hit(damage_per_hit), money_per_kill(money_per_kill), type(type), available_for(available_for) {}

string Weapon::get_name() const {
    return name;
}

uint Weapon::get_price() const {
    return price;
}

uint Weapon::get_damage_per_hit() const {
    return damage_per_hit;
}

uint Weapon::get_money_per_kill() const {
    return money_per_kill;
}

WeaponType Weapon::get_type() const {
    return type;
}

bool Weapon::is_available_for(Side side) const {
    return (side & available_for) >= side;
}
