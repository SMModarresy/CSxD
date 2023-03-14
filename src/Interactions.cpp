#include <utility>

#include "Interactions.h"
#include "exceptions/ActionAtIllegalTimeException.h"
#include "exceptions/ActionFromDeadPlayerException.h"
#include "exceptions/AttackDeadPlayerException.h"
#include "exceptions/FriendlyFireException.h"
#include "exceptions/NotEnoughMoneyException.h"
#include "exceptions/NullPointerException.h"
#include "exceptions/PlayerAlreadyInTeamException.h"
#include "exceptions/PlayerInOpponentTeamException.h"
#include "exceptions/PlayerNotFoundException.h"
#include "exceptions/TeamIsFullException.h"
#include "exceptions/WeaponNotAvailableException.h"
#include "exceptions/WeaponNotEquippedException.h"
#include "exceptions/WeaponOfThisTypeAlreadyEquippedException.h"

istream* Interactions::in = &cin;
ostream* Interactions::out = &cout;
uint Interactions::rounds;
shared_ptr<GamePlay> Interactions::game_play;

void Interactions::set_input_stream(istream& stream) {
    in = &stream;
}

void Interactions::set_output_stream(ostream& stream) {
    out = &stream;
}

void Interactions::init() {
    *in >> rounds;
}

uint Interactions::get_rounds() {
    return rounds;
}

void Interactions::set_game_play(shared_ptr<GamePlay> game_play) {
    Interactions::game_play = game_play;
}

void Interactions::begin() {
    string command;
    uint command_count;
    while (!game_play->has_ended()) {
        *in >> command >> command_count;
        while (command_count--) {
            *in >> command;
            switch (get_command_from_string(command)) {
                case ADD_USER: {
                    add_user();
                    break;
                }
                case GET_HEALTH: {
                    get_health();
                    break;
                }
                case GET_MONEY: {
                    get_money();
                    break;
                }
                case BUY: {
                    buy();
                    break;
                }
                case TAP: {
                    tap();
                    break;
                }
                case SCORE_BOARD: {
                    scoreboard();
                    break;
                }
            }
        }
        auto round_winner = game_play->determine_winner_and_go_next_round();
        if (round_winner == COUNTER_TERRORIST) {
            *out << "Counter-Terrorist won" << endl;
        }
        else {
            *out << "Terrorist won" << endl;
        }
    }
}

void Interactions::add_user() {
    string name, side, time;
    *in >> name >> side >> time;
    game_play->set_round_time(get_time_from_string(time));

    auto player = game_play->create_player(name, get_side_from_string(side));
    try {
        game_play->add_player(player);
        *out << "this user added to " << side << endl;
    }
    catch (const PlayerAlreadyInTeamException& ex) {
        *out << "you are already in this game" << endl;
    }
    catch (const PlayerInOpponentTeamException& ex) {
        *out << "you are already in this game" << endl;
    }
    catch (const TeamIsFullException& ex) {
        *out << "this team is full" << endl;
    }
}

void Interactions::get_health() {
    string player_name, time;
    *in >> player_name >> time;
    game_play->set_round_time(get_time_from_string(time));

    try {
        *out << game_play->get_hp(player_name) << endl;
    }
    catch (const PlayerNotFoundException& ex) {
        *out << "invalid username" << endl;
    }
}

void Interactions::get_money() {
    string player_name, time;
    *in >> player_name >> time;
    game_play->set_round_time(get_time_from_string(time));

    try {
        *out << game_play->get_money(player_name) << endl;
    }
    catch (const PlayerNotFoundException& ex) {
        *out << "invalid username" << endl;
    }
}

