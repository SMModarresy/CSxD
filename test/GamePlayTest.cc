#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "utils/data/Data.h"
#include "mocks/MockWeapon.h"
#include "mocks/MockPlayer.h"
#include "mocks/MockGame.h"
#include "GamePlay.h"
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
using ::testing::ElementsAreArray;

TEST(GamePlayTest, SetRoundTimeAssertions) {
    auto mock_game = make_shared<MockGame>();
    GamePlay game_play(mock_game);

    EXPECT_CALL(*mock_game, set_round_time(123))
        .Times(1);

    game_play.set_round_time(123);
}

TEST(GamePlayTest, CreatePlayerAssertions) {
    Data::load();
    GamePlay game_play(10);
    auto player = game_play.create_player("Player", TERRORIST);

    EXPECT_EQ(player->get_name(), "Player");
    EXPECT_EQ(player->get_hp(), 100);
    EXPECT_TRUE(player->is_alive());
    EXPECT_EQ(player->get_kills(), 0);
    EXPECT_EQ(player->get_deaths(), 0);
    EXPECT_EQ(player->get_side(), TERRORIST);
    EXPECT_EQ(player->get_entry_time(), 0);
    EXPECT_EQ(player->get_max_money(), 10000);
    EXPECT_EQ(player->get_money(), 1000);
    EXPECT_EQ(player->get_weapon(MELEE), Data::get_weapon_by_name("Knife"));
}

TEST(GamePlayTest, CreatePlayerAfterTimeLimitAssertions) {
    Data::load();
    GamePlay game_play(10);
    game_play.set_round_time(3 * 1000);
    auto player = game_play.create_player("Player", TERRORIST);

    EXPECT_EQ(player->get_hp(), 0);
    EXPECT_FALSE(player->is_alive());
    EXPECT_EQ(player->get_entry_time(), 3 * 1000);
}

TEST(GamePlayTest, CreatePlayerInNextRoundAssertions) {
    Data::load();
    GamePlay game_play(10);
    game_play.determine_winner_and_go_next_round();
    game_play.set_round_time(3 * 1000);
    auto player = game_play.create_player("Player", TERRORIST);

    EXPECT_EQ(player->get_entry_time(), (2 * 60 + 18) * 1000);
}

TEST(GamePlayTest, AddPlayerAssertions) {
    GamePlay game_play(10);
    auto mock_player = make_shared<MockPlayer>();

    EXPECT_CALL(*mock_player, get_name)
        .Times(2)
        .WillRepeatedly(Return("Player"));
    EXPECT_CALL(*mock_player, get_side)
        .Times(3)
        .WillRepeatedly(Return(TERRORIST));

    game_play.add_player(mock_player);
}

TEST(GamePlayTest, AddNullPlayerAssertions) {
    GamePlay game_play(10);

    EXPECT_THROW(game_play.add_player(nullptr), NullPointerException);
}

TEST(GamePlayTest, AddSamePlayerTwiceAssertions) {
    GamePlay game_play(10);
    auto mock_player1 = make_shared<MockPlayer>();
    auto mock_player2 = make_shared<MockPlayer>();

    EXPECT_CALL(*mock_player1, get_side)
        .Times(5)
        .WillRepeatedly(Return(TERRORIST));
    EXPECT_CALL(*mock_player2, get_side)
        .Times(2)
        .WillOnce(Return(TERRORIST))
        .WillOnce(Return(COUNTER_TERRORIST));

    game_play.add_player(mock_player1);

    EXPECT_THROW(game_play.add_player(mock_player2), PlayerAlreadyInTeamException);
    EXPECT_THROW(game_play.add_player(mock_player2), PlayerInOpponentTeamException);
}

TEST(GamePlayTest, AddPlayerFullTeamAssertions) {
    auto mock_game = make_shared<MockGame>();
    GamePlay game_play(mock_game);
    auto mock_player = make_shared<MockPlayer>();

    EXPECT_CALL(*mock_game, add_player)
        .WillOnce(Throw(TeamIsFullException()));

    EXPECT_THROW(game_play.add_player(mock_player), TeamIsFullException);
}

TEST(GamePlayTest, GetHpAssertions) {
    GamePlay game_play(10);
    auto mock_player = make_shared<MockPlayer>();

    ON_CALL(*mock_player, get_side)
        .WillByDefault(Return(TERRORIST));

    game_play.add_player(mock_player);

    EXPECT_CALL(*mock_player, get_hp)
        .WillOnce(Return(63));

    EXPECT_EQ(game_play.get_hp(""), 63);
}

