# Monster projectile blocking

For declarative monster attacks with `name = "combat"`, set `blockShield = true`
to allow shield or weapon defense. Omission or `false` keeps defense disabled.
The name follows `COMBAT_PARAM_BLOCKSHIELD`; this is defense eligibility, not a
requirement to wear a shield and not a guaranteed complete block.

```lua
{ name = "combat", type = COMBAT_PHYSICALDAMAGE, blockShield = true,
  interval = 2000, chance = 40, minDamage = 0, maxDamage = -80,
  range = 7, shootEffect = CONST_ANI_BOLT, target = false },
```

The corresponding C++ descriptor field is `MonsterSpell::blockShield`, set by
`MonsterSpell:setBlockShield(bool)`. The declarative reader forwards explicit
false as well as true. The deserializer applies it only to the `combat` branch.
Melee retains its existing defense behavior. Named registered spells and
scripted spells own their combat parameters; configure those in their scripts.

This option does not infer blocking from damage type, projectile appearance,
range or area. Physical generic combat still requests armor independently.
A future non-physical or area attack may explicitly opt in if designed to do so.

The initial audited opt-ins are Amazon throwing knives, Valkyrie spears and
minotaur archer bolts. Both bundled Amazon/Valkyrie definitions are updated;
the bundled minotaur archer is in data-otservbr-global. Custom/private copies
need the same explicit property on their corresponding attack.

Player ammunition and thrown weapons keep their existing defense bypass.
Eligible monster projectiles use the normal defense opportunity budget. A
complete defense block can train shielding only with a shield and remaining
training allowance, under the existing Oldroads rules.

Validation:

- `MonsterSpellTest.*`: opt-in combat construction, preserved defaults and Lua
  setter to combat forwarding, including true followed by false.
- `luajit tests/lua/test_monster_block_shield.lua`: real Lua reader and the five
  audited definitions (standalone binding stubs).
- Live: replenish training allowance with a blood hit, then receive isolated
  ranged shots with a shield; compare stances and confirm shielding can advance.
  Player-fired bolts must still bypass defense.
