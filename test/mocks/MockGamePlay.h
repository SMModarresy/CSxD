#ifndef CSXD_MOCKGAMEPLAY_H
#define CSXD_MOCKGAMEPLAY_H


#include <utility>

#include "gmock/gmock.h"

#include "GamePlay.h"

class MockGamePlay : public GamePlay {
public:
    MockGamePlay() : GamePlay(1) { }
    MOCK_METHOD(void, set_round_time, (ull time), (const, override));
    MOCK_METHOD(shared_ptr<Player>, create_player, (const string& name, Side side), (const, override));
    MOCK_METHOD(void, add_player, (const shared_ptr<Player>& player), (const, override));
    MOCK_METHOD(uint, get_hp, (const string& player_name), (const, override));
    MOCK_METHOD(uint, get_money, (const string& player_name), (const, override));
    MOCK_METHOD(void, buy_weapon, (const string& player_name, const shared_ptr<Weapon>& weapon), (const, override));
    MOCK_METHOD(void, attack_occurred, (const string& attacker_name, const string& attacked_name, WeaponType weapon_type), (const, override));
    MOCK_METHOD(Side, determine_winner_and_go_next_round, (), (const, override));
    MOCK_METHOD(vector<shared_ptr<Player>>, get_scoreboard, (Side side), (const, override));
    MOCK_METHOD(bool, has_ended, (), (const, override));
};


#endif //CSXD_MOCKGAMEPLAY_H
