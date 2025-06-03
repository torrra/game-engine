Life = ScriptObject:_new()

Life.health = 3

-- Is executed once when the object becomes active
function Life:Start()

end


-- Is executed every tick
function Life:Update(deltaTime)

end

function Life:OnDeath()
    print("Entity has died")
    DestroyEntity(self.entity)
    self.entity = nil
end

function Life:TakeDamage(damage)

    self.health = self.health - damage
    print("Health: "..self.health)

    if self.health <= 0 then
        self:OnDeath()
    end
end

function Life:RegenHealth(amount)
    self.health = self.health + amount
    print("Health: "..self.health)
end


-- Engine definitions
ScriptObjectTypes.Life = Life
return Life