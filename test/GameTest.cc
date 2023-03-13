#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "models/game/Game.h"
#include "exceptions/LastRoundException.h"
#include "exceptions/NullPointerException.h"
#include "exceptions/PlayerAlreadyInTeamException.h"
#include "exceptions/PlayerInOpponentTeamException.h"
#include "exceptions/PlayerNotFoundException.h"
#include "exceptions/TeamIsFullException.h"

using ::testing::ElementsAre;
using ::testing::UnorderedElementsAre;
using ::testing::UnorderedElementsAreArray;

TEST(GameTest, ConstructionAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    EXPECT_EQ(game.get_id(), 1);
    EXPECT_EQ(game.get_rounds(), 13);
    EXPECT_EQ(game.get_current_round(), 1);
    EXPECT_EQ(game.get_round_length(), 180 * 1000);
    EXPECT_EQ(game.get_game_time(), 0);
    EXPECT_EQ(game.get_round_time(), 0);
    EXPECT_EQ(game.get_max_team_size(), 10);
    EXPECT_FALSE(game.has_ended());
}

TEST(GameTest, ConstructionOutOfRangeAssertions) {
    EXPECT_THROW(new Game(1, 0, 1, 1), out_of_range);
    EXPECT_THROW(new Game(1, 1, 0, 1), out_of_range);
    EXPECT_THROW(new Game(1, 1, 1, 0), out_of_range);
}

TEST(GameTest, NextRoundAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    game.go_next_round();

    EXPECT_EQ(game.get_current_round(), 2);
}

TEST(GameTest, NextRoundOnLastRoundAssertions) {
    Game game(1, 1, 180 * 1000, 10);

    EXPECT_THROW(game.go_next_round(), LastRoundException);
}

TEST(GameTest, RoundTimeAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    game.set_round_time(12 * 1000);

    EXPECT_EQ(game.get_round_time(), 12 * 1000);
}

TEST(GameTest, NextRoundTimeResetAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    game.set_round_time(12 * 1000);
    game.go_next_round();

    EXPECT_EQ(game.get_round_time(), 0);
}

TEST(GameTest, NextRoundTimeAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    game.go_next_round();
    game.set_round_time(12 * 1000);

    EXPECT_EQ(game.get_round_time(), 12 * 1000);
}

TEST(GameTest, GameTimeAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    game.go_next_round();
    game.set_round_time(12 * 1000);

    EXPECT_EQ(game.get_game_time(), 192 * 1000);
}

TEST(GameTest, MaxTeamSizeAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    game.set_max_team_size(5);

    EXPECT_EQ(game.get_max_team_size(), 5);
}

TEST(GameTest, EndGameAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    game.end();

    EXPECT_TRUE(game.has_ended());
}

TEST(GameTest, AddPlayerAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    shared_ptr<Player> terrorist_player = make_shared<Player>("Terrorist", 100, 10000, 1000, TERRORIST, 1);
    shared_ptr<Player> counter_terrorist_player = make_shared<Player>("Counter-Terrorist", 100, 10000, 1000, COUNTER_TERRORIST, 2);

    game.add_player(terrorist_player);
    game.add_player(counter_terrorist_player);

    EXPECT_EQ(game.get_player_by_name("Terrorist"), terrorist_player);
    EXPECT_EQ(game.get_player_by_name("Counter-Terrorist"), counter_terrorist_player);
}

TEST(GameTest, AddNullPlayerAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    EXPECT_THROW(game.add_player(nullptr), NullPointerException);
}

TEST(GameTest, AddSamePlayerTwiceAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    shared_ptr<Player> terrorist_player = make_shared<Player>("Terrorist", 100, 10000, 1000, TERRORIST, 1);
    shared_ptr<Player> terrorist_player_wrong_side = make_shared<Player>("Terrorist", 100, 10000, 1000, COUNTER_TERRORIST, 1);
    shared_ptr<Player> counter_terrorist_player = make_shared<Player>("Counter-Terrorist", 100, 10000, 1000, COUNTER_TERRORIST, 2);
    shared_ptr<Player> counter_terrorist_player_wrong_side = make_shared<Player>("Counter-Terrorist", 100, 10000, 1000, TERRORIST, 2);

    game.add_player(terrorist_player);
    game.add_player(counter_terrorist_player);

    EXPECT_THROW(game.add_player(terrorist_player), PlayerAlreadyInTeamException);
    EXPECT_THROW(game.add_player(terrorist_player_wrong_side), PlayerInOpponentTeamException);
    EXPECT_THROW(game.add_player(counter_terrorist_player), PlayerAlreadyInTeamException);
    EXPECT_THROW(game.add_player(counter_terrorist_player_wrong_side), PlayerInOpponentTeamException);
}

TEST(GameTest, NotAddedPlayerAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    EXPECT_THROW(game.get_player_by_name("Terrorist"), PlayerNotFoundException);
}

