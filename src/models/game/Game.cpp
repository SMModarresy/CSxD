#include "Game.h"
#include "exceptions/LastRoundException.h"
#include "exceptions/NullPointerException.h"
#include "exceptions/PlayerAlreadyInTeamException.h"
#include "exceptions/PlayerInOpponentTeamException.h"
#include "exceptions/PlayerNotFoundException.h"
#include "exceptions/TeamIsFullException.h"

Game::Game(int id, uint rounds, ull round_length, size_t max_team_size) : id(id), rounds(rounds), current_round(1), round_length(round_length), round_time(0), max_team_size(max_team_size), ended(false), counter_terrorist_player_count(0), terrorist_player_count(0), players() {
    if (rounds == 0) {
        throw out_of_range("rounds should be more than 0");
    }
    if (round_length == 0) {
        throw out_of_range("round_length should be more than 0");
    }
    if (max_team_size == 0) {
        throw out_of_range("max_team_size should be more than 0");
    }
}

ull Game::get_id() const {
    return id;
}

uint Game::get_rounds() const {
    return rounds;
}

uint Game::get_current_round() const {
    return current_round;
}

void Game::go_next_round() {
    if (current_round >= rounds) {
        throw LastRoundException();
    }
    current_round++;
    round_time = 0;
}

ull Game::get_round_length() const {
    return round_length;
}

ull Game::get_round_time() const {
    return round_time;
}

void Game::set_round_time(ull time) {
    round_time = time;
}

size_t Game::get_max_team_size() const {
    return max_team_size;
}

void Game::set_max_team_size(size_t size) {
    max_team_size = size;
}

ull Game::get_game_time() const {
    return (current_round - 1) * round_length + round_time;
}

void Game::end() {
    ended = true;
}

bool Game::has_ended() const {
    return ended;
}

vector<shared_ptr<Player>> Game::get_all_players(Side side) const {
    vector<shared_ptr<Player>> player_list;
    for (const auto& player: players) {
        if(side & player.second->get_side()) {
            player_list.push_back(player.second);
        }
    }
    return player_list;
}

shared_ptr<Player> Game::get_player_by_name(const string& name) {
    if (players.find(name) != players.end()) {
        return players[name];
    }
    throw PlayerNotFoundException();
}

vector<shared_ptr<Player>> Game::get_alive_players(Side side) const {
    vector<shared_ptr<Player>> player_list;
    for(const auto& player : players) {
        if(side & player.second->get_side() && player.second->is_alive()) {
            player_list.push_back(player.second);
        }
    }
    return player_list;
}

uint Game::get_alive_player_count(Side side) const {
    uint player_count = 0;
    for(const auto& player : players) {
        if(side & player.second->get_side() && player.second->is_alive()) {
            player_count++;
        }
    }
    return player_count;
}

void Game::add_player(const shared_ptr<Player>& player) {
    if (player == nullptr) {
        throw NullPointerException("player");
    }

    check_player_can_be_added(player);

    if (player->get_side() == TERRORIST) {
        terrorist_player_count++;
    }
    if (player->get_side() == COUNTER_TERRORIST) {
        counter_terrorist_player_count++;
    }
    players[player->get_name()] = player;
}

void Game::check_player_can_be_added(const shared_ptr<Player>& player) {
    if (players.find(player->get_name()) != players.end()) {
        handle_player_already_in_game(player);
    }
    if (is_team_full(player->get_side())) {
        throw TeamIsFullException();
    }
}

void Game::handle_player_already_in_game(const shared_ptr<Player>& player) {
    auto old_player = players[player->get_name()];
    if (old_player->get_side() == player->get_side()) {
        throw PlayerAlreadyInTeamException();
    }
    else {
        throw PlayerInOpponentTeamException();
    }
}

bool Game::is_team_full(Side side) const {
    if (side == TERRORIST) {
        return terrorist_player_count >= max_team_size;
    }
    if (side == COUNTER_TERRORIST) {
        return counter_terrorist_player_count >= max_team_size;
    }
    throw invalid_argument("side is invalid. should be one of: [COUNTER_TERRORIST, TERRORIST]");
}