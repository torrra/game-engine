DeathZoneScript = ScriptObject:_new()

-- Is executed once when the object becomes active
function DeathZoneScript:Start()

    self.player = GetEntity("Player").handle
    
    local script = GetScriptComponent(self.entity.handle)
    local playerScript = GetScriptComponent(self.player)

    if script then
        self.menu = script.Menu
    end

    if playerScript then
        self.playerLife = playerScript.Life
    end
end


-- Is executed every tick
function DeathZoneScript:Update(deltaTime)

end

function DeathZoneScript:OnTriggerEnter(other)

    if other == self.player and self.menu then
        self.menu:OpenMenu("You died...")
        self.playerLife:TakeDamage(150)

        local allEnemies = GetEntity("Enemy")

        if allEnemies then
            DestroyEntity(allEnemies, true)
        end

    end 
end

-- Engine definitions
ScriptObjectTypes.DeathZoneScript = DeathZoneScript
return DeathZoneScript