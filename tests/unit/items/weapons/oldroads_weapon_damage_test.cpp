/**
 * Oldroads weapon damage formula tests.
 */

#include "items/weapons/weapons.hpp"

TEST(OldroadsWeaponDamageTest, AppliesFightModeModifiers) {
	constexpr int32_t skill = 30;
	constexpr int32_t attack = 25;
	constexpr int32_t maximumRoll = 99;

	EXPECT_EQ(
		59,
		Weapons::getOldroadsWeaponDamage(
			skill, attack, FIGHTMODE_ATTACK, maximumRoll
		)
	);

	EXPECT_EQ(
		49,
		Weapons::getOldroadsWeaponDamage(
			skill, attack, FIGHTMODE_BALANCED, maximumRoll
		)
	);

	EXPECT_EQ(
		29,
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
		49,
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
		49,
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
		133,
		Weapons::getOldroadsWeaponDamage(
			distanceSkill,
			arrowAttack,
			FIGHTMODE_ATTACK,
			maximumRoll
		)
	);

	EXPECT_EQ(
		111,
		Weapons::getOldroadsWeaponDamage(
			distanceSkill,
			arrowAttack,
			FIGHTMODE_BALANCED,
			maximumRoll
		)
	);

	EXPECT_EQ(
		66,
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
		7,
		Weapons::getOldroadsWeaponDamage(
			fistSkill,
			fistAttack,
			FIGHTMODE_ATTACK,
			maximumRoll
		)
	);

	EXPECT_EQ(
		6,
		Weapons::getOldroadsWeaponDamage(
			fistSkill,
			fistAttack,
			FIGHTMODE_BALANCED,
			maximumRoll
		)
	);

	EXPECT_EQ(
		4,
		Weapons::getOldroadsWeaponDamage(
			fistSkill,
			fistAttack,
			FIGHTMODE_DEFENSE,
			maximumRoll
		)
	);
}

TEST(OldroadsDefenseTest, AppliesClassicFightModeModifiers) {
	constexpr int32_t shieldingSkill = 80;
	constexpr int32_t shieldDefense = 37;
	constexpr int32_t maximumRoll = 99;

	EXPECT_EQ(102, Weapons::getOldroadsDefense(shieldingSkill, shieldDefense, FIGHTMODE_ATTACK, maximumRoll));
	EXPECT_EQ(164, Weapons::getOldroadsDefense(shieldingSkill, shieldDefense, FIGHTMODE_BALANCED, maximumRoll));
	EXPECT_EQ(294, Weapons::getOldroadsDefense(shieldingSkill, shieldDefense, FIGHTMODE_DEFENSE, maximumRoll));
}

TEST(OldroadsDefenseTest, UsesExplicitDefenseRoll) {
	constexpr int32_t shieldingSkill = 80;
	constexpr int32_t shieldDefense = 37;

	EXPECT_EQ(0, Weapons::getOldroadsDefense(shieldingSkill, shieldDefense, FIGHTMODE_BALANCED, 0));
	EXPECT_EQ(83, Weapons::getOldroadsDefense(shieldingSkill, shieldDefense, FIGHTMODE_BALANCED, 50));
	EXPECT_EQ(164, Weapons::getOldroadsDefense(shieldingSkill, shieldDefense, FIGHTMODE_BALANCED, 99));
}

TEST(OldroadsDefenseTest, UsesClassicIntegerStanceRounding) {
	constexpr int32_t shieldingSkill = 80;
	constexpr int32_t maximumRoll = 99;

	// Both values become 21 on full attack: 34 - 13 and 35 - 14.
	EXPECT_EQ(
		Weapons::getOldroadsDefense(shieldingSkill, 34, FIGHTMODE_ATTACK, maximumRoll),
		Weapons::getOldroadsDefense(shieldingSkill, 35, FIGHTMODE_ATTACK, maximumRoll)
	);
}

TEST(OldroadsDefenseTest, ClampsInvalidInputs) {
	EXPECT_EQ(
		Weapons::getOldroadsDefense(0, 37, FIGHTMODE_BALANCED, 99),
		Weapons::getOldroadsDefense(-10, 37, FIGHTMODE_BALANCED, 99)
	);
	EXPECT_EQ(0, Weapons::getOldroadsDefense(80, -37, FIGHTMODE_BALANCED, 99));
	EXPECT_EQ(
		Weapons::getOldroadsDefense(80, 37, FIGHTMODE_BALANCED, 99),
		Weapons::getOldroadsDefense(80, 37, FIGHTMODE_BALANCED, 500)
	);
}

TEST(OldroadsDistanceHitChanceTest, ScalesWithSkillUpToEquipmentCap) {
	EXPECT_EQ(55, Weapons::getOldroadsDistanceHitChance(10, 0, 90));
	EXPECT_EQ(70, Weapons::getOldroadsDistanceHitChance(40, 0, 90));
	EXPECT_EQ(90, Weapons::getOldroadsDistanceHitChance(80, 0, 90));
	EXPECT_EQ(90, Weapons::getOldroadsDistanceHitChance(100, 0, 90));
	EXPECT_EQ(75, Weapons::getOldroadsDistanceHitChance(80, 0, 75));
}

TEST(OldroadsDistanceHitChanceTest, AppliesEquipmentOverridesAndClampsAccuracy) {
	// Explicit hitChance is an absolute override for deliberately configured items.
	EXPECT_EQ(95, Weapons::getOldroadsDistanceHitChance(10, 95, 90));

	// Launchers may provide future equipment-based accuracy bonuses.
	EXPECT_EQ(95, Weapons::getOldroadsDistanceHitChance(80, 0, 90, 5));
	EXPECT_EQ(100, Weapons::getOldroadsDistanceHitChance(80, 0, 90, 20));
	EXPECT_EQ(50, Weapons::getOldroadsDistanceHitChance(-10, 0, 90));
}
