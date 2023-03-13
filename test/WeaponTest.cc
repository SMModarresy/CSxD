#include "gtest/gtest.h"

#include "models/weapon/Weapon.h"

TEST(WeaponTest, ConstructionAssertions) {
    Weapon weapon("Weapon", 1000, 10, 100, PISTOL, TERRORIST);

    EXPECT_EQ(weapon.get_name(), "Weapon");
    EXPECT_EQ(weapon.get_price(), 1000);
    EXPECT_EQ(weapon.get_damage_per_hit(), 10);
    EXPECT_EQ(weapon.get_money_per_kill(), 100);
    EXPECT_EQ(weapon.get_type(), PISTOL);
}

TEST(WeaponTest, AvailabilityAssertions) {
    Weapon all("Weapon", 1000, 10, 100, PISTOL, ALL);
    Weapon terrorist("Weapon", 1000, 10, 100, PISTOL, TERRORIST);
    Weapon counter_terrorist("Weapon", 1000, 10, 100, PISTOL, COUNTER_TERRORIST);

    EXPECT_TRUE(all.is_available_for(ALL));
    EXPECT_TRUE(all.is_available_for(TERRORIST));
    EXPECT_TRUE(all.is_available_for(COUNTER_TERRORIST));
    EXPECT_FALSE(terrorist.is_available_for(ALL));
    EXPECT_TRUE(terrorist.is_available_for(TERRORIST));
    EXPECT_FALSE(terrorist.is_available_for(COUNTER_TERRORIST));
    EXPECT_FALSE(counter_terrorist.is_available_for(ALL));
    EXPECT_FALSE(counter_terrorist.is_available_for(TERRORIST));
    EXPECT_TRUE(counter_terrorist.is_available_for(COUNTER_TERRORIST));
}

