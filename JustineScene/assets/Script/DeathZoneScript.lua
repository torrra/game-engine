DeathZoneScript = ScriptObject:_new()

-- Is executed once when the object becomes active
function DeathZoneScript:Start()

    self.playerEntity = GetEntity("Player").handle

end


-- Is executed every tick
function DeathZoneScript:Update(deltaTime)

end

function DeathZoneScript:OnTriggerEnter(otherEntity)

    if otherEntity == self.playerEntity then
        print("Player is dead game over")
    end

end

-- Engine definitions
ScriptObjectTypes.DeathZoneScript = DeathZoneScript
return DeathZoneScript