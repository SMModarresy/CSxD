#include <utility>

#include "Player.h"
#include "exceptions/NotEnoughMoneyException.h"
#include "exceptions/NullPointerException.h"
#include "exceptions/WeaponNotEquippedException.h"

Player::Player(string name, uint initial_hp, uint max_money, uint initial_money, Side side, ull entry_time) : name(std::move(name)), hp(initial_hp), kills(0), deaths(0), max_money(max_money), money(initial_money), side(side), entry_time(entry_time), weapons() {
    if (initial_hp > 100) {
        throw out_of_range("initial_hp should be between 0 and 100 (inclusive)");
    }
}

uint Player::get_hp() const {
    return hp;
}

void Player::add_hp(uint added_hp) {
    hp = min(100u, hp + added_hp);
}

void Player::take_damage(uint damage) {
    if (hp == 0) {
        return;
    }
    hp -= min(damage, hp);
    if (hp == 0) {
        deaths++;
    }
}

void Player::reset_hp() {
    hp = 100;
}

bool Player::is_alive() const {
    return hp > 0;
}

uint Player::get_kills() const {
    return kills;
}

void Player::add_kill() {
    kills++;
}

uint Player::get_deaths() const {
    return deaths;
}

uint Player::get_max_money() const {
    return max_money;
}

uint Player::get_money() const {
    return money;
}

void Player::add_money(uint amount) {
    money = min(max_money, money + amount);
}

void Player::subtract_money(uint amount) {
    if(amount > money) {
        throw NotEnoughMoneyException();
    }
    money -= amount;
}

ull Player::get_entry_time() const {
    return entry_time;
}

Side Player::get_side() const {
    return side;
}

string Player::get_name() const {
    return name;
}

shared_ptr<Weapon> Player::get_weapon(WeaponType type) {
    if(weapons.find(type) == weapons.end()) {
        throw WeaponNotEquippedException();
    }
    return weapons[type];
}

void Player::equip_weapon(shared_ptr<Weapon> weapon) {
    if (weapon == nullptr) {
        throw NullPointerException("weapon");
    }
    weapons[weapon->get_type()] = std::move(weapon);
}

void Player::drop_weapon(WeaponType type) {
    if(weapons.find(type) == weapons.end()) {
        throw WeaponNotEquippedException();
    }
    weapons.erase(type);
}
