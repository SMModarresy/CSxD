#ifndef CSXD_GAMEPLAY_H
#define CSXD_GAMEPLAY_H


#include <string>
#include <iostream>
#include <algorithm>

#include "models/weapon/WeaponType.h"
#include "models/player/Side.h"
#include "models/game/Game.h"

using namespace std;

class GamePlay {
public:
    explicit GamePlay(shared_ptr<Game> for_game);
    explicit GamePlay(uint rounds);
    virtual ~GamePlay() = default;

    virtual void set_round_time(ull time) const;
    virtual shared_ptr<Player> create_player(const string& name, Side side) const;
    virtual void add_player(const shared_ptr<Player>& player) const;
    virtual uint get_hp(const string& player_name) const;
    virtual uint get_money(const string& player_name) const;
    virtual void buy_weapon(const string& player_name, const shared_ptr<Weapon>& weapon) const;
    virtual void attack_occurred(const string& attacker_name, const string& attacked_name, WeaponType weapon_type) const;
    virtual Side determine_winner_and_go_next_round() const;
    virtual vector<shared_ptr<Player>> get_scoreboard(Side side) const;
    virtual bool has_ended() const;

protected:
    virtual void check_player_can_buy_weapon(const shared_ptr<Player>& player, const shared_ptr<Weapon>& weapon) const;
    virtual bool is_weapon_already_equipped(const shared_ptr<Player>& player, WeaponType weapon_type) const;
    virtual void check_attack_could_have_occurred(const shared_ptr<Player>& attacker, const shared_ptr<Player>& attacked,
                                                  WeaponType weapon_type) const;
    virtual void attacked_died_in_attack(const shared_ptr<Player>& attacker, const shared_ptr<Player>& attacked,
                                         const shared_ptr<Weapon>& weapon) const;
    virtual void drop_weapon_if_equipped(const shared_ptr<Player>& player, WeaponType weapon_type) const;
    virtual void go_next_round_or_end() const;
    virtual void find_winner_loser(Side& winner_side, Side& loser_side) const;
    virtual void reset_players_and_add_money(Side side, uint money) const;
    static bool scoreboard_comparer(const shared_ptr<Player>& p1, const shared_ptr<Player>& p2);

    const ull ROUND_LENGTH = (2 * 60 + 15) * 1000;
    const ull ENTER_TIME_LIMIT = 3 * 1000;
    const ull BUY_TIME_LIMIT = 45 * 1000;
    const uint PLAYER_MAX_MONEY = 10000;
    const uint PLAYER_INITIAL_MONEY = 1000;
    const uint WINNER_MONEY_PER_ROUND = 2700;
    const uint LOSER_MONEY_PER_ROUND = 2400;
    const size_t MAX_TEAM_SIZE = 10;

    shared_ptr<Game> game;

};


#endif //CSXD_GAMEPLAY_H
