#ifndef CSXD_GAME_H
#define CSXD_GAME_H


#include <unordered_map>
#include <memory>
#include <vector>

#include "models/player/Player.h"

using namespace std;

typedef unsigned long long ull;

class Game {
public:
    Game(int id, uint rounds, ull round_length, size_t max_team_size);
    virtual ~Game() = default;

    virtual ull get_id() const;
    virtual uint get_rounds() const;
    virtual uint get_current_round() const;
    virtual void go_next_round();
    virtual ull get_round_length() const;
    virtual ull get_round_time() const;
    virtual void set_round_time(ull time);
    virtual ull get_game_time() const;
    virtual size_t get_max_team_size() const;
    virtual void set_max_team_size(size_t size);
    virtual void end();
    virtual bool has_ended() const;
    virtual vector<shared_ptr<Player>> get_all_players(Side side) const;
    virtual shared_ptr<Player> get_player_by_name(const string& name);
    virtual vector<shared_ptr<Player>> get_alive_players(Side side) const;
    virtual uint get_alive_player_count(Side side) const;
    virtual void add_player(const shared_ptr<Player>& player);

protected:
    void check_player_can_be_added(const shared_ptr<Player>& player);
    void handle_player_already_in_game(const shared_ptr<Player>& player);
    bool is_team_full(Side side) const;

    ull id;
    uint rounds;
    uint current_round;
    ull round_length;
    ull round_time;
    size_t max_team_size;
    bool ended;
    uint counter_terrorist_player_count;
    uint terrorist_player_count;
    unordered_map<string, shared_ptr<Player>> players;
};


#endif //CSXD_GAME_H