TEST(GamePlayTest, GetHpNonExistingPlayerAssertions) {
    GamePlay game_play(10);

    EXPECT_THROW(game_play.get_hp("Player"), PlayerNotFoundException);
}

TEST(GamePlayTest, GetMoneyAssertions) {
    GamePlay game_play(10);
    auto mock_player = make_shared<MockPlayer>();

    ON_CALL(*mock_player, get_side)
        .WillByDefault(Return(TERRORIST));

    game_play.add_player(mock_player);

    EXPECT_CALL(*mock_player, get_money)
        .WillOnce(Return(1234));

    EXPECT_EQ(game_play.get_money(""), 1234);
}

TEST(GamePlayTest, GetMoneyNonExistingPlayerAssertions) {
    GamePlay game_play(10);

    EXPECT_THROW(game_play.get_money("Player"), PlayerNotFoundException);
}

TEST(GamePlayTest, BuyWeaponPlayerAssertions) {
    Data::load();
    GamePlay game_play(10);
    auto mock_player = make_shared<MockPlayer>();
    auto weapon = Data::get_weapon_by_name("Glock-18");

    ON_CALL(*mock_player, get_side)
        .WillByDefault(Return(TERRORIST));

    game_play.add_player(mock_player);

    EXPECT_CALL(*mock_player, is_alive)
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_player, get_side)
        .WillOnce(Return(TERRORIST));
    EXPECT_CALL(*mock_player, get_weapon(weapon->get_type()))
        .WillOnce(Throw(WeaponNotEquippedException()));
    EXPECT_CALL(*mock_player, subtract_money(weapon->get_price()))
        .WillOnce(Return());
    EXPECT_CALL(*mock_player, equip_weapon(weapon))
        .WillOnce(Return());

    game_play.buy_weapon("", weapon);
}

TEST(GamePlayTest, BuyWeaponWeaponAssertions) {
    Data::load();
    GamePlay game_play(10);
    auto player = game_play.create_player("Player", TERRORIST);
    auto mock_weapon = make_shared<MockWeapon>();

    game_play.add_player(player);

    EXPECT_CALL(*mock_weapon, is_available_for(player->get_side()))
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_weapon, get_type)
        .Times(2)
        .WillRepeatedly(Return(PISTOL));
    EXPECT_CALL(*mock_weapon, get_price)
        .WillOnce(Return(1000));

    game_play.buy_weapon("Player", mock_weapon);
}

TEST(GamePlayTest, BuyWeaponNonExistingPlayerAssertions) {
    GamePlay game_play(10);
    auto mock_weapon = make_shared<MockWeapon>();

    EXPECT_THROW(game_play.buy_weapon("Player", mock_weapon), PlayerNotFoundException);
}

TEST(GamePlayTest, BuyWeaponDeadPlayerAssertions) {
    GamePlay game_play(10);
    auto mock_player = make_shared<MockPlayer>();
    auto mock_weapon = make_shared<MockWeapon>();

    ON_CALL(*mock_player, get_side)
        .WillByDefault(Return(TERRORIST));

    game_play.add_player(mock_player);

    EXPECT_CALL(*mock_player, is_alive)
        .WillOnce(Return(false));

    EXPECT_THROW(game_play.buy_weapon("", mock_weapon), ActionFromDeadPlayerException);
}

TEST(GamePlayTest, BuyWeaponAfterTimeLimitAssertions) {
    auto mock_game = make_shared<MockGame>();
    GamePlay game_play(mock_game);
    auto mock_player = make_shared<MockPlayer>();
    auto mock_weapon = make_shared<MockWeapon>();

    ON_CALL(*mock_game, get_round_time)
        .WillByDefault(Return(45 * 1000));
    ON_CALL(*mock_game, get_player_by_name)
        .WillByDefault(Return(mock_player));

    ON_CALL(*mock_player, is_alive)
        .WillByDefault(Return(true));
    ON_CALL(*mock_player, get_weapon)
        .WillByDefault(Throw(WeaponNotEquippedException()));

    ON_CALL(*mock_weapon, is_available_for)
        .WillByDefault(Return(true));

    game_play.add_player(mock_player);

    EXPECT_THROW(game_play.buy_weapon("", mock_weapon), ActionAtIllegalTimeException);
}

