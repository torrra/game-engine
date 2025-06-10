Life = ScriptObject:_new()

Life.health = 50
Life.CustomDamageFunction = nil
Life.CustomDeathFunction = nil
Life.CustomRegenFunction = nil

-- Is executed once when the object becomes active
function Life:Start()

end


-- Is executed every tick
function Life:Update(deltaTime)

end

function Life:OnDeath()

    if self.CustomDeathFunction then
        self:CustomDeathFunction()
    end
    
    print("Entity has died")
    DestroyEntity(self.entity, true)
    self.entity = nil
end

function Life:TakeDamage(damage)

    if self.CustomDamageFunction then
        self:CustomDamageFunction(damage)
    end

    self.health = self.health - damage
    print("Health: "..self.health)

    if self.health <= 0 then
        self:OnDeath()
    end
end

function Life:RegenHealth(amount)
    self.health = self.health + amount

    if self.CustomRegenFunction then
        self:CustomRegenFunction(amount)
    end

    print("Health: "..self.health)
end


-- Engine definitions
ScriptObjectTypes.Life = Life
return Life