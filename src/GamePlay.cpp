#include <utility>

#include "GamePlay.h"
#include "utils/data/Data.h"
#include "exceptions/ActionAtIllegalTimeException.h"
#include "exceptions/ActionFromDeadPlayerException.h"
#include "exceptions/AttackDeadPlayerException.h"
#include "exceptions/FriendlyFireException.h"
#include "exceptions/LastRoundException.h"
#include "exceptions/NullPointerException.h"
#include "exceptions/WeaponNotAvailableException.h"
#include "exceptions/WeaponNotEquippedException.h"
#include "exceptions/WeaponOfThisTypeAlreadyEquippedException.h"

GamePlay::GamePlay(shared_ptr<Game> for_game) {
    if (for_game == nullptr) {
        throw NullPointerException("for_game");
    }

    game = std::move(for_game);
}

GamePlay::GamePlay(uint rounds) {
    game = make_shared<Game>(1, rounds, ROUND_LENGTH, MAX_TEAM_SIZE);
}

void GamePlay::set_round_time(ull time) const {
    game->set_round_time(time);
}

shared_ptr<Player> GamePlay::create_player(const string& name, Side side) const {
    auto player = make_shared<Player>(name, game->get_round_time() >= ENTER_TIME_LIMIT ? 0 : 100, PLAYER_MAX_MONEY,
                                      PLAYER_INITIAL_MONEY, side, game->get_game_time());

    player->equip_weapon(Data::get_weapon_by_name("Knife"));

    return player;
}

void GamePlay::add_player(const shared_ptr<Player>& player) const {
    game->add_player(player);
}

uint GamePlay::get_hp(const string& player_name) const {
    return game->get_player_by_name(player_name)->get_hp();
}

uint GamePlay::get_money(const string& player_name) const {
    return game->get_player_by_name(player_name)->get_money();
}

void GamePlay::buy_weapon(const string& player_name, const shared_ptr<Weapon>& weapon) const {
    auto player = game->get_player_by_name(player_name);

    check_player_can_buy_weapon(player, weapon);

    player->subtract_money(weapon->get_price());
    player->equip_weapon(weapon);
}

void GamePlay::check_player_can_buy_weapon(const shared_ptr<Player>& player, const shared_ptr<Weapon>& weapon) const {
    if(!player->is_alive()) {
        throw ActionFromDeadPlayerException();
    }
    if(game->get_round_time() >= BUY_TIME_LIMIT) {
        throw ActionAtIllegalTimeException();
    }
    if (weapon == nullptr) {
        throw NullPointerException("weapon");
    }
    if (!weapon->is_available_for(player->get_side())) {
        throw WeaponNotAvailableException();
    }
    if (is_weapon_already_equipped(player, weapon->get_type())) {
        throw WeaponOfThisTypeAlreadyEquippedException();
    }
}

bool GamePlay::is_weapon_already_equipped(const shared_ptr<Player>& player, WeaponType weapon_type) const {
    try {
        player->get_weapon(weapon_type);
    }
    catch (...) {
        return false;
    }
    return true;
}

void GamePlay::attack_occurred(const string& attacker_name, const string& attacked_name, WeaponType weapon_type) const {
    auto attacker = game->get_player_by_name(attacker_name);
    auto attacked = game->get_player_by_name(attacked_name);

    check_attack_could_have_occurred(attacker, attacked, weapon_type);

    auto weapon = attacker->get_weapon(weapon_type);

    attacked->take_damage(weapon->get_damage_per_hit());
    if (!attacked->is_alive()) {
        attacked_died_in_attack(attacker, attacked, weapon);
    }
}

void GamePlay::check_attack_could_have_occurred(const shared_ptr<Player>& attacker, const shared_ptr<Player>& attacked,
                                                WeaponType weapon_type) const {
    if (!attacker->is_alive()) {
        throw ActionFromDeadPlayerException();
    }
    if (!attacked->is_alive()) {
        throw AttackDeadPlayerException();
    }
    /// Throws if weapon of this type is not equipped
    attacker->get_weapon(weapon_type);
    if (attacker->get_side() == attacked->get_side()) {
        throw FriendlyFireException();
    }
}

void GamePlay::attacked_died_in_attack(const shared_ptr<Player>& attacker, const shared_ptr<Player>& attacked,
                                       const shared_ptr<Weapon>& weapon) const {
    drop_weapon_if_equipped(attacked, PISTOL);
    drop_weapon_if_equipped(attacked, HEAVY);

    attacker->add_kill();
    attacker->add_money(weapon->get_money_per_kill());
}

void GamePlay::drop_weapon_if_equipped(const shared_ptr<Player>& player, WeaponType weapon_type) const {
    try {
        player->drop_weapon(weapon_type);
    }
    catch (const WeaponNotEquippedException& ex) { }
    catch (...) {
        throw;
    }
}

Side GamePlay::determine_winner_and_go_next_round() const {
    go_next_round_or_end();

    Side winner_side, loser_side;
    find_winner_loser(winner_side, loser_side);

    reset_players_and_add_money(winner_side, WINNER_MONEY_PER_ROUND);
    reset_players_and_add_money(loser_side, LOSER_MONEY_PER_ROUND);

    return winner_side;
}

void GamePlay::go_next_round_or_end() const {
    try {
        game->go_next_round();
    }
    catch (const LastRoundException& ex) {
        game->end();
    }
}

void GamePlay::find_winner_loser(Side& winner_side, Side& loser_side) const {
    uint counter_terrorist_alive_count = game->get_alive_player_count(COUNTER_TERRORIST);
    uint terrorist_alive_count = game->get_alive_player_count(TERRORIST);

    if (terrorist_alive_count > 0 && counter_terrorist_alive_count == 0) {
        winner_side = TERRORIST;
        loser_side = COUNTER_TERRORIST;
    }
    else {
        winner_side = COUNTER_TERRORIST;
        loser_side = TERRORIST;
    }
}

void GamePlay::reset_players_and_add_money(Side side, uint money) const {
    for (const auto& player : game->get_all_players(side)) {
        player->reset_hp();
        player->add_money(money);
    }
}

vector<shared_ptr<Player>> GamePlay::get_scoreboard(Side side) const {
    auto players = game->get_all_players(side);

    sort(players.begin(), players.end(), scoreboard_comparer);

    return players;
}

bool GamePlay::scoreboard_comparer(const shared_ptr<Player>& p1, const shared_ptr<Player>& p2) {
    if(p1->get_kills() != p2->get_kills()) {
        return p1->get_kills() > p2->get_kills();
    }
    if(p1->get_deaths() != p2->get_deaths()) {
        return p1->get_deaths() < p2->get_deaths();
    }
    return p1->get_entry_time() < p2->get_entry_time();
}

bool GamePlay::has_ended() const {
    return game->has_ended();
}