void Interactions::buy() {
    string player_name, weapon_name, time;
    *in >> player_name >> weapon_name >> time;
    game_play->set_round_time(get_time_from_string(time));

    shared_ptr<Weapon> weapon;
    try {
        weapon = Data::get_weapon_by_name(weapon_name);
    }
    catch (...) { }
    try {
        game_play->buy_weapon(player_name, weapon);
        *out << "I hope you can use it" << endl;
    }
    catch (const PlayerNotFoundException& ex) {
        *out << "invalid username" << endl;
    }
    catch (const ActionFromDeadPlayerException& ex) {
        *out << "deads can not buy" << endl;
    }
    catch (const ActionAtIllegalTimeException& ex) {
        *out << "you are out of time" << endl;
    }
    catch (const NullPointerException& ex) {
        *out << "invalid category gun" << endl;
    }
    catch (const WeaponNotAvailableException& ex) {
        *out << "invalid category gun" << endl;
    }
    catch (const WeaponOfThisTypeAlreadyEquippedException& ex) {
        *out << "you have a " << (weapon->get_type() == PISTOL ? "pistol" : "heavy") << endl;
    }
    catch (const NotEnoughMoneyException& ex) {
        *out << "no enough money" << endl;
    }
}

void Interactions::tap() {
    string attacker_name, attacked_name, weapon_type, time;
    *in >> attacker_name >> attacked_name >> weapon_type >> time;
    game_play->set_round_time(get_time_from_string(time));

    try {
        game_play->attack_occurred(attacker_name, attacked_name, get_weapon_type_from_string(weapon_type));
        *out << "nice shot" << endl;
    }
    catch (const PlayerNotFoundException& ex) {
        *out << "invalid username" << endl;
    }
    catch (const ActionFromDeadPlayerException& ex) {
        *out << "attacker is dead" << endl;
    }
    catch (const AttackDeadPlayerException& ex) {
        *out << "attacked is dead" << endl;
    }
    catch (const WeaponNotEquippedException& ex) {
        *out << "no such gun" << endl;
    }
    catch (const FriendlyFireException& ex) {
        *out << "friendly fire" << endl;
    }
}

void Interactions::scoreboard() {
    string time;
    *in >> time;
    game_play->set_round_time(get_time_from_string(time));

    *out << "Counter-Terrorist-Players:" << endl;
    uint rank = 1;
    for(const auto& player : game_play->get_scoreboard(COUNTER_TERRORIST)) {
        *out << rank++ << " " << player->get_name() << " " << player->get_kills() << " " << player->get_deaths() << endl;
    }
    *out << "Terrorist-Players:" << endl;
    rank = 1;
    for(const auto& player : game_play->get_scoreboard(TERRORIST)) {
        *out << rank++ << " " << player->get_name() << " " << player->get_kills() << " " << player->get_deaths() << endl;
    }
}

ull Interactions::get_time_from_string(const string& time) {
    ull t = 0;
    t += (time[0] - '0') * 10 * 60 * 1000;
    t += (time[1] - '0') * 60 * 1000;
    t += (time[3] - '0') * 10 * 1000;
    t += (time[4] - '0') * 1000;
    t += (time[6] - '0') * 100;
    t += (time[7] - '0') * 10;
    t += (time[8] - '0');
    return t;
}

Command Interactions::get_command_from_string(const string& command) {
    if (command == "ADD-USER")
        return ADD_USER;
    if (command == "GET-HEALTH")
        return GET_HEALTH;
    if (command == "GET-MONEY")
        return GET_MONEY;
    if (command == "BUY")
        return BUY;
    if (command == "TAP")
        return TAP;
    if (command == "SCORE-BOARD")
        return SCORE_BOARD;
    throw invalid_argument("command is invalid. should be one of: [ADD-USER, GET-HEALTH, GET-MONEY, BUY, TAP, SCORE-BOARD]");
}

Side Interactions::get_side_from_string(const string& side) {
    if (side == "Counter-Terrorist")
        return COUNTER_TERRORIST;
    if (side == "Terrorist")
        return TERRORIST;
    throw invalid_argument("side is invalid. should be one of: [Counter-Terrorist, Terrorist]");
}

WeaponType Interactions::get_weapon_type_from_string(const string& weapon_type) {
    if (weapon_type == "knife")
        return MELEE;
    if (weapon_type == "pistol")
        return PISTOL;
    if (weapon_type == "heavy")
        return HEAVY;
    throw invalid_argument("weapon_type is invalid. should be one of: [knife, pistol, heavy]");
}
