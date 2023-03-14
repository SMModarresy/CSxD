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

void to_json(json& json, const Weapon& weapon) {
    json["name"] = weapon.name;
    json["price"] = weapon.price;
    json["damage_per_hit"] = weapon.damage_per_hit;
    json["money_per_kill"] = weapon.money_per_kill;
    json["type"] = weapon.type;
    json["available_for"] = weapon.available_for;
}

void from_json(const json& json, Weapon& weapon) {
    weapon.name = json.at("name");
    weapon.price = json.at("price");
    weapon.damage_per_hit = json.at("damage_per_hit");
    weapon.money_per_kill = json.at("money_per_kill");
    weapon.type = json.at("type");
    weapon.available_for = json.at("available_for");
}
