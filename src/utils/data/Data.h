#ifndef CSXD_DATA_H
#define CSXD_DATA_H


#include <memory>
#include <unordered_map>

#include "models/weapon/WeaponType.h"
#include "models/weapon/Weapon.h"

using namespace std;

class Data {
public:
    static void load();
    static shared_ptr<Weapon> get_weapon_by_name(const string& name);
    static shared_ptr<Weapon> try_get_weapon_by_name(const string& name);

private:
    static void load_weapons();

    static unordered_map<string, shared_ptr<Weapon>> weapons;
};


#endif //CSXD_DATA_H
