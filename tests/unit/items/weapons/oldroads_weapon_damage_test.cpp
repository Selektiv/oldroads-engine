/**
 * Oldroads weapon damage formula tests.
 */

#include "items/weapons/weapons.hpp"

TEST(OldroadsWeaponDamageTest, AppliesFightModeModifiers) {
	constexpr int32_t skill = 30;
	constexpr int32_t attack = 25;
	constexpr int32_t maximumRoll = 99;

	EXPECT_EQ(
		60,
		Weapons::getOldroadsWeaponDamage(
			skill, attack, FIGHTMODE_ATTACK, maximumRoll
		)
	);

	EXPECT_EQ(
		50,
		Weapons::getOldroadsWeaponDamage(
			skill, attack, FIGHTMODE_BALANCED, maximumRoll
		)
	);

	EXPECT_EQ(
		30,
		Weapons::getOldroadsWeaponDamage(
			skill, attack, FIGHTMODE_DEFENSE, maximumRoll
		)
	);
}

TEST(OldroadsWeaponDamageTest, UsesExplicitDamageRoll) {
	constexpr int32_t skill = 30;
	constexpr int32_t attack = 25;

	EXPECT_EQ(
		0,
		Weapons::getOldroadsWeaponDamage(
			skill, attack, FIGHTMODE_BALANCED, 0
		)
	);

	EXPECT_EQ(
		25,
		Weapons::getOldroadsWeaponDamage(
			skill, attack, FIGHTMODE_BALANCED, 50
		)
	);

	EXPECT_EQ(
		50,
		Weapons::getOldroadsWeaponDamage(
			skill, attack, FIGHTMODE_BALANCED, 99
		)
	);
}

TEST(OldroadsWeaponDamageTest, ClampsInvalidInputs) {
EXPECT_EQ(
	Weapons::getOldroadsWeaponDamage(
		0, 25, FIGHTMODE_BALANCED, 99
	),
	Weapons::getOldroadsWeaponDamage(
		-10, 25, FIGHTMODE_BALANCED, 99
	)
);
	EXPECT_EQ(
		0,
		Weapons::getOldroadsWeaponDamage(
			30, -25, FIGHTMODE_BALANCED, 99
		)
	);

	EXPECT_EQ(
		50,
		Weapons::getOldroadsWeaponDamage(
			30, 25, FIGHTMODE_BALANCED, 500
		)
	);
}