TEST(GameTest, AddPlayerToFullTeamAssertions) {
    Game game(1, 13, 180 * 1000, 2);

    shared_ptr<Player> terrorist_player1 = make_shared<Player>("Terrorist1", 100, 10000, 1000, TERRORIST, 1);
    shared_ptr<Player> terrorist_player2 = make_shared<Player>("Terrorist2", 100, 10000, 1000, TERRORIST, 1);
    shared_ptr<Player> counter_terrorist_player1 = make_shared<Player>("Counter-Terrorist1", 100, 10000, 1000, COUNTER_TERRORIST, 2);
    shared_ptr<Player> counter_terrorist_player2 = make_shared<Player>("Counter-Terrorist2", 100, 10000, 1000, COUNTER_TERRORIST, 2);
    shared_ptr<Player> extra_player = make_shared<Player>("Player", 100, 10000, 1000, COUNTER_TERRORIST, 2);

    game.add_player(terrorist_player1);
    game.add_player(terrorist_player2);
    game.add_player(counter_terrorist_player1);
    game.add_player(counter_terrorist_player2);

    EXPECT_THROW(game.add_player(extra_player), TeamIsFullException);
    EXPECT_THROW(game.add_player(extra_player), TeamIsFullException);
}

TEST(GameTest, GetAllPlayerAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    shared_ptr<Player> terrorist_player1 = make_shared<Player>("Terrorist1", 100, 10000, 1000, TERRORIST, 1);
    shared_ptr<Player> terrorist_player2 = make_shared<Player>("Terrorist2", 0, 10000, 1000, TERRORIST, 1);
    shared_ptr<Player> counter_terrorist_player1 = make_shared<Player>("Counter-Terrorist1", 0, 10000, 1000, COUNTER_TERRORIST, 2);
    shared_ptr<Player> counter_terrorist_player2 = make_shared<Player>("Counter-Terrorist2", 100, 10000, 1000, COUNTER_TERRORIST, 2);

    game.add_player(terrorist_player1);
    game.add_player(terrorist_player2);
    game.add_player(counter_terrorist_player1);
    game.add_player(counter_terrorist_player2);

    vector<shared_ptr<Player>> all_players;
    vector<shared_ptr<Player>> terrorist_players;
    vector<shared_ptr<Player>> counter_terrorist_players;

    all_players.push_back(terrorist_player1);
    all_players.push_back(terrorist_player2);
    all_players.push_back(counter_terrorist_player1);
    all_players.push_back(counter_terrorist_player2);

    terrorist_players.push_back(terrorist_player1);
    terrorist_players.push_back(terrorist_player2);

    counter_terrorist_players.push_back(counter_terrorist_player1);
    counter_terrorist_players.push_back(counter_terrorist_player2);

    EXPECT_THAT(game.get_all_players(ALL), UnorderedElementsAreArray(all_players));
    EXPECT_THAT(game.get_all_players(TERRORIST), UnorderedElementsAreArray(terrorist_players));
    EXPECT_THAT(game.get_all_players(COUNTER_TERRORIST), UnorderedElementsAreArray(counter_terrorist_players));
}

TEST(GameTest, GetAlivePlayersAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    shared_ptr<Player> terrorist_player1 = make_shared<Player>("Terrorist1", 100, 10000, 1000, TERRORIST, 1);
    shared_ptr<Player> terrorist_player2 = make_shared<Player>("Terrorist2", 100, 10000, 1000, TERRORIST, 1);
    shared_ptr<Player> counter_terrorist_player1 = make_shared<Player>("Counter-Terrorist1", 100, 10000, 1000, COUNTER_TERRORIST, 2);
    shared_ptr<Player> counter_terrorist_player2 = make_shared<Player>("Counter-Terrorist2", 100, 10000, 1000, COUNTER_TERRORIST, 2);

    game.add_player(terrorist_player1);
    game.add_player(terrorist_player2);
    game.add_player(counter_terrorist_player1);
    game.add_player(counter_terrorist_player2);

    terrorist_player1->take_damage(50);
    terrorist_player2->take_damage(100);
    counter_terrorist_player1->take_damage(200);

    EXPECT_THAT(game.get_alive_players(ALL), UnorderedElementsAre(terrorist_player1, counter_terrorist_player2));
    EXPECT_THAT(game.get_alive_players(TERRORIST), ElementsAre(terrorist_player1));
    EXPECT_THAT(game.get_alive_players(COUNTER_TERRORIST), ElementsAre(counter_terrorist_player2));
}

TEST(GameTest, GetAlivePlayerCountAssertions) {
    Game game(1, 13, 180 * 1000, 10);

    shared_ptr<Player> terrorist_player1 = make_shared<Player>("Terrorist1", 100, 10000, 1000, TERRORIST, 1);
    shared_ptr<Player> terrorist_player2 = make_shared<Player>("Terrorist2", 100, 10000, 1000, TERRORIST, 1);
    shared_ptr<Player> counter_terrorist_player1 = make_shared<Player>("Counter-Terrorist1", 100, 10000, 1000, COUNTER_TERRORIST, 2);
    shared_ptr<Player> counter_terrorist_player2 = make_shared<Player>("Counter-Terrorist2", 100, 10000, 1000, COUNTER_TERRORIST, 2);

    game.add_player(terrorist_player1);
    game.add_player(terrorist_player2);
    game.add_player(counter_terrorist_player1);
    game.add_player(counter_terrorist_player2);

    terrorist_player1->take_damage(50);
    terrorist_player2->take_damage(100);
    counter_terrorist_player1->take_damage(200);

    EXPECT_EQ(game.get_alive_player_count(ALL), 2);
    EXPECT_EQ(game.get_alive_player_count(TERRORIST), 1);
    EXPECT_EQ(game.get_alive_player_count(COUNTER_TERRORIST), 1);
}