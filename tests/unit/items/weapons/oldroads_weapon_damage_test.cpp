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
TEST(OldroadsWeaponDamageTest, DocumentsClassicArrowDamageCeilings) {
	constexpr int32_t distanceSkill = 80;
	constexpr int32_t arrowAttack = 25;
	constexpr int32_t maximumRoll = 99;

	EXPECT_EQ(
		134,
		Weapons::getOldroadsWeaponDamage(
			distanceSkill,
			arrowAttack,
			FIGHTMODE_ATTACK,
			maximumRoll
		)
	);

	EXPECT_EQ(
		112,
		Weapons::getOldroadsWeaponDamage(
			distanceSkill,
			arrowAttack,
			FIGHTMODE_BALANCED,
			maximumRoll
		)
	);

	EXPECT_EQ(
		67,
		Weapons::getOldroadsWeaponDamage(
			distanceSkill,
			arrowAttack,
			FIGHTMODE_DEFENSE,
			maximumRoll
		)
	);
}

TEST(OldroadsWeaponDamageTest, DocumentsClassicFistDamageCeilings) {
	constexpr int32_t fistSkill = 10;
	constexpr int32_t fistAttack = 7;
	constexpr int32_t maximumRoll = 99;

	EXPECT_EQ(
		8,
		Weapons::getOldroadsWeaponDamage(
			fistSkill,
			fistAttack,
			FIGHTMODE_ATTACK,
			maximumRoll
		)
	);

	EXPECT_EQ(
		7,
		Weapons::getOldroadsWeaponDamage(
			fistSkill,
			fistAttack,
			FIGHTMODE_BALANCED,
			maximumRoll
		)
	);

	EXPECT_EQ(
		5,
		Weapons::getOldroadsWeaponDamage(
			fistSkill,
			fistAttack,
			FIGHTMODE_DEFENSE,
			maximumRoll
		)
	);
}