TEST(GamePlayTest, BuyNullWeaponAssertions) {
    GamePlay game_play(10);
    auto mock_player = make_shared<MockPlayer>();

    ON_CALL(*mock_player, get_side)
        .WillByDefault(Return(TERRORIST));
    ON_CALL(*mock_player, is_alive)
        .WillByDefault(Return(true));

    game_play.add_player(mock_player);

    EXPECT_THROW(game_play.buy_weapon("", nullptr), NullPointerException);
}

TEST(GamePlayTest, BuyWeaponNotAvailableAssertions) {
    GamePlay game_play(10);
    auto mock_player = make_shared<MockPlayer>();
    auto mock_weapon = make_shared<MockWeapon>();

    ON_CALL(*mock_player, get_side)
        .WillByDefault(Return(TERRORIST));
    ON_CALL(*mock_player, is_alive)
        .WillByDefault(Return(true));

    game_play.add_player(mock_player);

    EXPECT_CALL(*mock_weapon, is_available_for)
        .WillOnce(Return(false));

    EXPECT_THROW(game_play.buy_weapon("", mock_weapon), WeaponNotAvailableException);
}

TEST(GamePlayTest, BuyWeaponAlreadyEquippedAssertions) {
    GamePlay game_play(10);
    auto mock_player = make_shared<MockPlayer>();
    auto mock_weapon = make_shared<MockWeapon>();

    ON_CALL(*mock_player, get_side)
        .WillByDefault(Return(TERRORIST));
    ON_CALL(*mock_player, is_alive)
        .WillByDefault(Return(true));

    ON_CALL(*mock_weapon, is_available_for)
        .WillByDefault(Return(true));

    game_play.add_player(mock_player);

    EXPECT_CALL(*mock_player, get_weapon)
        .WillOnce(Return(mock_weapon));

    EXPECT_THROW(game_play.buy_weapon("", mock_weapon), WeaponOfThisTypeAlreadyEquippedException);
}

TEST(GamePlayTest, BuyWeaponNoMoneyAssertions) {
    GamePlay game_play(10);
    auto mock_player = make_shared<MockPlayer>();
    auto mock_weapon = make_shared<MockWeapon>();

    ON_CALL(*mock_player, get_side)
        .WillByDefault(Return(TERRORIST));
    ON_CALL(*mock_player, is_alive)
        .WillByDefault(Return(true));
    ON_CALL(*mock_player, get_weapon)
        .WillByDefault(Throw(WeaponNotEquippedException()));

    ON_CALL(*mock_weapon, is_available_for)
        .WillByDefault(Return(true));
    ON_CALL(*mock_weapon, get_price)
        .WillByDefault(Return(1000));

    game_play.add_player(mock_player);

    EXPECT_CALL(*mock_player, subtract_money(1000))
        .WillRepeatedly(Throw(NotEnoughMoneyException()));

    EXPECT_THROW(game_play.buy_weapon("", mock_weapon), NotEnoughMoneyException);
}

TEST(GamePlayTest, AttackOccurredPlayerAssertions) {
    Data::load();
    GamePlay game_play(10);
    auto mock_attacker = make_shared<MockPlayer>();
    auto mock_attacked = make_shared<MockPlayer>();
    auto weapon = Data::get_weapon_by_name("Glock-18");

    ON_CALL(*mock_attacker, get_name)
        .WillByDefault(Return("Attacker"));
    ON_CALL(*mock_attacker, get_side)
        .WillByDefault(Return(TERRORIST));

    ON_CALL(*mock_attacked, get_name)
        .WillByDefault(Return("Attacked"));
    ON_CALL(*mock_attacked, get_side)
        .WillByDefault(Return(COUNTER_TERRORIST));

    game_play.add_player(mock_attacker);
    game_play.add_player(mock_attacked);

    EXPECT_CALL(*mock_attacker, is_alive)
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_attacker, get_weapon(weapon->get_type()))
        .Times(2)
        .WillRepeatedly(Return(weapon));

    EXPECT_CALL(*mock_attacked, is_alive)
        .Times(2)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(*mock_attacked, take_damage(weapon->get_damage_per_hit()))
        .Times(1);

    game_play.attack_occurred("Attacker", "Attacked", weapon->get_type());
}

