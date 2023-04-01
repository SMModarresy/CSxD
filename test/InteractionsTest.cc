#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "utils/data/Data.h"
#include "mocks/MockWeapon.h"
#include "mocks/MockPlayer.h"
#include "mocks/MockGame.h"
#include "GamePlay.h"
#include "mocks/MockGamePlay.h"
#include "Interactions.h"
#include "exceptions/ActionAtIllegalTimeException.h"
#include "exceptions/ActionFromDeadPlayerException.h"
#include "exceptions/AttackDeadPlayerException.h"
#include "exceptions/FriendlyFireException.h"
#include "exceptions/LastRoundException.h"
#include "exceptions/NotEnoughMoneyException.h"
#include "exceptions/NullPointerException.h"
#include "exceptions/PlayerAlreadyInTeamException.h"
#include "exceptions/PlayerInOpponentTeamException.h"
#include "exceptions/PlayerNotFoundException.h"
#include "exceptions/TeamIsFullException.h"
#include "exceptions/WeaponNotAvailableException.h"
#include "exceptions/WeaponNotEquippedException.h"
#include "exceptions/WeaponOfThisTypeAlreadyEquippedException.h"

using ::testing::Return;
using ::testing::Throw;
using ::testing::Eq;

TEST(InteractionsTest, InitAssertions) {
    string input = "5";
    stringstream input_stream(input);
    Interactions::set_input_stream(input_stream);

    Interactions::init();

    EXPECT_EQ(Interactions::get_rounds(), 5);
}

