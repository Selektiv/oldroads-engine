/**
 * Canary - A free and open-source MMORPG server emulator
 * Copyright (©) 2019–present OpenTibiaBR <opentibiabr@outlook.com>
 * Repository: https://github.com/opentibiabr/canary
 * License: https://github.com/opentibiabr/canary/blob/main/LICENSE
 * Contributors: https://github.com/opentibiabr/canary/graphs/contributors
 * Website: https://docs.opentibiabr.com/
 */

#include "creatures/monsters/monsters.hpp"
#include "creatures/combat/combat.hpp"
#include "creatures/combat/spells.hpp"
#include "lua/functions/creatures/monster/monster_spell_functions.hpp"
#include "lua/functions/lua_functions_loader.hpp"

TEST(MonsterSpellTest, RejectsUnsupportedDamageCondition) {
	auto spell = std::make_shared<MonsterSpell>();
	spell->name = "combat";
	spell->conditionType = CONDITION_AGONY;
	spellBlock_t spellBlock;

	EXPECT_FALSE(g_monsters().deserializeSpell(spell, spellBlock, "agony monster"));
	EXPECT_FALSE(spellBlock.spell);
}

TEST(MonsterSpellTest, RejectsNonDamageCondition) {
	auto spell = std::make_shared<MonsterSpell>();
	spell->name = "combat";
	spell->conditionType = CONDITION_HASTE;
	spellBlock_t spellBlock;

	EXPECT_FALSE(g_monsters().deserializeSpell(spell, spellBlock, "haste monster"));
	EXPECT_FALSE(spellBlock.spell);
}

TEST(MonsterSpellTest, PhysicalProjectileDefenseIsOptIn) {
	auto spell = std::make_shared<MonsterSpell>();
	spell->name = "combat";
	spell->combatType = COMBAT_PHYSICALDAMAGE;
	spell->shoot = CONST_ANI_BOLT;
	spell->range = 7;

	for (const bool blockShield : { false, true }) {
		spell->blockShield = blockShield;
		spellBlock_t block;
		ASSERT_TRUE(g_monsters().deserializeSpell(spell, block));
		const auto combatSpell = std::dynamic_pointer_cast<CombatSpell>(block.spell);
		ASSERT_TRUE(combatSpell);
		const auto combat = combatSpell->getCombat();
		ASSERT_TRUE(combat);
		EXPECT_EQ(combat->isBlockedByShield(), blockShield);
		EXPECT_TRUE(combat->isBlockedByArmor());
	}
}

TEST(MonsterSpellTest, UnconfiguredAttacksKeepBlockingDefaults) {
	for (const auto name : { "melee", "combat" }) {
		for (const auto type : { COMBAT_PHYSICALDAMAGE, COMBAT_FIREDAMAGE }) {
			auto spell = std::make_shared<MonsterSpell>();
			spell->name = name;
			spell->combatType = type;
			// Exercise area combat as well as normal melee.
			spell->radius = spell->name == "combat" ? 3 : 0;
			EXPECT_FALSE(spell->blockShield);
			spellBlock_t block;
			ASSERT_TRUE(g_monsters().deserializeSpell(spell, block));
			const auto combatSpell = std::dynamic_pointer_cast<CombatSpell>(block.spell);
			ASSERT_TRUE(combatSpell);
			const auto combat = combatSpell->getCombat();
			ASSERT_TRUE(combat);
			EXPECT_EQ(combat->isBlockedByShield(), spell->name == "melee");
			EXPECT_EQ(combat->isBlockedByArmor(), spell->name == "melee" || type == COMBAT_PHYSICALDAMAGE);
		}
	}
}

TEST(MonsterSpellTest, LuaBlockShieldSetterReachesDeserializedCombat) {
	std::unique_ptr<lua_State, decltype(&lua_close)> state { luaL_newstate(), &lua_close };
	ASSERT_TRUE(state);
	MonsterSpellFunctions::init(state.get());
	auto spell = std::make_shared<MonsterSpell>();
	spell->name = "combat";
	spell->combatType = COMBAT_PHYSICALDAMAGE;

	// True followed by false also verifies explicit disabling through Lua.
	for (const bool enabled : { true, false }) {
		lua_getglobal(state.get(), "MonsterSpell");
		lua_getfield(state.get(), -1, "setBlockShield");
		lua_remove(state.get(), -2);
		Lua::pushSharedUserdata<MonsterSpell>(state.get(), spell);
		lua_pushboolean(state.get(), enabled);
		ASSERT_EQ(lua_pcall(state.get(), 2, 1, 0), LUA_OK) << lua_tostring(state.get(), -1);
		ASSERT_TRUE(lua_toboolean(state.get(), -1));
		lua_pop(state.get(), 1);

		spellBlock_t block;
		ASSERT_TRUE(g_monsters().deserializeSpell(spell, block));
		const auto combatSpell = std::dynamic_pointer_cast<CombatSpell>(block.spell);
		ASSERT_TRUE(combatSpell);
		const auto combat = combatSpell->getCombat();
		ASSERT_TRUE(combat);
		EXPECT_EQ(combat->isBlockedByShield(), enabled);
		EXPECT_TRUE(combat->isBlockedByArmor());
	}
}