TEST(GamePlayTest, AttackOccurredPlayerDiesAssertions) {
    Data::load();
    GamePlay game_play(10);
    auto mock_attacker = make_shared<MockPlayer>();
    auto mock_attacked = make_shared<MockPlayer>();
    auto weapon = Data::get_weapon_by_name("Glock-18");

    ON_CALL(*mock_attacker, get_name)
        .WillByDefault(Return("Attacker"));
    ON_CALL(*mock_attacker, get_side)
        .WillByDefault(Return(TERRORIST));

    ON_CALL(*mock_attacked, get_name)
        .WillByDefault(Return("Attacked"));
    ON_CALL(*mock_attacked, get_side)
        .WillByDefault(Return(COUNTER_TERRORIST));

    game_play.add_player(mock_attacker);
    game_play.add_player(mock_attacked);

    EXPECT_CALL(*mock_attacker, is_alive)
        .WillOnce(Return(true));
    EXPECT_CALL(*mock_attacker, get_weapon(weapon->get_type()))
        .Times(2)
        .WillRepeatedly(Return(weapon));
    EXPECT_CALL(*mock_attacker, add_kill)
        .Times(1);
    EXPECT_CALL(*mock_attacker, add_money(weapon->get_money_per_kill()))
        .Times(1);

    EXPECT_CALL(*mock_attacked, is_alive)
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    EXPECT_CALL(*mock_attacked, take_damage(weapon->get_damage_per_hit()))
        .Times(1);
    EXPECT_CALL(*mock_attacked, drop_weapon(PISTOL))
        .Times(1);
    EXPECT_CALL(*mock_attacked, drop_weapon(HEAVY))
        .Times(1);

    game_play.attack_occurred("Attacker", "Attacked", weapon->get_type());
}

TEST(GamePlayTest, AttackOccurredNonExistingAttackerAssertions) {
    GamePlay game_play(10);

    EXPECT_THROW(game_play.attack_occurred("Attacker", "Attacked", MELEE), PlayerNotFoundException);
}

TEST(GamePlayTest, AttackOccurredNonExistingAttackedAssertions) {
    GamePlay game_play(10);
    auto mock_attacker = make_shared<MockPlayer>();

    ON_CALL(*mock_attacker, get_name)
        .WillByDefault(Return("Attacker"));
    ON_CALL(*mock_attacker, get_side)
        .WillByDefault(Return(TERRORIST));

    game_play.add_player(mock_attacker);

    EXPECT_THROW(game_play.attack_occurred("Attacker", "Attacked", MELEE), PlayerNotFoundException);
}

TEST(GamePlayTest, AttackOccurredDeadAttackerAssertions) {
    GamePlay game_play(10);
    auto mock_attacker = make_shared<MockPlayer>();
    auto mock_attacked = make_shared<MockPlayer>();

    ON_CALL(*mock_attacker, get_name)
        .WillByDefault(Return("Attacker"));
    ON_CALL(*mock_attacker, get_side)
        .WillByDefault(Return(TERRORIST));
    ON_CALL(*mock_attacker, is_alive)
        .WillByDefault(Return(false));

    ON_CALL(*mock_attacked, get_name)
        .WillByDefault(Return("Attacked"));
    ON_CALL(*mock_attacked, get_side)
        .WillByDefault(Return(COUNTER_TERRORIST));

    game_play.add_player(mock_attacker);
    game_play.add_player(mock_attacked);

    EXPECT_THROW(game_play.attack_occurred("Attacker", "Attacked", MELEE), ActionFromDeadPlayerException);
}

TEST(GamePlayTest, AttackOccurredDeadAttackedAssertions) {
    GamePlay game_play(10);
    auto mock_attacker = make_shared<MockPlayer>();
    auto mock_attacked = make_shared<MockPlayer>();

    ON_CALL(*mock_attacker, get_name)
        .WillByDefault(Return("Attacker"));
    ON_CALL(*mock_attacker, get_side)
        .WillByDefault(Return(TERRORIST));
    ON_CALL(*mock_attacker, is_alive)
        .WillByDefault(Return(true));

    ON_CALL(*mock_attacked, get_name)
        .WillByDefault(Return("Attacked"));
    ON_CALL(*mock_attacked, get_side)
        .WillByDefault(Return(COUNTER_TERRORIST));
    ON_CALL(*mock_attacked, is_alive)
        .WillByDefault(Return(false));

    game_play.add_player(mock_attacker);
    game_play.add_player(mock_attacked);

    EXPECT_THROW(game_play.attack_occurred("Attacker", "Attacked", MELEE), AttackDeadPlayerException);
}

