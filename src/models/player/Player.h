#ifndef CSXD_PLAYER_H
#define CSXD_PLAYER_H


#include <string>
#include <unordered_map>
#include <memory>
#include <utility>

#include "Side.h"
#include "models/weapon/WeaponType.h"
#include "models/weapon/Weapon.h"

using namespace std;

typedef unsigned long long ull;

class Player {
public:
    Player(string name, uint initial_hp, uint max_money, uint initial_money, Side side, ull entry_time);
    virtual ~Player() = default;

    virtual uint get_hp() const;
    virtual void add_hp(uint added_hp);
    virtual void take_damage(uint damage);
    virtual void reset_hp();
    virtual bool is_alive() const;
    virtual uint get_kills() const;
    virtual void add_kill();
    virtual uint get_deaths() const;
    virtual uint get_max_money() const;
    virtual uint get_money() const;
    virtual void add_money(uint amount);
    virtual void subtract_money(uint amount);
    virtual ull get_entry_time() const;
    virtual Side get_side() const;
    virtual string get_name() const;
    virtual shared_ptr<Weapon> get_weapon(WeaponType type);
    virtual void equip_weapon(shared_ptr<Weapon> weapon);
    virtual void drop_weapon(WeaponType type);

protected:
    string name;
    uint hp;
    uint kills;
    uint deaths;
    uint max_money;
    uint money;
    ull entry_time;
    Side side;
    unordered_map<WeaponType, shared_ptr<Weapon>> weapons;
};


#endif //CSXD_PLAYER_H
