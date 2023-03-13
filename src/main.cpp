#include "utils/data/Data.h"
#include "GamePlay.h"
#include "Interactions.h"

int main() {
    Data::load();
    Interactions::init();
    auto game_play = make_shared<GamePlay>(Interactions::get_rounds());
    Interactions::set_game_play(game_play);
    Interactions::begin();
    return 0;
}