TEST(GamePlayTest, AttackOccurredWeaponNotEquippedAssertions) {
    GamePlay game_play(10);
    auto mock_attacker = make_shared<MockPlayer>();
    auto mock_attacked = make_shared<MockPlayer>();

    ON_CALL(*mock_attacker, get_name)
        .WillByDefault(Return("Attacker"));
    ON_CALL(*mock_attacker, get_side)
        .WillByDefault(Return(TERRORIST));

    ON_CALL(*mock_attacker, is_alive)
        .WillByDefault(Return(true));
    ON_CALL(*mock_attacker, get_weapon)
        .WillByDefault(Throw(WeaponNotEquippedException()));

    ON_CALL(*mock_attacked, get_name)
        .WillByDefault(Return("Attacked"));
    ON_CALL(*mock_attacked, get_side)
        .WillByDefault(Return(COUNTER_TERRORIST));
    ON_CALL(*mock_attacked, is_alive)
        .WillByDefault(Return(true));

    game_play.add_player(mock_attacker);
    game_play.add_player(mock_attacked);

    EXPECT_THROW(game_play.attack_occurred("Attacker", "Attacked", MELEE), WeaponNotEquippedException);
}

TEST(GamePlayTest, AttackOccurredFriendlyFireAssertions) {
    Data::load();
    GamePlay game_play(10);
    auto mock_attacker = make_shared<MockPlayer>();
    auto mock_attacked = make_shared<MockPlayer>();
    auto weapon = Data::get_weapon_by_name("Glock-18");

    ON_CALL(*mock_attacker, get_name)
        .WillByDefault(Return("Attacker"));
    ON_CALL(*mock_attacker, get_side)
        .WillByDefault(Return(TERRORIST));
    ON_CALL(*mock_attacker, is_alive)
        .WillByDefault(Return(true));
    ON_CALL(*mock_attacker, get_weapon(weapon->get_type()))
        .WillByDefault(Return(weapon));

    ON_CALL(*mock_attacked, get_name)
        .WillByDefault(Return("Attacked"));
    ON_CALL(*mock_attacked, get_side)
        .WillByDefault(Return(TERRORIST));
    ON_CALL(*mock_attacked, is_alive)
        .WillByDefault(Return(true));

    game_play.add_player(mock_attacker);
    game_play.add_player(mock_attacked);

    EXPECT_THROW(game_play.attack_occurred("Attacker", "Attacked", weapon->get_type()), FriendlyFireException);
}

TEST(GamePlayTest, DetermineWinnerAssertions) {
    auto mock_game = make_shared<MockGame>();
    GamePlay game_play(mock_game);

    EXPECT_CALL(*mock_game, get_alive_player_count(COUNTER_TERRORIST))
        .WillOnce(Return(0))
        .WillOnce(Return(1))
        .WillOnce(Return(0))
        .WillOnce(Return(1));

    EXPECT_CALL(*mock_game, get_alive_player_count(TERRORIST))
        .WillOnce(Return(0))
        .WillOnce(Return(0))
        .WillOnce(Return(1))
        .WillOnce(Return(1));

    EXPECT_EQ(game_play.determine_winner_and_go_next_round(), COUNTER_TERRORIST);
    EXPECT_EQ(game_play.determine_winner_and_go_next_round(), COUNTER_TERRORIST);
    EXPECT_EQ(game_play.determine_winner_and_go_next_round(), TERRORIST);
    EXPECT_EQ(game_play.determine_winner_and_go_next_round(), COUNTER_TERRORIST);
}

TEST(GamePlayTest, GoNextRoundRoundNumberAssertions) {
    auto mock_game = make_shared<MockGame>();
    GamePlay game_play(mock_game);

    EXPECT_CALL(*mock_game, go_next_round)
        .Times(1);
    EXPECT_CALL(*mock_game, end)
        .Times(0);

    game_play.determine_winner_and_go_next_round();
}

TEST(GamePlayTest, GoNextRoundLastRoundAssertions) {
    auto mock_game = make_shared<MockGame>();
    GamePlay game_play(mock_game);

    EXPECT_CALL(*mock_game, go_next_round)
        .WillOnce(Throw(LastRoundException()));
    EXPECT_CALL(*mock_game, end)
        .Times(1);

    game_play.determine_winner_and_go_next_round();
}

