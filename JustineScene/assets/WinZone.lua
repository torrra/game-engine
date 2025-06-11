WinZone = ScriptObject:_new()

-- Is executed once when the object becomes active
function WinZone:Start()

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
function WinZone:Update(deltaTime)

end

function WinZone:OnTriggerEnter(other)

    if other == self.player and self.menu then
        self.menu:OpenMenu("You won!")
        self.playerLife:TakeDamage(150) -- destroy player to stop control

        local allEnemies = GetEntity("Enemy")

        if allEnemies then
            DestroyEntity(allEnemies, true) -- destroy all enemies left
        end

    end 
end

-- Engine definitions
ScriptObjectTypes.WinZone = WinZone
return WinZone