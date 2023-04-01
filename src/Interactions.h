#ifndef CSXD_INTERACTIONS_H
#define CSXD_INTERACTIONS_H


#include <string>
#include <iostream>
#include <algorithm>

#include "Command.h"
#include "models/weapon/WeaponType.h"
#include "models/player/Side.h"
#include "utils/data/Data.h"
#include "GamePlay.h"

using namespace std;

class Interactions {
public:
    static void set_input_stream(istream& stream);
    static void set_output_stream(ostream& stream);
    static void init();
    static uint get_rounds();
    static void set_game_play(shared_ptr<GamePlay> game_play);
    static void begin();

private:
    static void execute_command(Command command);
    static void output_winner_and_go_next_round();
    static void add_user();
    static void get_health();
    static void get_money();
    static void buy();
    static void tap();
    static void scoreboard();
    static void print_scoreboard(Side side);
    static void update_round_time();
    static ull get_time_from_string(const string& time);
    static Command get_command_from_string(const string& command);
    static Side get_side_from_string(const string& side);
    static WeaponType get_weapon_type_from_string(const string& weapon_type);

    static istream* in;
    static ostream* out;
    static uint rounds;
    static shared_ptr<GamePlay> game_play;
};


#endif //CSXD_INTERACTIONS_H