TEST(GamePlayTest, GoNextRoundHandlePlayersAssertions) {
    auto mock_game = make_shared<MockGame>();
    GamePlay game_play(mock_game);
    auto mock_winner = make_shared<MockPlayer>();
    auto mock_loser = make_shared<MockPlayer>();

    auto winners = vector<shared_ptr<Player>> {mock_winner};
    auto losers = vector<shared_ptr<Player>> {mock_loser};

    ON_CALL(*mock_game, get_alive_player_count(TERRORIST))
        .WillByDefault(Return(1));
    ON_CALL(*mock_game, get_alive_player_count(COUNTER_TERRORIST))
        .WillByDefault(Return(0));
    ON_CALL(*mock_game, get_all_players(TERRORIST))
        .WillByDefault(Return(winners));
    ON_CALL(*mock_game, get_all_players(COUNTER_TERRORIST))
        .WillByDefault(Return(losers));

    EXPECT_CALL(*mock_winner, reset_hp)
        .Times(1);
    EXPECT_CALL(*mock_winner, add_money(2700))
        .Times(1);

    EXPECT_CALL(*mock_loser, reset_hp)
        .Times(1);
    EXPECT_CALL(*mock_loser, add_money(2400))
        .Times(1);

    game_play.determine_winner_and_go_next_round();
}

TEST(GamePlayTest, ScoreboardAssertions) {
    auto mock_game = make_shared<MockGame>();
    GamePlay game_play(mock_game);
    auto mock_player1 = make_shared<MockPlayer>();
    auto mock_player2 = make_shared<MockPlayer>();
    auto mock_player3 = make_shared<MockPlayer>();
    auto mock_player4 = make_shared<MockPlayer>();

    auto players_in_correct_order = vector<shared_ptr<Player>> {mock_player1, mock_player2, mock_player3, mock_player4};
    auto players_in_reverse_order = vector<shared_ptr<Player>> {mock_player4, mock_player3, mock_player2, mock_player1};
    auto players_in_random_order = vector<shared_ptr<Player>> {mock_player3, mock_player4, mock_player1, mock_player2};

    ON_CALL(*mock_player1, get_kills)
        .WillByDefault(Return(3));
    ON_CALL(*mock_player1, get_deaths)
        .WillByDefault(Return(7));
    ON_CALL(*mock_player1, get_entry_time)
        .WillByDefault(Return(5000));

    ON_CALL(*mock_player2, get_kills)
        .WillByDefault(Return(2));
    ON_CALL(*mock_player2, get_deaths)
        .WillByDefault(Return(5));
    ON_CALL(*mock_player2, get_entry_time)
        .WillByDefault(Return(2500));

    ON_CALL(*mock_player3, get_kills)
        .WillByDefault(Return(2));
    ON_CALL(*mock_player3, get_deaths)
        .WillByDefault(Return(6));
    ON_CALL(*mock_player3, get_entry_time)
        .WillByDefault(Return(1500));

    ON_CALL(*mock_player4, get_kills)
        .WillByDefault(Return(2));
    ON_CALL(*mock_player4, get_deaths)
        .WillByDefault(Return(6));
    ON_CALL(*mock_player4, get_entry_time)
        .WillByDefault(Return(2000));

    EXPECT_CALL(*mock_game, get_all_players)
        .WillOnce(Return(players_in_correct_order))
        .WillOnce(Return(players_in_reverse_order))
        .WillOnce(Return(players_in_random_order));

    EXPECT_THAT(game_play.get_scoreboard(ALL), ElementsAreArray(players_in_correct_order));
    EXPECT_THAT(game_play.get_scoreboard(ALL), ElementsAreArray(players_in_correct_order));
    EXPECT_THAT(game_play.get_scoreboard(ALL), ElementsAreArray(players_in_correct_order));
}

TEST(GamePlayTest, HasEndedAssertions) {
    auto mock_game = make_shared<MockGame>();
    GamePlay game_play(mock_game);

    EXPECT_CALL(*mock_game, has_ended)
        .WillOnce(Return(true))
        .WillOnce(Return(false));

    EXPECT_TRUE(game_play.has_ended());
    EXPECT_FALSE(game_play.has_ended());
}
