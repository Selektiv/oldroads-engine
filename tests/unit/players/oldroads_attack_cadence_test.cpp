/**
 * Oldroads attack cadence tests.
 */

#include "creatures/players/player.hpp"
#include "items/items.hpp"
#include "lua/creature/actions.hpp"
#include "utils/tools.hpp"

TEST(OldroadsAttackCadenceTest, RestartsTheAttackClockAtSupplyUseTime) {
	UPDATE_OTSYS_TIME();

	auto player = std::make_shared<Player>();
	ASSERT_EQ(0, player->getLastAttack());

	player->restartAttackSwing();

	EXPECT_EQ(static_cast<uint64_t>(OTSYS_TIME()), player->getLastAttack());
	EXPECT_TRUE(player->checkLastAttackWithin(1));
}

TEST(OldroadsAttackCadenceTest, EveryRuneAndPotionRestartsTheSwing) {
	ItemType rune;
	rune.type = ITEM_TYPE_RUNE;
	EXPECT_TRUE(Actions::shouldRestartAttackSwing(rune, 0, false));

	ItemType potion;
	potion.type = ITEM_TYPE_POTION;
	EXPECT_TRUE(Actions::shouldRestartAttackSwing(potion, 0, false));
}

TEST(OldroadsAttackCadenceTest, OnlyConsumedManaAndLifeFluidsRestartTheSwing) {
	ItemType fluid;
	fluid.group = ITEM_GROUP_FLUID;

	EXPECT_TRUE(Actions::shouldRestartAttackSwing(fluid, FLUID_MANA, true));
	EXPECT_TRUE(Actions::shouldRestartAttackSwing(fluid, FLUID_LIFE, true));
	EXPECT_FALSE(Actions::shouldRestartAttackSwing(fluid, FLUID_WATER, true));
	EXPECT_FALSE(Actions::shouldRestartAttackSwing(fluid, FLUID_MANA, false));
}

TEST(OldroadsAttackCadenceTest, OrdinaryToolsDoNotRestartTheSwing) {
	ItemType tool;
	EXPECT_FALSE(Actions::shouldRestartAttackSwing(tool, 0, false));
	EXPECT_FALSE(Actions::shouldRestartAttackSwing(tool, 0, true));
}
