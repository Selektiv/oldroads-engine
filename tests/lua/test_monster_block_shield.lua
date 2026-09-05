-- Run from the repository root: luajit tests/lua/test_monster_block_shield.lua
-- Tests the real declarative reader; C++ binding/deserialization is covered by MonsterSpellTest.
MonsterType = {}
COMBAT_PHYSICALDAMAGE = 1
MonsterSpell = function()
	return setmetatable({ calls = {} }, {
		__index = function(_, method)
			return function(self, value)
				self.calls[method] = { value = value }
			end
		end,
	})
end

dofile("data/scripts/lib/register_monster_type.lua")

local function read(name, value)
	return readSpell({
		name = name,
		type = COMBAT_PHYSICALDAMAGE,
		blockShield = value,
		soundCast = 1,
		impactCast = 1,
	}, {})
end

assert(read("combat", nil).calls.setBlockShield == nil)
assert(read("combat", true).calls.setBlockShield.value == true)
assert(read("combat", false).calls.setBlockShield.value == false)
assert(read("melee", nil).calls.setBlockShield == nil)

-- Execute each audited monster definition and pass its real projectile through readSpell.
local captured
Game = { createMonsterType = function()
	return { register = function(_, monster) captured = monster end }
end }
loadSpellSoundType = function() return { cast = 0, impact = 0 } end
SOUND_EFFECT_TYPE_SILENCE = 0
local files = {
	"data-canary/monster/humans/amazon.lua",
	"data-canary/monster/humans/valkyrie.lua",
	"data-otservbr-global/monster/humans/amazon.lua",
	"data-otservbr-global/monster/humans/valkyrie.lua",
	"data-otservbr-global/monster/humanoids/minotaur_archer.lua",
}
for _, file in ipairs(files) do
	captured = nil
	dofile(file)
	assert(captured, file)
	local projectiles = 0
	for _, attack in ipairs(captured.attacks) do
		if attack.name == "combat" and attack.type == COMBAT_PHYSICALDAMAGE then
			projectiles = projectiles + 1
			assert(attack.blockShield == true, file)
			assert(not attack.radius and not attack.length, file)
			assert(readSpell(attack, {}).calls.setBlockShield.value == true, file)
		elseif attack.name == "melee" then
			assert(attack.blockShield == nil, file)
		end
	end
	assert(projectiles == 1, file)
end
print("PASS: omitted/true/false forwarding, melee default, and all five monster definitions")
