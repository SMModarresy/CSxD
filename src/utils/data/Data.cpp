#include <fstream>

#include "nlohmann/json.hpp"

#include "Data.h"
#include "exceptions/WeaponNotFoundException.h"

using namespace std;
using json = nlohmann::json;

unordered_map<string, shared_ptr<Weapon>> Data::weapons;

void Data::load_weapons() {
//    weapons["Desert-Eagle"] = make_shared<Weapon>("Desert-Eagle", 600, 53, 175, PISTOL, COUNTER_TERRORIST);
//    weapons["UPS-S"] = make_shared<Weapon>("UPS-S", 300, 13, 225, PISTOL, COUNTER_TERRORIST);
//    weapons["M4A1"] = make_shared<Weapon>("M4A1", 2700, 29, 100, HEAVY, COUNTER_TERRORIST);
//    weapons["Revolver"] = make_shared<Weapon>("Revolver", 600, 51, 150, PISTOL, TERRORIST);
//    weapons["Glock-18"] = make_shared<Weapon>("Glock-18", 300, 11, 200, PISTOL, TERRORIST);
//    weapons["AK"] = make_shared<Weapon>("AK", 2700, 31, 100, HEAVY, TERRORIST);
//    weapons["AWP"] = make_shared<Weapon>("AWP", 4300, 110, 50, HEAVY, ALL);
//    weapons["Knife"] = make_shared<Weapon>("Knife", 0, 43, 500, MELEE, ALL);
    ifstream weapons_json_file("weapons.json");
    json weapon_list = json::parse(weapons_json_file);
    for(Weapon weapon : weapon_list) {
        weapons[weapon.get_name()] = make_shared<Weapon>(weapon);
    }
}

void Data::load() {
    load_weapons();
}

shared_ptr<Weapon> Data::get_weapon_by_name(const string& name) {
    if(weapons.find(name) == weapons.end()) {
        throw WeaponNotFoundException();
    }
    return weapons[name];
}

shared_ptr<Weapon> Data::try_get_weapon_by_name(const string& name) {
    try {
        return get_weapon_by_name(name);
    }
    catch (...) {
        return nullptr;
    }
}