TEST(InteractionsTest, AddUserAssertions) {
    string input = "1\nROUND 1\nADD-USER Player Counter-Terrorist 00:01:000";
    string expected = "this user added to Counter-Terrorist\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();
    auto mock_player = make_shared<MockPlayer>();

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, set_round_time(1000))
        .Times(1);
    EXPECT_CALL(*mock_game_play, create_player("Player", COUNTER_TERRORIST))
        .WillOnce(Return(mock_player));
    EXPECT_CALL(*mock_game_play, add_player(Eq(mock_player)))
        .Times(1);
    EXPECT_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillOnce(Return(COUNTER_TERRORIST));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, AddUserAlreadyInTeamAssertions) {
    string input = "1\nROUND 1\nADD-USER Player Counter-Terrorist 00:01:000";
    string expected = "you are already in this game\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, add_player)
        .WillOnce(Throw(PlayerAlreadyInTeamException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, AddUserInOpponentTeamAssertions) {
    string input = "1\nROUND 1\nADD-USER Player Counter-Terrorist 00:01:000";
    string expected = "you are already in this game\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, add_player)
        .WillOnce(Throw(PlayerInOpponentTeamException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, AddUserFullTeamAssertions) {
    string input = "1\nROUND 1\nADD-USER Player Counter-Terrorist 00:01:000";
    string expected = "this team is full\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, add_player)
        .WillOnce(Throw(TeamIsFullException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, GetHealthAssertions) {
    string input = "1\nROUND 1\nGET-HEALTH Player 00:01:000";
    string expected = "63\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, set_round_time(1000))
        .Times(1);
    EXPECT_CALL(*mock_game_play, get_hp("Player"))
        .WillOnce(Return(63));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, GetHealthNonExistingPlayerAssertions) {
    string input = "1\nROUND 1\nGET-HEALTH Player 00:01:000";
    string expected = "invalid username\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, get_hp)
        .WillOnce(Throw(PlayerNotFoundException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, GetMoneyAssertions) {
    string input = "1\nROUND 1\nGET-MONEY Player 00:01:000";
    string expected = "3700\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, set_round_time(1000))
        .Times(1);
    EXPECT_CALL(*mock_game_play, get_money("Player"))
        .WillOnce(Return(3700));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, GetMoneyNonExistingPlayerAssertions) {
    string input = "1\nROUND 1\nGET-MONEY Player 00:01:000";
    string expected = "invalid username\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, get_money)
        .WillOnce(Throw(PlayerNotFoundException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, BuyAssertions) {
    string input = "1\nROUND 1\nBUY Player AWP 00:01:000";
    string expected = "I hope you can use it\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);
    Data::load();

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended)
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, set_round_time(1000))
        .Times(1);
    EXPECT_CALL(*mock_game_play, buy_weapon("Player", Eq(Data::get_weapon_by_name("AWP"))))
        .Times(1);

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, BuyNonExistingPlayerAssertions) {
    string input = "1\nROUND 1\nBUY Player AWP 00:01:000";
    string expected = "invalid username\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, buy_weapon)
        .WillOnce(Throw(PlayerNotFoundException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, BuyDeadPlayerAssertions) {
    string input = "1\nROUND 1\nBUY Player AWP 00:01:000";
    string expected = "deads can not buy\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, buy_weapon)
        .WillOnce(Throw(ActionFromDeadPlayerException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, BuyAfterTimeLimitAssertions) {
    string input = "1\nROUND 1\nBUY Player AWP 00:01:000";
    string expected = "you are out of time\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, buy_weapon)
        .WillOnce(Throw(ActionAtIllegalTimeException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, BuyNonExistingWeaponAssertions) {
    string input = "1\nROUND 1\nBUY Player AWP 00:01:000";
    string expected = "invalid category gun\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, buy_weapon)
        .WillOnce(Throw(NullPointerException("weapon")));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, BuyWeaponNotAvailableAssertions) {
    string input = "1\nROUND 1\nBUY Player AWP 00:01:000";
    string expected = "invalid category gun\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, buy_weapon)
        .WillOnce(Throw(WeaponNotAvailableException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, BuyAlreadyEquippedAssertions) {
    string input = "1\nROUND 1\nBUY Player AWP 00:01:000";
    string expected = "you have a heavy\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);
    Data::load();

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, buy_weapon)
        .WillOnce(Throw(WeaponOfThisTypeAlreadyEquippedException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, BuyNotEnoughMoneyAssertions) {
    string input = "1\nROUND 1\nBUY Player AWP 00:01:000";
    string expected = "no enough money\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended())
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, buy_weapon)
        .WillOnce(Throw(NotEnoughMoneyException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, TapAssertions) {
    string input = "1\nROUND 1\nTAP Attacker Attacked heavy 01:00:000";
    string expected = "nice shot\nTerrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended)
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, set_round_time(60 * 1000))
        .Times(1);
    EXPECT_CALL(*mock_game_play, attack_occurred("Attacker", "Attacked", HEAVY))
        .Times(1);

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, TapNonExistingPlayerAssertions) {
    string input = "1\nROUND 1\nTAP Attacker Attacked heavy 01:00:000";
    string expected = "invalid username\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended)
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, attack_occurred)
        .WillOnce(Throw(PlayerNotFoundException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, TapDeadAttackerAssertions) {
    string input = "1\nROUND 1\nTAP Attacker Attacked heavy 01:00:000";
    string expected = "attacker is dead\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended)
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, attack_occurred)
        .WillOnce(Throw(ActionFromDeadPlayerException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, TapDeadAttackedAssertions) {
    string input = "1\nROUND 1\nTAP Attacker Attacked heavy 01:00:000";
    string expected = "attacked is dead\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended)
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, attack_occurred)
        .WillOnce(Throw(AttackDeadPlayerException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, TapWeaponNotEquippedAssertions) {
    string input = "1\nROUND 1\nTAP Attacker Attacked heavy 01:00:000";
    string expected = "no such gun\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended)
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, attack_occurred)
        .WillOnce(Throw(WeaponNotEquippedException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, TapFriendlyFireAssertions) {
    string input = "1\nROUND 1\nTAP Attacker Attacked heavy 01:00:000";
    string expected = "friendly fire\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    EXPECT_CALL(*mock_game_play, has_ended)
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, attack_occurred)
        .WillOnce(Throw(FriendlyFireException()));

    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}

TEST(InteractionsTest, ScoreboardAssertions) {
    string input = "1\nROUND 1\nSCORE-BOARD 02:03:000";
    string expected = "Counter-Terrorist-Players:\n1 CT-1 3 1\n2 CT-2 0 0\nTerrorist-Players:\n1 Terrorist 1 3\nCounter-Terrorist won\n";
    stringstream input_stream(input);
    ostringstream output_stream;
    Interactions::set_input_stream(input_stream);
    Interactions::set_output_stream(output_stream);

    auto mock_game_play = make_shared<MockGamePlay>();

    auto mock_counter_terrorist1 = make_shared<MockPlayer>();
    auto mock_counter_terrorist2 = make_shared<MockPlayer>();
    auto mock_terrorist = make_shared<MockPlayer>();

    auto counter_terrorists = vector<shared_ptr<Player>> {mock_counter_terrorist1, mock_counter_terrorist2};
    auto terrorists = vector<shared_ptr<Player>> {mock_terrorist};

    ON_CALL(*mock_game_play, determine_winner_and_go_next_round)
        .WillByDefault(Return(COUNTER_TERRORIST));

    ON_CALL(*mock_counter_terrorist1, get_name)
        .WillByDefault(Return("CT-1"));
    ON_CALL(*mock_counter_terrorist1, get_kills)
        .WillByDefault(Return(3));
    ON_CALL(*mock_counter_terrorist1, get_deaths)
        .WillByDefault(Return(1));

    ON_CALL(*mock_counter_terrorist2, get_name)
        .WillByDefault(Return("CT-2"));
    ON_CALL(*mock_counter_terrorist2, get_kills)
        .WillByDefault(Return(0));
    ON_CALL(*mock_counter_terrorist2, get_deaths)
        .WillByDefault(Return(0));

    ON_CALL(*mock_terrorist, get_name)
        .WillByDefault(Return("Terrorist"));
    ON_CALL(*mock_terrorist, get_kills)
        .WillByDefault(Return(1));
    ON_CALL(*mock_terrorist, get_deaths)
        .WillByDefault(Return(3));

    EXPECT_CALL(*mock_game_play, has_ended)
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_game_play, get_scoreboard(COUNTER_TERRORIST))
        .WillOnce(Return(counter_terrorists));
    EXPECT_CALL(*mock_game_play, get_scoreboard(TERRORIST))
        .WillOnce(Return(terrorists));
    
    Interactions::init();
    Interactions::set_game_play(mock_game_play);
    Interactions::begin();

    string output = output_stream.str();
    EXPECT_EQ(output, expected);
}
