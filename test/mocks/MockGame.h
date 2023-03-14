#ifndef CSXD_MOCKGAME_H
#define CSXD_MOCKGAME_H


#include <utility>

#include "gmock/gmock.h"

#include "models/game/Game.h"

class MockGame : public Game {
public:
    MockGame() : Game(0, 1, 1, 1) { }

    MOCK_METHOD(ull, get_id, (), (const, override));
    MOCK_METHOD(uint, get_rounds, (), (const, override));
    MOCK_METHOD(uint, get_current_round, (), (const, override));
    MOCK_METHOD(void, go_next_round, (), (override));
    MOCK_METHOD(ull, get_round_length, (), (const, override));
    MOCK_METHOD(ull, get_round_time, (), (const, override));
    MOCK_METHOD(void, set_round_time, (ull time), (override));
    MOCK_METHOD(ull, get_game_time, (), (const, override));
    MOCK_METHOD(size_t, get_max_team_size, (), (const, override));
    MOCK_METHOD(void, set_max_team_size, (size_t size), (override));
    MOCK_METHOD(void, end, (), (override));
    MOCK_METHOD(bool, has_ended, (), (const, override));
    MOCK_METHOD(vector<shared_ptr<Player>>, get_all_players, (Side side), (const, override));
    MOCK_METHOD(shared_ptr<Player>, get_player_by_name, (const string& name), (override));
    MOCK_METHOD(vector<shared_ptr<Player>>, get_alive_players, (Side side), (const, override));
    MOCK_METHOD(uint, get_alive_player_count, (Side side), (const, override));
    MOCK_METHOD(void, add_player, (const shared_ptr<Player>& player), (override));
};


#endif //CSXD_MOCKGAME_H
