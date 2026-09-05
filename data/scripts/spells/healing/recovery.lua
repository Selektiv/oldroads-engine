local combat = Combat()
combat:setParameter(COMBAT_PARAM_EFFECT, CONST_ME_MAGIC_GREEN)
combat:setParameter(COMBAT_PARAM_AGGRESSIVE, false)

-- Configuration Settings
local config = {
    percent = 0.01,     -- 1% of Max HP per tick
    minRegen = 8,      -- Minimum HP gained per tick
    maxRegen = 16,     -- Maximum HP gained per tick
    duration = 60 * 1000, -- 1 minute total duration
    healInterval = 2 * 1000   -- Heal every 2 second
}

local spell = Spell("instant")

function spell.onCastSpell(creature, variant)
    if not combat:execute(creature, variant) then
        return false
    end

    -- Calculate percent-based regen
    local maxHp = creature:getMaxHealth()
    local healAmount = math.floor(maxHp * config.percent)

    -- Clamp heal amount between minRegen and maxRegen limits
    healAmount = math.max(config.minRegen, math.min(config.maxRegen, healAmount))

    -- Create dynamic regeneration condition
    local condition = Condition(CONDITION_REGENERATION)
    condition:setParameter(CONDITION_PARAM_TICKS, config.duration)
    condition:setParameter(CONDITION_PARAM_HEALTHGAIN, healAmount)
    condition:setParameter(CONDITION_PARAM_HEALTHTICKS, config.healInterval)
    condition:setParameter(CONDITION_PARAM_BUFF_SPELL, true)

    creature:addCondition(condition)
    return true
end

spell:name("Recovery")
spell:words("utura")
spell:group("healing")
spell:vocation("knight;true", "elite knight;true", "paladin;true", "royal paladin;true")
spell:castSound(SOUND_EFFECT_TYPE_SPELL_RECOVERY)
spell:id(159)
spell:cooldown(1 * 60 * 1000)
spell:groupCooldown(1 * 1000)
spell:level(30)
spell:mana(60)
spell:isSelfTarget(true)
spell:isAggressive(false)
spell:register()