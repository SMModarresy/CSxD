#include "gtest/gtest.h"

#include "models/player/Player.h"
#include "exceptions/NotEnoughMoneyException.h"
#include "exceptions/NullPointerException.h"
#include "exceptions/WeaponNotEquippedException.h"

TEST(PlayerTest, ConstructionAssertions) {
    Player player("Player", 63, 10000, 1000, TERRORIST, 2023);

    EXPECT_EQ(player.get_name(), "Player");
    EXPECT_EQ(player.get_hp(), 63);
    EXPECT_TRUE(player.is_alive());
    EXPECT_EQ(player.get_kills(), 0);
    EXPECT_EQ(player.get_deaths(), 0);
    EXPECT_EQ(player.get_side(), TERRORIST);
    EXPECT_EQ(player.get_entry_time(), 2023);
    EXPECT_EQ(player.get_max_money(), 10000);
    EXPECT_EQ(player.get_money(), 1000);
}

TEST(PlayerTest, DeadConstructionAssertions) {
    Player player("Player", 0, 10000, 0, TERRORIST, 2023);

    EXPECT_FALSE(player.is_alive());
    EXPECT_EQ(player.get_deaths(), 0);
}

TEST(PlayerTest, ConstructionNoWeaponEquippedAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    EXPECT_THROW(player.get_weapon(MELEE), WeaponNotEquippedException);
    EXPECT_THROW(player.get_weapon(PISTOL), WeaponNotEquippedException);
    EXPECT_THROW(player.get_weapon(HEAVY), WeaponNotEquippedException);
}

TEST(PlayerTest, ConstructionOutOfRangeAssertions) {
    EXPECT_THROW(new Player("Player", 101, 10000, 0, TERRORIST, 2023), out_of_range);
}

TEST(PlayerTest, AddHpAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.add_hp(10);

    EXPECT_EQ(player.get_hp(), 73);
}

TEST(PlayerTest, AddHpLimitAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.add_hp(50);

    EXPECT_EQ(player.get_hp(), 100);
}

TEST(PlayerTest, TakeDamageAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.take_damage(10);

    EXPECT_EQ(player.get_hp(), 53);
}

TEST(PlayerTest, TakeDamageLimitAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.take_damage(70);

    EXPECT_EQ(player.get_hp(), 0);
}

TEST(PlayerTest, TakeDamageDeathCountAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.take_damage(63);

    EXPECT_EQ(player.get_deaths(), 1);
}

TEST(PlayerTest, TakeLowDamageDeathCountAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.take_damage(10);

    EXPECT_EQ(player.get_deaths(), 0);
}

TEST(PlayerTest, DeadTakeDamageDeathCountAssertions) {
    Player player("Player", 0, 10000, 0, TERRORIST, 2023);

    player.take_damage(33);

    EXPECT_EQ(player.get_deaths(), 0);
}

TEST(PlayerTest, MultipleDeathCountAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.take_damage(100);
    player.reset_hp();
    player.take_damage(100);
    player.reset_hp();
    player.take_damage(100);

    EXPECT_EQ(player.get_deaths(), 3);
}

TEST(PlayerTest, TakeDamageAliveAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.take_damage(63);

    EXPECT_FALSE(player.is_alive());
}

TEST(PlayerTest, TakeLowDamageAliveAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.take_damage(10);

    EXPECT_TRUE(player.is_alive());
}

TEST(PlayerTest, TakeDamageResetHpAliveAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.take_damage(100);
    player.reset_hp();

    EXPECT_TRUE(player.is_alive());
}

TEST(PlayerTest, DeadResetHpAliveAssertions) {
    Player player("Player", 0, 10000, 0, TERRORIST, 2023);

    player.reset_hp();

    EXPECT_TRUE(player.is_alive());
}

TEST(PlayerTest, AddMoneyAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.add_money(1000);

    EXPECT_EQ(player.get_money(), 1000);
}

TEST(PlayerTest, AddMoneyLimitAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.add_money(20000);

    EXPECT_EQ(player.get_money(), 10000);
}

TEST(PlayerTest, MultipleAddMoneyAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.add_money(1000);
    player.add_money(1000);

    EXPECT_EQ(player.get_money(), 2000);
}

TEST(PlayerTest, SubtractMoneyAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.add_money(3000);
    player.subtract_money(2000);

    EXPECT_EQ(player.get_money(), 1000);
}

TEST(PlayerTest, SubtractMoneyNotEnoughMoneyAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.add_money(3000);

    EXPECT_THROW(player.subtract_money(4000), NotEnoughMoneyException);
}

TEST(PlayerTest, AddKillAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.add_kill();

    EXPECT_EQ(player.get_kills(), 1);
}

TEST(PlayerTest, AddMultipleKillsAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    player.add_kill();
    player.add_kill();
    player.add_kill();

    EXPECT_EQ(player.get_kills(), 3);
}

TEST(PlayerTest, EquipWeaponAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    shared_ptr<Weapon> melee = make_shared<Weapon>("Melee", 1000, 10, 100, MELEE, TERRORIST);
    shared_ptr<Weapon> pistol = make_shared<Weapon>("Pistol", 1000, 10, 100, PISTOL, TERRORIST);
    shared_ptr<Weapon> heavy = make_shared<Weapon>("Heavy", 1000, 10, 100, HEAVY, TERRORIST);

    player.equip_weapon(melee);
    player.equip_weapon(pistol);
    player.equip_weapon(heavy);

    EXPECT_EQ(player.get_weapon(MELEE), melee);
    EXPECT_EQ(player.get_weapon(PISTOL), pistol);
    EXPECT_EQ(player.get_weapon(HEAVY), heavy);
}

TEST(PlayerTest, EquipNullWeaponAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    EXPECT_THROW(player.equip_weapon(nullptr), NullPointerException);
}

TEST(PlayerTest, EquipWeaponTwiceAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    shared_ptr<Weapon> pistol = make_shared<Weapon>("Pistol", 1000, 10, 100, PISTOL, TERRORIST);

    player.equip_weapon(pistol);

    EXPECT_NO_THROW(player.equip_weapon(pistol));
}

TEST(PlayerTest, GetDroppedWeaponAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    shared_ptr<Weapon> pistol = make_shared<Weapon>("Pistol", 1000, 10, 100, PISTOL, TERRORIST);

    player.equip_weapon(pistol);
    player.drop_weapon(PISTOL);

    EXPECT_THROW(player.get_weapon(MELEE), WeaponNotEquippedException);
}

TEST(PlayerTest, DropNonEquippedWeaponAssertions) {
    Player player("Player", 63, 10000, 0, TERRORIST, 2023);

    EXPECT_THROW(player.drop_weapon(MELEE), WeaponNotEquippedException);
}